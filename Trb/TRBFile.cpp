#include "TRBFile.h"
#include <iostream>

TRBFile::TRBFile()
{
	m_err = 0;
	m_TSFL = new TSFL();
}

TRBFile::TRBFile(const char* path)
{
	FILE* file;

	m_TSFL = nullptr;
	m_err = fopen_s(&file, path, "rb");
	assert(m_err == 0 && "Unable to open the file");
	
	if (file)
	{
		// read the file
		m_TSFL = new TSFL(file);

		// close the file
		fclose(file);
	}
}

TRBFile::~TRBFile()
{
	delete m_TSFL;
}

TSFL* TRBFile::GetTSFL()
{
	return m_TSFL;
}

errno_t TRBFile::GetErrorCode()
{
	return m_err;
}

void TRBFile::Save(const char* path)
{
	FILE* file;

	m_err = fopen_s(&file, path, "wb");
	assert(m_err == 0 && "Unable to open the file");

	if (file)
	{
		m_TSFL->Unlink();
		m_TSFL->Calculate(m_TSFL);
		m_TSFL->Write(file);
		m_TSFL->Link();

		// close the file
		fclose(file);
	}
}