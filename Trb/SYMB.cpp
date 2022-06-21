#include "SYMB.h"

#include "Utils.h"
#include "SECT.h"

#include <iostream>

SYMB::SYMB() : TRBTag("SYMB", 0)
{
	isLinked = false;
}

SYMB::SYMB(FILE* pFile) : TRBTag(pFile)
{
	isLinked = false;

	int count;
	ReadFileData(&count, sizeof(size_t), 1, pFile);
	m_entries.resize(count);
	m_entriesNames.resize(count);

	long namesOffset = ftell(pFile) + 12 * count;
	ReadFileData(m_entries.data(), sizeof(SYMBEntry), count, pFile);

	for (size_t i = 0; i < count; i++)
	{
		int nameOffset = namesOffset + m_entries[i].nameOffset;
		m_entriesNames[i] = read_str_offset(pFile, nameOffset);
	}
}

SYMB::~SYMB()
{
	m_entries.clear();
	m_entriesNames.clear();
}

inline size_t SYMB::GetCount() const
{
	return m_entries.size();
}

bool SYMB::IsLinked() const
{
	return isLinked;
}

const SYMBEntry* SYMB::GetEntry(size_t index) const
{
	if (GetCount() > index)
	{
		return &m_entries[index];
	}

	return nullptr;
}

const std::string SYMB::GetEntryName(size_t index) const
{
	if (GetCount() > index)
	{
		return m_entriesNames[index];
	}

	return "";
}

void* SYMB::Find(std::string name) const
{
	int count = GetCount();
	for (size_t i = 0; i < count; i++)
	{
		if (m_entriesNames[i] == name) return m_entries[i].dataOffset;
	}

	return nullptr;
}

void SYMB::LinkSECT(SECT* pSect)
{
	// todo: support files with few HDRXs
	assert(!isLinked && "SYMB can't be linked twice");

	unsigned int dataStart = (unsigned int)pSect->GetBuffer();

	int count = GetCount();
	for (size_t i = 0; i < count; i++)
	{
		m_entries[i].dataOffset += dataStart;
	}

	isLinked = true;
}

void SYMB::UnlinkSECT(SECT* pSect)
{
	// todo: support files with few HDRXs
	assert(isLinked && "SYMB can't be unlinked twice");

	unsigned int dataStart = (unsigned int)pSect->GetBuffer();

	int count = GetCount();
	for (size_t i = 0; i < count; i++)
	{
		m_entries[i].dataOffset -= dataStart;
	}

	isLinked = false;
}

void SYMB::Add(unsigned short hdrx, std::string name, short nameID, void* ptr)
{
	m_entriesNames.push_back(name);

	SYMBEntry symb{ hdrx, 0, nameID, (char*)ptr };
	m_entries.push_back(symb);
}
