#pragma once
#include <cstdio>

class TRBTag
{
public:
	TRBTag(FILE* pFile);

	char name[4];
	int size;
};