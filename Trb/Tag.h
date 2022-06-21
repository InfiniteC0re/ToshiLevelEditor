#pragma once
#include <cstdio>

class TRBTag
{
public:
	TRBTag(const char* name, size_t size);
	TRBTag(FILE* pFile);

	char name[4];
	size_t size;
};