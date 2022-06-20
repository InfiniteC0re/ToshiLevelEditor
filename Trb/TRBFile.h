#pragma once
#include "Utils.h"
#include "TSFL.h"

class TRBFile
{
public:
	TRBFile(const char* path);
	~TRBFile();

	TSFL* GetTSFL();

	errno_t GetErrorCode();

private:
	TSFL* m_TSFL;
	errno_t m_err;
};