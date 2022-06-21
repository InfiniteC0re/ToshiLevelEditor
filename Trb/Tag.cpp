#define _CRT_SECURE_NO_WARNINGS
#include "Tag.h"
#include "Utils.h"

TRBTag::TRBTag(const char* name, size_t size)
{
	strncpy(TRBTag::name, name, 4);
	TRBTag::size = size;
}

TRBTag::TRBTag(FILE* pFile)
{
	name[0] = 0;
	size = 0;

	ReadFileData(name, sizeof(name), 1, pFile);
	ReadFileData(&size, sizeof(size), 1, pFile);
}

void TRBTag::Write(FILE* pFile)
{
	// writing tag name
	fwrite(name, 4, 1, pFile);
	// writing tag size
	fwrite(&size, 4, 1, pFile);
}

void TRBTag::Calculate(TSFL* tsfl)
{
	size = 0;
}
