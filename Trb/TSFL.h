#pragma once
#include "Tag.h"
#include "HDRX.h"
#include "SECT.h"
#include "RELC.h"
#include "SYMB.h"
#include "SECTPtr.h"

#include <cassert>
#include <iostream>

class TSFL : public TRBTag
{
public:
	TSFL();
	TSFL(FILE* pFile);
	~TSFL();

	HDRX* GetHDRX();
	SECT* GetSECT();
	RELC* GetRELC();
	SYMB* GetSYMB();

	/* Update pointers of SECT and SYMB to be relative to the program memory */
	void Link();

	/* Update pointers of SECT and SYMB to be relative to the file memory */
	void Unlink();

	/* SECT */

	/* Dump data in SECT to file */
	void* DumpSECT(const char* filepath);

	/* Allocate memory in SECT */
	template<typename T>
	SECTPtr<T> AllocateSECT(int count = 1);

	/* Allocate memory in SECT and write a pointer using SECTPtr */
	template<typename T>
	SECTPtr<T> AllocateSECT(SECTPtr<T*> out, int count = 1);

	/* Allocate memory in SECT and write a pointer */
	template<typename T>
	SECTPtr<T> AllocateSECT(void* out, int count = 1);

	/* SYMB */

	/* Find SYMB Data pointer by name. Returns 0 if not found */
	template<typename T>
	inline SECTPtr<T> FindSymbol(std::string name);

	/* Add symbol */
	void AddSymbol(unsigned short hdrx, std::string name, void* ptr);

	/* Write to TRB */
	virtual void Write(FILE* pFile);

	/* Calculate data for writing */
	virtual void Calculate(TSFL* tsfl);

private:
	char m_trbf[4];
	unsigned short m_activeHdrx;

	HDRX* m_HDRX;
	SECT* m_SECT;
	RELC* m_RELC;
	SYMB* m_SYMB;
};

template<typename T>
inline SECTPtr<T> TSFL::FindSymbol(std::string name)
{
	const SYMBEntry* symb = m_SYMB->Find(name);
	
	if (symb)
	{
		SECTPtr<T> ptr(this, symb->hdrx, (size_t)symb->dataOffset);
		return ptr;
	}

	SECTPtr<T> nullPtr(nullptr, 0, 0);
	return nullPtr;
}

template<typename T>
SECTPtr<T> TSFL::AllocateSECT(int count)
{
	// unlink everything before moving the data
	Unlink();

	// get currently selected SECT file
	SECTFile* file = m_SECT->GetFile(m_activeHdrx);
	
	size_t size = sizeof(T) * count;
	T* data = (T*)file->AllocMem(size);

	size_t unlinkedOffset = (size_t)data - (size_t)file->GetBuffer();
	SECTPtr<T> ptr(this, m_activeHdrx, unlinkedOffset);

	// link everything back
	Link();

	return ptr;
}

template<typename T>
SECTPtr<T> TSFL::AllocateSECT(SECTPtr<T*> out, int count)
{
	// get currently selected SECT file
	SECTFile* file = m_SECT->GetFile(m_activeHdrx);

	assert(file->IsPtrInBounds(out.data()) && "The pointer isn't in bounds of SECT");
	assert(out.hdrx() == m_activeHdrx && "Selected HDRX is wrong");
	
	// allocating the memory
	SECTPtr<T> pPtr = AllocateSECT<T>(count);

	// saving the pointer to out
	*out.data() = pPtr.data();

	// saving the file relative pointer in RELC
	m_RELC->Add(out.hdrx(), out.hdrx(), (void*)out.offset());

	return pPtr;
}

template<typename T>
inline SECTPtr<T> TSFL::AllocateSECT(void* out, int count)
{
	// get currently selected SECT file
	SECTFile* file = m_SECT->GetFile(m_activeHdrx);

	assert(out != nullptr && "The pointer can't be null");
	assert(file->IsPtrInBounds(out) && "The pointer isn't in bounds of SECT");

	// getting a file relative pointer
	size_t outUnlinked = (size_t)out - (size_t)file->GetBuffer();

	// allocating the memory
	SECTPtr<T> pData = AllocateSECT<T>(count);

	// getting the pointer that will store a pointer to the allocated memory
	T** outLinked = (T**)(file->GetBuffer() + outUnlinked);

	// saving the pointer to allocated memory
	*outLinked = pData.data();

	// saving the file relative pointer in RELC
	m_RELC->Add(pData.hdrx(), pData.hdrx(), (void*)outUnlinked);

	return pData;
}
