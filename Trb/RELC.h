#pragma once
#include "Tag.h"
#include <vector>

struct RELCEntry
{
	short hdrx1;
	short hdrx2;
	size_t offset;
};

static_assert(sizeof(RELCEntry) == 8, "invalid size of RELCEntry");

class RELC : public TRBTag
{
public:
	RELC();
	RELC(FILE* pFile);
	~RELC();

	/* Get count of RELC entries */
	size_t GetCount() const;

	/* Get entry by index */
	RELCEntry GetEntry(size_t index) const;

	/* Get array of entries */
	const RELCEntry* GetEntries() const;

	/* Add new entry. ptr should be relative to the file memory */
	void Add(short hdrx1, short hdrx2, void* ptr);

private:
	std::vector<RELCEntry> m_entries;
};

