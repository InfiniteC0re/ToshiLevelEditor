#include "SECT.h"

#include "RELC.h"
#include "HDRX.h"
#include "Utils.h"
#include <iostream>

SECT::SECT() : TRBTag("SECT", 0)
{
	size = 0;
	CreateFile();
}

SECT::SECT(FILE* pFile, HDRX* hdrx) : TRBTag(pFile)
{
	if (*(int*)name == MAKEFOURCC('S', 'E', 'C', 'C'))
	{
		for (int i = 0; i < hdrx->GetFileCount(); i++)
		{
			SECTFile* file = CreateFile();

			BTEC btec;
			ReadFileData(&btec, sizeof(BTEC), 1, pFile);

			assert((btec.version == BTECVersion::NOXOR || btec.version == BTECVersion::XOR) && "Unknown BTEC version");

			std::vector<unsigned char> decompressed;
			std::vector<unsigned char> compressed(btec.csize);

			if (btec.version == BTECVersion::NOXOR)
			{
				ReadFileData(compressed.data(), btec.csize, 1, pFile);
				Decompress(&btec, compressed, decompressed);
			}
			else if (btec.version == BTECVersion::XOR)
			{
				int xorValue;
				
				ReadFileData(&xorValue, sizeof(int), 1, pFile);
				ReadFileData(compressed.data(), btec.csize, 1, pFile);
				Decompress(&btec, xorValue, compressed, decompressed);
			}

			// copying decompressed data to the SECT buffer
			file->AllocMem(btec.usize);
			memcpy(file->GetBuffer(), decompressed.data(), btec.usize);
		}

		ValidateAlignment(pFile);
	}
	else
	{
		for (unsigned short i = 0; i < hdrx->GetFileCount(); i++)
		{
			HDRXFile* hdrxFile = hdrx->GetFile(i);
			SECTFile* file = CreateFile();

			file->AllocMem(hdrxFile->m_size);
			ReadFileData(file->GetBuffer(), 1, hdrxFile->m_size, pFile);
		}
	}
}

SECT::~SECT()
{
	for (SECTFile* file : m_files)
	{
		delete file;
	}

	m_files.clear();
}

void SECT::Decompress(BTEC* btec, std::vector<unsigned char>& compressedData, std::vector<unsigned char>& decompressedData)
{
	int read_dst, size, offset = 0;
	int read_count = 0;
	int remaining = btec->csize;
	int dst_pos = 0;
	int file_pos = 0;

	while (remaining > 0)
	{
		read_count = Get_Size_Info(file_pos, read_dst, size, offset, compressedData);
		remaining -= read_count;
		file_pos += read_count;

		if (read_dst)
		{
			if (size > 0)
			{
				if (size < offset)
				{
					append_bytes(decompressedData, decompressedData, decompressedData.size() - offset, size);
				}
				else
				{
					dst_pos = decompressedData.size() - offset;
					for (size_t i = 0; i < size; i++)
					{
						decompressedData.push_back(decompressedData[dst_pos + i]);
					}
				}
			}
		}
		else
		{
			append_bytes(decompressedData, compressedData, file_pos, size);

			remaining -= size;
			file_pos += size;
		}
	}
}

void SECT::Decompress(BTEC* btec, int xorValue, std::vector<unsigned char>& compressedData, std::vector<unsigned char>& decompressedData)
{
	SECT::Decompress(btec, compressedData, decompressedData);
	xor_data(decompressedData, xorValue);
}

int SECT::Get_Size_Info(int file_pos, int& read_dst, int& size, int& offset, std::vector<unsigned char>& compressedData)
{
	int read_count = 0;

	size = compressedData[file_pos + read_count++];
	read_dst = (size & 0x80) == 0;

	if ((size & 0x40) == 0)
	{
		size &= 0x3F;
	}
	else
	{
		size = ((size & 0x3F) << 0x08) + compressedData[file_pos + read_count++];
	}

	size++;
	offset = 0;

	if (read_dst)
	{
		offset = compressedData[file_pos + read_count++];

		if ((offset & 0x80) == 0)
		{
			offset &= 0x7F;
		}
		else
		{
			offset = ((offset & 0x7F) << 0x08) + compressedData[file_pos + read_count++];
		}
		offset++;
	}
	return read_count;
}

bool SECT::IsLinked() const
{
	bool isLinked = true;

	for (SECTFile* file : m_files)
	{
		isLinked = isLinked && file->IsLinked();
	}

	return isLinked;
}

