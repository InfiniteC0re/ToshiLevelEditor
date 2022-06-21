#pragma once
#include "Tag.h"
#include <string>

#pragma pack(push, 1)
struct SYMBEntry
{
	short hdrx;
	size_t nameOffset;
	short nameID;
	char* dataOffset;
};
#pragma pack(pop)

static_assert(sizeof(SYMBEntry) == 12, "invalid size of SYMBEntry");

class SECT;

class SYMB : public TRBTag
{
public:
	SYMB();
	SYMB(FILE* pFile);
	~SYMB();

	/* Get count of SYMB entries */
	size_t GetCount() const;

	/* Returns true if SYMB is linked to SECT */
	bool IsLinked() const;

	/* Get entry by index */
	const SYMBEntry* GetEntry(size_t index) const;
	
	/* Get name of entry by index */
	const std::string GetEntryName(size_t index) const;

	/* Find Symbol in the list */
	void* Find(std::string name) const;
	
	/* Update pointers to be relative to the program memory */
	void LinkSECT(SECT* pSect);

	/* Update pointers to be relative to the file memory */
	void UnlinkSECT(SECT* pSect);

private:
	bool isLinked;
	size_t m_count;
	SYMBEntry* m_entries;
	std::string* m_entriesNames;
};
