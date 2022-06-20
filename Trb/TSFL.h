#pragma once
#include "Tag.h"
#include "HDRX.h"
#include "SECT.h"
#include "RELC.h"
#include "SYMB.h"

class TSFL : public TRBTag
{
public:
	TSFL(FILE* pFile);
	~TSFL();

	HDRX* GetHDRX();
	SECT* GetSECT();
	RELC* GetRELC();
	SYMB* GetSYMB();

	/* Dump data in SECT to file */
	void* DumpSECTToFile(const char* filepath);

private:
	char trbf[4];

	HDRX* m_HDRX;
	SECT* m_SECT;
	RELC* m_RELC;
	SYMB* m_SYMB;
};

