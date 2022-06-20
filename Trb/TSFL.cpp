#include "TSFL.h"
#include "Utils.h"
#include <iostream>

TSFL::TSFL(FILE* pFile) : TRBTag(pFile)
{
	// reading magic number
	ReadFileData(trbf, sizeof(trbf), 1, pFile);

	// reading file
	m_HDRX = new HDRX(pFile);
	m_SECT = new SECT(pFile);
	m_RELC = new RELC(pFile);
	m_SYMB = new SYMB(pFile);

	// updating pointers
	m_SECT->LinkRELC(m_RELC);
	m_SYMB->LinkSECT(m_SECT);
}

TSFL::~TSFL()
{
	// freeing the memory
	delete m_HDRX;
	delete m_SECT;
	delete m_RELC;
	delete m_SYMB;
}

HDRX* TSFL::GetHDRX()
{
	return m_HDRX;
}

SECT* TSFL::GetSECT()
{
	return m_SECT;
}

RELC* TSFL::GetRELC()
{
	return m_RELC;
}

SYMB* TSFL::GetSYMB()
{
	return m_SYMB;
}

void* TSFL::DumpSECTToFile(const char* filepath)
{
	FILE* file;
	errno_t err = fopen_s(&file, filepath, "wb");

	if (err == 0)
	{
		fwrite(m_SECT->GetBuffer(), m_SECT->size, 1, file);
		fclose(file);

		std::cout << "Dumped SECT to " << filepath << std::endl;
	}
	else
	{
		std::cerr << "Unable to dump SECT to" << filepath << std::endl;
	}

	return nullptr;
}
