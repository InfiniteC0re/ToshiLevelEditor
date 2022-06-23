#include "SECT.h"

#include "RELC.h"
#include "Utils.h"
#include <iostream>

SECT::SECT() : TRBTag("SECT", 0)
{
	size = 0;
	m_buffer = nullptr;
	m_bufferSize = 0;
	isLinked = false;
}

SECT::SECT(FILE* pFile) : TRBTag(pFile)
{
	m_bufferSize = 0;

	int btec;
	ReadFileData(&btec, 4, 1, pFile);
	if (btec == MAKEFOURCC('B', 'T', 'E', 'C'))
	{
		std::vector<unsigned char> decompressedData;
		int version;
		do
		{
			std::vector<unsigned char> currentDecompressedData;
			ReadFileData(&version, 4, 1, pFile);
			fseek(pFile, -8, SEEK_CUR);
			assert((version == 0x10002 || version == 0x10003) && "Unknown BTEC version");
			if (version == 0x10002)
			{
				BTEC2 btec2;
				ReadFileData(&btec2, sizeof(BTEC2), 1, pFile);
				unsigned char* compressedData = (unsigned char*)malloc(btec2.csize);
				ReadFileData(compressedData, 1, btec2.csize, pFile);
				Decompress(&btec2, compressedData, currentDecompressedData);
				free(compressedData);
			}
			else
			{
				BTEC3 btec3;
				ReadFileData(&btec3, sizeof(BTEC3), 1, pFile);
				unsigned char* compressedData = (unsigned char*)malloc(btec3.csize);
				ReadFileData(compressedData, 1, btec3.csize, pFile);
				Decompress(&btec3, compressedData, currentDecompressedData);
				free(compressedData);
			}
			int btec;
			ReadFileData(&btec, 4, 1, pFile);
			if (btec != MAKEFOURCC('B', 'T', 'E', 'C')) break; // AdventureT: It only works this way, if i put it in the while it just keeps reading for whatever reason...
			decompressedData.insert(decompressedData.end(), currentDecompressedData.data(), currentDecompressedData.data() + std::size(currentDecompressedData));
		} while (true);

		AllocMem(decompressedData.size());
		memcpy(m_buffer, decompressedData.data(), decompressedData.size());
		fseek(pFile, 4 - (ftell(pFile) % 4) - 4, SEEK_CUR); //AdventureT: Skip Padding to next section

	}
	else
	{
		AllocMem(size);
		ReadFileData(GetBuffer(), 1, size, pFile);
	}
}

SECT::~SECT()
{
	delete[] m_buffer;
}

void SECT::Decompress(BTEC2* btec, unsigned char* compressedData, std::vector<unsigned char>& decompressedData)
{
	int read_dst, size, offset = 0;
	int read_count = 0;
	int remaining = btec->csize;
	std::vector<unsigned char> cmpData(compressedData, compressedData + btec->csize);
	int dst_pos = 0;
	int file_pos = 0;

	while (remaining > 0)
	{
		read_count = Get_Size_Info(file_pos, read_dst, size, offset, cmpData);
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
			append_bytes(decompressedData, cmpData, file_pos, size);

			remaining -= size;
			file_pos += size;
		}
	}
}

void SECT::Decompress(BTEC3* btec, unsigned char* compressedData, std::vector<unsigned char>& decompressedData)
{
	SECT::Decompress((BTEC2*)btec, compressedData, decompressedData);
	xor_data(decompressedData, btec->xorValue);
}

int SECT::Get_Size_Info(int file_pos, int& read_dst, int& size, int& offset, std::vector<unsigned char> compressedData)
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

char* SECT::GetBuffer()
{
	return m_buffer;
}

size_t SECT::GetBufferSize()
{
	return m_bufferSize;
}

bool SECT::IsLinked() const
{
	return isLinked;
}

void SECT::LinkRELC(RELC* pRelc)
{
	// todo: support files with few HDRXs
	assert(!isLinked && "SECT can't be linked twice");

	char* buffer = GetBuffer();
	size_t count = pRelc->GetCount();

	for (size_t i = 0; i < count; i++)
	{
		RELCEntry entry = pRelc->GetEntry(i);
		if (entry.hdrx1 != 0 || entry.hdrx2 != 0) break;

		// updating file relative pointers to be memory relative pointers
		assert(IsPtrInBounds(buffer + entry.offset) && "Invalid RELC pointer - not in SECT bounds");
		*(int*)(buffer + entry.offset) += (unsigned int)buffer;
	}

	isLinked = true;
}

void SECT::UnlinkRELC(RELC* pRelc)
{
	// todo: support files with few HDRXs
	assert(isLinked && "SECT isn't linked");

	char* buffer = GetBuffer();
	size_t count = pRelc->GetCount();

	for (size_t i = 0; i < count; i++)
	{
		RELCEntry entry = pRelc->GetEntry(i);
		if (entry.hdrx1 != 0 || entry.hdrx2 != 0) break;

		// updating memory relative pointers to be file relative pointers
		assert(entry.offset < m_bufferSize && "the pointer is not file relative");
		*(int*)(buffer + entry.offset) -= (unsigned int)buffer;
	}

	isLinked = false;
}

void* SECT::AllocMem(size_t size)
{
	assert(!isLinked && "Unlink before allocating memory");
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

bool SECT::IsPtrInBounds(void* ptr)
{
	return (ptr >= m_buffer && ptr < m_buffer + m_bufferSize);
}

void SECT::Write(FILE* pFile)
{
	assert(!isLinked && "Unlink SECT before generating TRB");

	TRBTag::Write(pFile);
	fwrite(GetBuffer(), GetBufferSize(), 1, pFile);
}

void SECT::Calculate(TSFL* tsfl)
{
	assert(!isLinked && "Unlink SECT before calculating it");
	size = GetBufferSize();
}
