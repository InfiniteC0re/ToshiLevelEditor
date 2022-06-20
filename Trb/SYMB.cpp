#include "SYMB.h"

#include "Utils.h"
#include "SECT.h"
#include <iostream>

SYMB::SYMB(FILE* pFile) : TRBTag(pFile)
{
	ReadFileData(&m_count, sizeof(size_t), 1, pFile);
	m_entries = new SYMBEntry[m_count];
	m_entriesNames = new std::string[m_count];

	long namesOffset = ftell(pFile) + 12 * m_count;
	ReadFileData(m_entries, sizeof(SYMBEntry), m_count, pFile);

	for (size_t i = 0; i < m_count; i++)
	{
		int nameOffset = namesOffset + m_entries[i].nameOffset;
		m_entriesNames[i] = read_str_offset(pFile, nameOffset);
	}
}

SYMB::~SYMB()
{
	if (m_entries)
	{
		delete[] m_entries;
		delete[] m_entriesNames;

		m_count = 0;
	}
}

size_t SYMB::GetCount() const
{
	return m_count;
}

const SYMBEntry* SYMB::GetEntry(size_t index) const
{
	if (m_count > index)
	{
		return &m_entries[index];
	}

	return nullptr;
}

const std::string SYMB::GetEntryName(size_t index) const
{
	if (m_count > index)
	{
		return m_entriesNames[index];
	}

	return "";
}

void SYMB::LinkSECT(SECT* pSect)
{
	// todo: support files with few HDRXs
	static bool isLinked = false;
	assert(!isLinked && "SYMB can't be linked twice");

	unsigned int dataStart = (unsigned int)pSect->GetBuffer();

	for (size_t i = 0; i < m_count; i++)
	{
		m_entries[i].dataOffset += dataStart;
	}

	isLinked = true;
}
