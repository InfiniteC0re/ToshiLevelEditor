#include "HDRX.h"
#include <iostream>

HDRX::HDRX() : TRBTag("HDRX", 0)
{
}

HDRX::HDRX(FILE* pFile) : TRBTag(pFile)
{
	// todo: hdrx data
	fseek(pFile, size, SEEK_CUR);
}
