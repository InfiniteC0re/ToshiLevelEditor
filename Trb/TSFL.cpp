#define _CRT_SECURE_NO_WARNINGS
#include "TSFL.h"
#include "Utils.h"

#include <cassert>

TSFL::TSFL() : TRBTag("TSFL", 0)
{
	strncpy(m_trbf, "TRBF", 4);

	m_HDRX = new HDRX();
	m_SECT = new SECT();
	m_RELC = new RELC();
	m_SYMB = new SYMB();
}

TSFL::TSFL(FILE* pFile) : TRBTag(pFile)
{
	// reading magic number
	ReadFileData(m_trbf, sizeof(m_trbf), 1, pFile);

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

void TSFL::Link()
{
	// updating all the pointers to be relative to the program memory
	if (!m_SECT->IsLinked()) m_SECT->LinkRELC(m_RELC);
	if (!m_SYMB->IsLinked()) m_SYMB->LinkSECT(m_SECT);
}

void TSFL::Unlink()
{
	// updating all the pointers to be relative to the file memory
	if (m_SECT->IsLinked()) m_SECT->UnlinkRELC(m_RELC);
	if (m_SYMB->IsLinked()) m_SYMB->UnlinkSECT(m_SECT);
}

void* TSFL::DumpSECT(const char* filepath)
{
	FILE* file;
	errno_t err = fopen_s(&file, filepath, "wb");

	if (err == 0)
	{
		Unlink();
		fwrite(m_SECT->GetBuffer(), m_SECT->GetBufferSize(), 1, file);
		fclose(file);
		Link();

		std::cout << "Dumped SECT to " << filepath << std::endl;
	}
	else
	{
		std::cerr << "Unable to dump SECT to" << filepath << std::endl;
	}

	return nullptr;
}

void TSFL::AddSymbol(unsigned short hdrx, std::string name, short nameID, void* ptr)
{
	// unlinking all symbols before adding a new one
	Unlink();

	assert(ptr != nullptr && "The pointer can't be null");
	assert(m_SECT->IsPtrInBounds(ptr) && "The pointer isn't in bounds of SECT");

	// getting a file relative pointer
	size_t outUnlinked = (size_t)ptr - (size_t)m_SECT->GetBuffer();
	m_SYMB->Add(hdrx, name, nameID, (void*)outUnlinked);
	
	// linking them back
	Link();
}

void TSFL::Write(FILE* pFile)
{
	// TSFL
	TRBTag::Write(pFile);
	fwrite(m_trbf, 4, 1, pFile);

	// other tags
	m_HDRX->Write(pFile);
	m_SECT->Write(pFile);
	m_RELC->Write(pFile);
	m_SYMB->Write(pFile);
}

void TSFL::Calculate(TSFL* tsfl)
{
	m_HDRX->Calculate(this);
	m_SECT->Calculate(this);
	m_RELC->Calculate(this);
	m_SYMB->Calculate(this);

	size = 8 * 4 + sizeof(m_trbf) + m_HDRX->size + m_SECT->size + m_RELC->size + m_SYMB->size;
}
