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

	size_t count;
	ReadFileData(&count, sizeof(size_t), 1, pFile);
	m_entries.resize(count);
	m_entriesNames.resize(count);

	long namesOffset = ftell(pFile) + sizeof(SYMBEntry) * count;
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

const unsigned short SYMB::CreateNameIDHash(std::string name) const
{
	int hash = 0;
	for (size_t i = 0; i < name.size(); i++)
	{
		hash = ((hash << 5) & 0x1FFFE0) - hash + name[i];
		hash &= 0xFFFF;
	}
	return hash;
}

const SYMBEntry* SYMB::Find(std::string name) const
{
	size_t count = GetCount();
	for (size_t i = 0; i < count; i++)
	{
		if (m_entriesNames[i] == name) return &m_entries[i];
	}

	return nullptr;
}

//void SYMB::LinkSECT(SECT* pSect)
//{
//	assert(!isLinked && "SYMB can't be linked twice");
//
//	unsigned short fileCount = pSect->GetFileCount();
//
//	for (unsigned short i = 0; i < fileCount; i++)
//	{
//		SECTFile* file = pSect->GetFile(i);
//		unsigned short hdrx = file->m_hdrx;
//		size_t count = GetCount();
//
//		for (size_t k = 0; k < count; k++)
//		{
//			if (m_entries[k].hdrx == hdrx)
//			{
//				size_t dataStart = (size_t)file->GetBuffer();
//				m_entries[k].dataOffset += dataStart;
//			}
//		}
//	}
//
//	isLinked = true;
//}

//void SYMB::UnlinkSECT(SECT* pSect)
//{
//	assert(isLinked && "SYMB can't be unlinked twice");
//
//	unsigned short fileCount = pSect->GetFileCount();
//
//	for (unsigned short i = 0; i < fileCount; i++)
//	{
//		SECTFile* file = pSect->GetFile(i);
//		unsigned short hdrx = file->m_hdrx;
//		size_t count = GetCount();
//
//		for (size_t k = 0; k < count; k++)
//		{
//			if (m_entries[k].hdrx == hdrx)
//			{
//				size_t dataStart = (size_t)file->GetBuffer();
//				m_entries[k].dataOffset -= dataStart;
//			}
//		}
//	}
//
//	isLinked = false;
//}

void SYMB::Add(unsigned short hdrx, std::string name, void* ptr)
{
	m_entriesNames.push_back(name);

	SYMBEntry symb{ hdrx, 0, CreateNameIDHash(name), (char*)ptr};
	m_entries.push_back(symb);
}

void SYMB::Write(FILE* pFile)
{
	assert(!isLinked && "Unlink SYMB before generating TRB");
	TRBTag::Write(pFile);

	int count = m_entries.size();
	fwrite(&count, sizeof(count), 1, pFile);
	fwrite(m_entries.data(), sizeof(SYMBEntry) * count, 1, pFile);
	
	// writing names
	for (size_t i = 0; i < m_entries.size(); i++)
	{
		fwrite(m_entriesNames[i].data(), m_entriesNames[i].length() + 1, 1, pFile);
	}
}

void SYMB::Calculate(TSFL* tsfl)
{
	assert(!isLinked && "Unlink SYMB before calculating it");
	size = sizeof(size_t) + sizeof(SYMBEntry) * m_entries.size();
	size_t nameOffset = 0;

	for (size_t i = 0; i < m_entries.size(); i++)
	{
		m_entries[i].nameOffset = nameOffset;

		size_t nameLen = m_entriesNames[i].length() + 1;
		size += nameLen;
		nameOffset += nameLen;
	}
}
