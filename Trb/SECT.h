#pragma once
#include "Tag.h"

struct RELC;

class SECT : public TRBTag
{
public:
	SECT(FILE* pFile);
	~SECT();

	/* Returns buffer of the SECT data */
	char* GetBuffer();

	/* Update pointers to be relative to the program memory */
	void LinkRELC(RELC* pRelc);

	/* Update pointers to be relative to file */
	void UnlinkRELC(RELC* pRelc);

private:
	static bool isLinked;

	char* m_buffer;
};

