/*
db2txt
Copyright (c) 2011 Bruno Sanches  http://code.google.com/p/dbf2txt

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


#include "DbfFile.h"

#include <algorithm>
#include <iostream>
#include <sstream>

DbfFile::DbfFile(const char *szFileName):
	m_clFile(szFileName, std::ios_base::binary | std::ios_base::in)
{
	if(!m_clFile.good())
		throw std::logic_error("Cannot open file");

	m_clFile.read(reinterpret_cast<char *>(&m_stHeader), sizeof(m_stHeader));
	size_t sz = sizeof(DbfRecord);

	const auto numRecords = m_stHeader.m_uNumRecords;
	
	for(unsigned i = 0;i < numRecords; ++i)
	{
		char end;
		m_clFile.read(&end, 1);
		if(end == 0x0D)
			break;

		//corrupted file? Abort to avoid infinite loop
		if (i == numRecords)
			break;

		m_vecRecords.push_back(DbfRecord());
		DbfRecord &record = m_vecRecords.back();

		memcpy(&record, &end, 1);
		m_clFile.read(reinterpret_cast<char *>(&record)+1, sizeof(DbfRecord)-1);

		m_szRowSize += record.m_uLength;
		m_szLargestFieldSize = std::max(m_szLargestFieldSize, static_cast<size_t>(record.m_uLength));
	}
}

void DbfFile::DumpAll(const char *szDestFileName)
{
	std::ofstream out(szDestFileName);

	std::vector<char> vecBuffer;
	vecBuffer.resize(m_szLargestFieldSize);

	size_t uTotalBytes = 0;
	size_t uNumRecords = 0;
	while(!m_clFile.eof())
	{
		char deleted;
		m_clFile.read(&deleted, 1);
		if(deleted == 0x2A)
		{
			m_clFile.seekg(m_szRowSize, std::ios_base::cur);
			continue;
		}

		if (m_clFile.fail())
			break;

		if (deleted == 0x1A) //end-of-file marker
			break;

		for(size_t i = 0;i < m_vecRecords.size(); ++i)
		{
			DbfRecord &record = m_vecRecords[i];
									
			m_clFile.read(&vecBuffer[0], record.m_uLength);
			out.write(&vecBuffer[0], record.m_uLength);
			uTotalBytes += record.m_uLength;
		}
		++uNumRecords;
		++uTotalBytes;

		out << std::endl;
	}

	std::cout << "Created " << uNumRecords << " records " << uTotalBytes << " bytes." << std::endl;
}

struct FieldInfo
{
	const DbfRecord &rstRecord;
	size_t szSkipSize;

	FieldInfo(const DbfRecord &rec):
		rstRecord(rec),
		szSkipSize(0)
	{		
	}

	FieldInfo &operator=(const FieldInfo &rhs)
	{
		return *this;
	}
};

void DbfFile::DumpFields(const char *szDestFileName, const char **fields, size_t numFields)
{
	std::vector<FieldInfo> vecFields;
	vecFields.reserve(numFields);

	//Build a sorted list (in file fields ordering) with all fields
	size_t current = 0;
	for(size_t i = 0;(i < m_vecRecords.size()) && (current < numFields); ++i)
	{
		if(strncmp(m_vecRecords[i].m_archName, fields[current], 11) == 0)
		{
			vecFields.emplace_back<FieldInfo>(m_vecRecords[i]);
			++current;
		}
	}

	if(current < numFields)
	{
		std::stringstream stream;
		stream << "Field not found: " << fields[current];
		throw std::logic_error(stream.str().c_str());
	}

	//Now build the skip table	
	current = 0;
	size_t szEndOfRowSeek = 0;
	for(size_t i = 0;i < numFields; ++i)
	{
		for(;current < m_vecRecords.size(); ++current)
		{
			if(&vecFields[i].rstRecord == &m_vecRecords[current])
			{
				szEndOfRowSeek += vecFields[i].szSkipSize;
				szEndOfRowSeek += vecFields[i].rstRecord.m_uLength;
				++current;
				break;
			}

			vecFields[i].szSkipSize += m_vecRecords[current].m_uLength;
		}
	}
	szEndOfRowSeek = szEndOfRowSeek == m_szRowSize ? 0 : m_szRowSize - szEndOfRowSeek;

	//Finally, do the output work
	std::ofstream out(szDestFileName);

	std::vector<char> vecBuffer;
	vecBuffer.resize(m_szLargestFieldSize);

	size_t uTotalBytes = 0;
	size_t uNumRecords = 0;
	while(uNumRecords < m_stHeader.m_uNumRecords)
	{
		char deleted;
		m_clFile.read(&deleted, 1);
		if(deleted == 0x2A)
		{
			m_clFile.seekg(m_szRowSize, std::ios_base::cur);
			continue;
		}
		
		for(size_t i = 0;i < numFields; ++i)
		{
			if(vecFields[i].szSkipSize > 0)
			{				
				m_clFile.seekg(vecFields[i].szSkipSize, std::ios_base::cur);
			}

			const DbfRecord &record = vecFields[i].rstRecord;
									
			m_clFile.read(&vecBuffer[0], record.m_uLength);
			out.write(&vecBuffer[0], record.m_uLength);

			uTotalBytes += record.m_uLength;			
		}

		if(szEndOfRowSeek > 0)
			m_clFile.seekg(szEndOfRowSeek, std::ios_base::cur);

		out << std::endl;
		++uNumRecords;
		++uTotalBytes;
	}

	std::cout << "Created " << uNumRecords << ", records " << uTotalBytes << " bytes." << std::endl;
}
