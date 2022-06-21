#include "Tag.h"
#include "Utils.h"
#include <iostream>

TRBTag::TRBTag(const char* name, size_t size)
{
	strncpy_s(TRBTag::name, name, _TRUNCATE);
	TRBTag::size = size;
}

TRBTag::TRBTag(FILE* pFile)
{
	name[0] = 0;
	size = 0;

	ReadFileData(this, sizeof(TRBTag), 1, pFile);
}
