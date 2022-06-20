#include "Tag.h"
#include "Utils.h"
#include <iostream>

TRBTag::TRBTag(FILE* pFile)
{
	name[0] = 0;
	size = 0;

	ReadFileData(this, sizeof(TRBTag), 1, pFile);
}
