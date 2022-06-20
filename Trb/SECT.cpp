#include "SECT.h"

#include "RELC.h"
#include "Utils.h"
#include <iostream>

bool SECT::isLinked = false;

SECT::SECT(FILE* pFile) : TRBTag(pFile)
{
	m_buffer = new char[size];
	ReadFileData(m_buffer, 1, size, pFile);
}

SECT::~SECT()
{
	delete[] m_buffer;
}

char* SECT::GetBuffer()
{
	return m_buffer;
}

void SECT::LinkRELC(RELC* pRelc)
{
	// todo: support files with few HDRXs
	assert(!isLinked && "SECT can't be linked twice");

	char* buffer = GetBuffer();
	size_t count = pRelc->GetCount();

	for (size_t i = 0; i < count; i++)
	{
		RELCEntry entry = pRelc->GetEntry(i);
		if (entry.hdrx1 != 0 || entry.hdrx2 != 0) break;

		// updating file relative pointers to be memory relative pointers
		*(int*)(buffer + entry.offset) += (unsigned int)buffer;
	}

	isLinked = true;
}

void SECT::UnlinkRELC(RELC* pRelc)
{
	// todo: support files with few HDRXs
	assert(isLinked && "SECT isn't linked");

	char* buffer = GetBuffer();
	size_t count = pRelc->GetCount();

	for (size_t i = 0; i < count; i++)
	{
		RELCEntry entry = pRelc->GetEntry(i);
		if (entry.hdrx1 != 0 || entry.hdrx2 != 0) break;

		// updating file relative pointers to be memory relative pointers
		*(int*)(buffer + entry.offset) -= (unsigned int)buffer;
	}

	isLinked = false;
}