void SECT::LinkRELC(RELC* pRelc)
{
	assert(!IsLinked() && "SECT can't be linked twice");

	size_t fileCount = GetFileCount();
	size_t relcCount = pRelc->GetCount();

	for (size_t i = 0; i < fileCount; i++)
	{
		SECTFile* file = GetFile(i);

		for (size_t k = 0; k < relcCount; k++)
		{
			RELCEntry entry = pRelc->GetEntry(k);

			if (file->m_hdrx == entry.hdrx1)
			{
				file->LinkRELCEntry(entry);
			}
		}

		file->m_isLinked = true;
	}
}

void SECT::UnlinkRELC(RELC* pRelc)
{
	assert(IsLinked() && "SECT isn't linked");

	size_t fileCount = GetFileCount();
	size_t relcCount = pRelc->GetCount();

	for (size_t i = 0; i < fileCount; i++)
	{
		SECTFile* file = GetFile(i);

		for (size_t k = 0; k < relcCount; k++)
		{
			RELCEntry entry = pRelc->GetEntry(k);

			if (file->m_hdrx == entry.hdrx1)
			{
				file->UnlinkRELCEntry(entry);
			}
		}

		file->m_isLinked = false;
	}
}

SECTFile* SECT::CreateFile()
{
	SECTFile* file = new SECTFile(m_files.size());
	m_files.push_back(file);

	return file;
}

SECTFile* SECT::GetFile(unsigned short index)
{
	return m_files[index];
}

unsigned short SECT::GetFileCount()
{
	return m_files.size();
}

void SECT::Write(FILE* pFile)
{
	assert(!IsLinked() && "Unlink SECT before generating TRB");

	TRBTag::Write(pFile);

	for (SECTFile* file : m_files)
	{
		fwrite(file->GetBuffer(), file->GetBufferSize(), 1, pFile);
	}
}

void SECT::AlignData()
{
	for (SECTFile* file : m_files)
	{
		// aligning data to 4 bytes
		size_t alignedSize = file->GetBufferSize();
		size_t toAdd = 4 - (alignedSize % 4);
		alignedSize += toAdd == 4 ? 0 : toAdd;
		file->AllocMem(toAdd);
	}
}

void SECT::Calculate(TSFL* tsfl)
{
	assert(!IsLinked() && "Unlink SECT before calculating it");

	size = 0;
	for (SECTFile* file : m_files)
	{
		size += file->GetBufferSize();
	}
}

SECTFile::SECTFile(unsigned short hdrx)
{
	m_buffer = 0;
	m_bufferSize = 0;
	m_hdrx = hdrx;
	m_isLinked = false;
}

SECTFile::~SECTFile()
{
	if (m_buffer)
	{
		delete[] m_buffer;
	}
}

void* SECTFile::AllocMem(size_t size)
{
	assert(!m_isLinked && "Unlink before allocating memory");
	size_t oldSize = m_bufferSize;
	size_t newSize = m_bufferSize + size;
	assert(newSize > oldSize && "Invalid allocation size");

	char* newBuffer = new char[newSize]();

	if (m_buffer != nullptr && oldSize > 0)
	{
		// copying old data to the new buffer
		memcpy(newBuffer, m_buffer, oldSize);

		// deleting the old buffer
		delete[] m_buffer;
	}

	size = newSize;
	m_bufferSize = newSize;
	m_buffer = newBuffer;

	return m_buffer + oldSize;
}

bool SECTFile::IsLinked() const
{
	return m_isLinked;
}

char* SECTFile::GetBuffer()
{
	return m_buffer;
}

size_t SECTFile::GetBufferSize()
{
	return m_bufferSize;
}

bool SECTFile::IsPtrInBounds(void* ptr)
{
	return (ptr >= m_buffer && ptr < m_buffer + m_bufferSize);
}

void SECTFile::LinkRELCEntry(RELCEntry entry)
{
	char* buffer = GetBuffer();

	// updating file relative pointer to be memory relative
	assert(IsPtrInBounds(buffer + entry.offset) && "Invalid RELC pointer - not in SECTFile bounds");
	*(int*)(buffer + entry.offset) += (unsigned int)buffer;
}

void SECTFile::UnlinkRELCEntry(RELCEntry entry)
{
	char* buffer = GetBuffer();

	// updating memory relative pointer to be file relative
	assert(entry.offset < m_bufferSize && "the pointer is not file relative");
	*(int*)(buffer + entry.offset) -= (unsigned int)buffer;
}
