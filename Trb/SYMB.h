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
	SYMB(FILE* pFile);
	~SYMB();

	/* Get count of SYMB entries */
	size_t GetCount() const;

	/* Get entry by index */
	const SYMBEntry* GetEntry(size_t index) const;
	
	/* Get name of entry by index */
	const std::string GetEntryName(size_t index) const;

	/* Find entry's data pointer by name. Returns 0 if not found */
	template<typename T>
	T* FindEntryByName(std::string name) const;
	
	/* Update pointers to be relative to the program memory */
	void LinkSECT(SECT* pSect);

private:
	size_t m_count;
	SYMBEntry* m_entries;
	std::string* m_entriesNames;
};

template<typename T>
T* SYMB::FindEntryByName(std::string target_name) const
{
	for (size_t i = 0; i < m_count; i++)
	{
		if (m_entriesNames[i] == target_name) return (T*)m_entries[i].dataOffset;
	}

	return nullptr;
}
