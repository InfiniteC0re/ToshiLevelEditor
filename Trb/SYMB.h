#pragma once
#include "Tag.h"
#include <string>
#include <vector>

#pragma pack(push, 1)
struct SYMBEntry
{
	unsigned short hdrx;
	size_t nameOffset;
	unsigned short nameID;
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
	inline size_t GetCount() const;

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

	/* Create new symbol */
	void Add(unsigned short hdrx, std::string name, short nameID, void* ptr);

private:
	bool isLinked;
	std::vector<SYMBEntry> m_entries;
	std::vector<std::string> m_entriesNames;
};
