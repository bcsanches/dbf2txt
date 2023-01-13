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

#include <iostream>

#include "DbfFile.h"

using namespace std;

//Produtos: NOME CODIGO AREA SUBCODIGO FORN QUANT MINIMO PRECOBRUTO PRECOCUSTO PRECOVENDA CODEX

int main(int argc, const char **argv)
{
	if(argc < 3)
	{
		cout << "Usage: dbfmanager <input.dbf> <output.txt>" << endl;

		return 0;
	}

	try
	{
		DbfFile file(argv[1]);

		if(argc == 3)
			file.DumpAll(argv[2]);
		else
		{
			file.DumpFields(argv[2], argv+3, argc-3);
		}
	}
	catch(const exception &e)
	{
		cerr << e.what(); 
		return -1;
	}


	return 0;
}