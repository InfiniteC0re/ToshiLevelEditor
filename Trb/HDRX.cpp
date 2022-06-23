#include "HDRX.h"
#include <iostream>
#include "Utils.h"
#include "TSFL.h"

HDRX::HDRX() : TRBTag("HDRX", 0), m_flag1(1), m_flag2(1), m_fileCount(0)
{
	
}

HDRX::HDRX(FILE* pFile) : TRBTag(pFile)
{
	ReadFileData(&m_flag1, sizeof(m_flag1), 1, pFile);
	ReadFileData(&m_flag2, sizeof(m_flag2), 1, pFile);
	ReadFileData(&m_fileCount, sizeof(m_fileCount), 1, pFile);
	m_files.resize(m_fileCount);

	ReadFileData(m_files.data(), sizeof(HDRXFile), m_fileCount, pFile);
}

void HDRX::Write(FILE* pFile)
{
	TRBTag::Write(pFile);

	// asserts
	assert(m_fileCount > 0 && "HDRX is empty");

	// write the data
	fwrite(&m_flag1, sizeof(m_flag1), 1, pFile);
	fwrite(&m_flag2, sizeof(m_flag2), 1, pFile);
	fwrite(&m_fileCount, sizeof(m_fileCount), 1, pFile);
	fwrite(m_files.data(), sizeof(HDRXFile), m_fileCount, pFile);
}

void HDRX::Calculate(TSFL* tsfl)
{
	// making our own HDRX
	m_files.clear();
	m_files.push_back({ 0, 0, tsfl->GetSECT()->GetBufferSize(), 0, 0 });

	m_fileCount = m_files.size();
	size = sizeof(m_flag1) + sizeof(m_flag2) + sizeof(m_fileCount) + sizeof(HDRXFile) * m_fileCount;
}

size_t HDRX::GetFileCount()
{
	return m_fileCount;
}
