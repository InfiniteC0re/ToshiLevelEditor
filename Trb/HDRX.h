#pragma once
#include "Tag.h"
#include <vector>

struct HDRXFile
{
	unsigned short m_unk1;
	unsigned short m_unk2;
	unsigned int m_size;
	unsigned int m_zero;
	unsigned int m_flag;
};

class HDRX : public TRBTag
{
public:
	HDRX();
	HDRX(FILE* pFile);

	virtual void Write(FILE* pFile);

	virtual void Calculate(TSFL* tsfl);

private:
	unsigned short m_flag1;
	unsigned short m_flag2;
	size_t m_fileCount;
	std::vector<HDRXFile> m_files;
};

