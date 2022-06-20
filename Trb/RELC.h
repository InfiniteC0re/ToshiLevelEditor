#pragma once
#include "Tag.h"

struct RELCEntry
{
	short hdrx1;
	short hdrx2;
	unsigned int offset;
};

static_assert(sizeof(RELCEntry) == 8, "invalid size of RELCEntry");

class RELC : public TRBTag
{
public:
	RELC(FILE* pFile);
	~RELC();

	/* Get count of RELC entries */
	size_t GetCount() const;

	/* Get entry by index */
	RELCEntry GetEntry(size_t index) const;

	/* Get array of entries */
	const RELCEntry* GetEntries() const;

private:
	size_t m_count;
	RELCEntry* m_entries;
};

