#include "RELC.h"
#include <cassert>
#include "Utils.h"

RELC::RELC(FILE* pFile) : TRBTag(pFile)
{
	ReadFileData(&m_count, sizeof(size_t), 1, pFile);
	assert(m_count >= 0 && "Count of RELC entries cannot be negative");
	m_entries = new RELCEntry[m_count];
	ReadFileData(m_entries, sizeof(RELCEntry), m_count, pFile);
}

RELC::~RELC()
{
	if (m_entries != nullptr)
	{
		delete[] m_entries;
	}
}

size_t RELC::GetCount() const
{
	return m_count;
}

RELCEntry RELC::GetEntry(size_t index) const
{
	if (m_count > index)
	{
		return m_entries[index];
	}

	return { };
}

const RELCEntry* RELC::GetEntries() const
{
	return m_entries;
}
