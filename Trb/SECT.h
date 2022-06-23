#pragma once
#include "Tag.h"
#include <vector>

class RELC;
class HDRX;

struct BTEC
{
	char magic[4];
	unsigned int version;
	int csize;
	int usize;
};

class SECT : public TRBTag
{
public:
	SECT();
	SECT(FILE* pFile, HDRX* hdrx);
	~SECT();

	/* Decompresses SECC data (no XOR) */
	void Decompress(BTEC* btec, std::vector<unsigned char>& compressedData, std::vector<unsigned char>& decompressedData);

	/* Decompresses SECC data (XOR) */
	void Decompress(BTEC* btec, int xorValue, std::vector<unsigned char>& compressedData, std::vector<unsigned char>& decompressedData);

	/* Helper Function for Decompressing SECC data */
	int Get_Size_Info(int file_pos, int& read_dst, int& size, int& offset, std::vector<unsigned char>& compressedData);

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

	/* Write to TRB */
	virtual void Write(FILE* pFile);

	/* Calculate data for writing */
	virtual void Calculate(TSFL* tsfl);

private:
	bool isLinked;
	char* m_buffer;
	size_t m_bufferSize;
};

