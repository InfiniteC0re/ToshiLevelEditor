#pragma once
#include "Tag.h"
#include <vector>

struct RELC;

struct BTEC2
{
	char magic[4];
	char version[4];
	int csize;
	int usize;
};

struct BTEC3 : BTEC2
{
	int xorValue;
};

class SECT : public TRBTag
{
public:
	SECT();
	SECT(FILE* pFile);
	~SECT();

	/* Decompresses SECC data*/
	void Decompress(BTEC2* btec, unsigned char* compressedData, std::vector<unsigned char>& decompressedData);

	void Decompress(BTEC3* btec, unsigned char* compressedData, std::vector<unsigned char>& decompressedData);

	/* Helper Function for Decompressing SECC data*/
	int Get_Size_Info(int file_pos, int& read_dst, int& size, int& offset, std::vector<unsigned char> compressedData);


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

