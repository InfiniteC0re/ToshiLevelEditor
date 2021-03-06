#pragma once
#include <cstdio>

class TSFL;

class TRBTag
{
public:
	TRBTag(const char* name, size_t size);
	TRBTag(FILE* pFile);

	/* Writing to the file */
	virtual void Write(FILE* pFile);

	/* Recalculating size of the tag */
	virtual void Calculate(TSFL* tsfl);

	/* Aligns position to 4 bytes */
	void ValidateAlignment(FILE* pFile);

	char name[4];
	size_t size;
};