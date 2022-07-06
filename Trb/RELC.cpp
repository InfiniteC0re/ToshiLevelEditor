#include "RELC.h"

#include "Utils.h"

#include <cassert>
#include <iostream>

RELC::RELC() : TRBTag("RELC", 0)
{
	
}

RELC::RELC(FILE* pFile) : TRBTag(pFile)
{
	if (strncmp(name, "RELC", 4) == 0)
	{
		int count;
		ReadFileData(&count, sizeof(size_t), 1, pFile);
		assert(count >= 0 && "Count of RELC entries cannot be negative");
		m_entries.resize(count);
		ReadFileData(m_entries.data(), sizeof(RELCEntry), count, pFile);
	}
	else
	{
		// RELC is missing so go back to make it possible to read SYMB
		fseek(pFile, -8, SEEK_CUR);
	}
}

RELC::~RELC()
{
	m_entries.clear();
}

size_t RELC::GetCount() const
{
	return m_entries.size();
}

RELCEntry RELC::GetEntry(size_t index) const
{
	if (GetCount() > index)
	{
		return m_entries[index];
	}

	return { };
}

const RELCEntry* RELC::GetEntries() const
{
	return m_entries.data();
}

void RELC::Add(short hdrx1, short hdrx2, void* ptr)
{
	m_entries.push_back({ hdrx1, hdrx2, (size_t)ptr });
}

void RELC::Calculate(TSFL* tsfl)
{
	size = sizeof(size_t) + sizeof(RELCEntry) * m_entries.size();
}

void RELC::Write(FILE* pFile)
{
	TRBTag::Write(pFile);
	
	size_t count = m_entries.size();
	fwrite(&count, sizeof(count), 1, pFile);
	fwrite(m_entries.data(), sizeof(RELCEntry), count, pFile);
}
