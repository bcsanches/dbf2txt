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

#ifndef DBF_FILE_H
#define DBF_FILE_H

#include <fstream>
#include <vector>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

struct DbfHeader
{
	uint8_t m_iType;
	char m_arcLastUpdate[3];

	uint32_t m_uNumRecords;

	uint16_t m_uFirstRecordOffset;
	uint16_t m_uRecordSize;

	char m_uReserved[15];
	uint8_t m_fFlags;
	uint8_t m_uCodePageMark;

	char m_uReserved2[2];
};

#pragma pack(push)
#pragma pack(1)
struct DbfRecord
{
	char m_archName[11];
	char chFieldType;

	uint32_t m_uDisplacement;
	uint8_t m_uLength;
	uint8_t m_uDecimalPlaces;
	uint8_t m_fFlags;

	uint32_t m_uNextValue;
	uint8_t m_uStepValue;
	char m_uReserved[8];
};
#pragma pack(pop)

class DbfFile
{
	public:
		DbfFile(const char *szFileName);

		void DumpAll(const char *szDestFileName);
		void DumpFields(const char *szDestFileName, const char **fields, size_t numFields);

	private:
		std::ifstream m_clFile;

		DbfHeader m_stHeader;
		std::vector<DbfRecord> m_vecRecords;

		size_t m_szRowSize = 0;
		size_t m_szLargestFieldSize = 0;
};


#endif
