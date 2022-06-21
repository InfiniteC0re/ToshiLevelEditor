#include "SECT.h"

#include "RELC.h"
#include "Utils.h"
#include <iostream>

SECT::SECT() : TRBTag("SECT", 0)
{
	size = 0;
	m_bufferSize = 0;
	isLinked = false;
}

SECT::SECT(FILE* pFile) : TRBTag(pFile)
{
	m_bufferSize = 0;

	// reading SECT data
	AllocMem(size);
	ReadFileData(GetBuffer(), 1, size, pFile);

	// BTEC
	assert(*(int*)GetBuffer() != MAKEFOURCC('B', 'T', 'E', 'C') && "Not implemented yet");
}

SECT::~SECT()
{
	delete[] m_buffer;
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

	if (oldSize > 0)
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
