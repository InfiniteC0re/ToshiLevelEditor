#include "RELC.h"
#include <cassert>
#include "Utils.h"

RELC::RELC(FILE* pFile) : TRBTag(pFile)
{
	int count;
	ReadFileData(&count, sizeof(size_t), 1, pFile);
	assert(count >= 0 && "Count of RELC entries cannot be negative");
	m_entries.resize(count);

	ReadFileData(m_entries.data(), sizeof(RELCEntry), count, pFile);
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
