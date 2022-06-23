#pragma once
#include "Tag.h"
#include <vector>

class RELC;
struct RELCEntry;
class HDRX;

enum class BTECVersion : unsigned int
{
	NOXOR = 0x10002,
	XOR = 0x10003
};

struct BTEC
{
	char magic[4];
	BTECVersion version;
	int csize;
	int usize;
};

class SECTFile
{
public:
	SECTFile(unsigned short hdrx);
	~SECTFile();

	unsigned short m_hdrx;

	/* Returns buffer of the SECT data */
	char* GetBuffer();

	/* Returns buffer size */
	size_t GetBufferSize();

	/* Allocating more memory */
	void* AllocMem(size_t size);

	/* Returns true if linked to RELC */
	bool IsLinked() const;

	/* Check if the pointer is in SECT bounds */
	bool IsPtrInBounds(void* ptr);

private:
	bool m_isLinked;
	char* m_buffer;
	size_t m_bufferSize;

	/* Makes file memory pointer to be program relative */
	void LinkRELCEntry(RELCEntry entry);

	/* Makes program memory pointer to be file relative */
	void UnlinkRELCEntry(RELCEntry entry);

	friend class SECT;
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

	/* Write to TRB */
	virtual void Write(FILE* pFile);

	/* Calculate data for writing */
	virtual void Calculate(TSFL* tsfl);

	/* Returns true if all SECT files are linked to RELC */
	bool IsLinked() const;

	/* Update pointers to be relative to the program memory */
	void LinkRELC(RELC* pRelc);

	/* Update pointers to be relative to the file memory */
	void UnlinkRELC(RELC* pRelc);

	/* Creates file and returns it's HDRX's id */
	SECTFile* CreateFile();

	/* Get file by it's hdrx */
	SECTFile* GetFile(unsigned short index);

	/* Get count of files */
	unsigned short GetFileCount();

	/* Aligns SECTFiles data to 4 bytes */
	void AlignData();

private:
	std::vector<SECTFile*> m_files;
};

