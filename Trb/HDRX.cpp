#include "HDRX.h"
#include <iostream>

HDRX::HDRX(FILE* pFile) : TRBTag(pFile)
{
	// todo: hdrx data
	fseek(pFile, size, SEEK_CUR);
}
