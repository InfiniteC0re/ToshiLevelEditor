#pragma once
#include "Tag.h"

struct RELC;

class SECT : public TRBTag
{
public:
	SECT();
	SECT(FILE* pFile);
	~SECT();

	/* Returns buffer of the SECT data */
	char* GetBuffer();

	/* Returns buffer size */
	size_t GetBufferSize();

	/* Returns true if SECT is linked to RELC */
	bool IsLinked() const;

	/* Update pointers to be relative to the program memory */
	void LinkRELC(RELC* pRelc);

	/* Update pointers to be relative to the file memory */
	void UnlinkRELC(RELC* pRelc);

	/* Allocating more memory */
	void* AllocMem(size_t size);

	/* Check if the pointer is in SECT bounds */
	bool IsPtrInBounds(void* ptr);

private:
	bool isLinked;
	char* m_buffer;
	size_t m_bufferSize;
};

