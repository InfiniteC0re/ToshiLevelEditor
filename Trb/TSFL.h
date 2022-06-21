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
	inline T* FindSymbol(std::string name) const;

	/* Add symbol */
	void AddSymbol(unsigned short hdrx, std::string name, short nameID, void* ptr);

	/* Write to TRB */
	virtual void Write(FILE* pFile);

	/* Calculate data for writing */
	virtual void Calculate(TSFL* tsfl);

private:
	char m_trbf[4];

	HDRX* m_HDRX;
	SECT* m_SECT;
	RELC* m_RELC;
	SYMB* m_SYMB;
};

template<typename T>
inline T* TSFL::FindSymbol(std::string name) const
{
	return (T*)m_SYMB->Find(name);
}

template<typename T>
SECTPtr<T> TSFL::AllocateSECT(int count)
{
	// unlink everything before moving the data
	Unlink();

	size_t size = sizeof(T) * count;
	T* data = (T*)m_SECT->AllocMem(size);

	size_t unlinkedOffset = (size_t)data - (size_t)m_SECT->GetBuffer();
	SECTPtr<T> ptr(this, unlinkedOffset);

	// link everything back
	Link();

	return ptr;
}

template<typename T>
SECTPtr<T> TSFL::AllocateSECT(SECTPtr<T*> out, int count)
{
	assert(m_SECT->IsPtrInBounds(out.data()) && "The pointer isn't in bounds of SECT");
	
	// allocating the memory
	SECTPtr<T> pPtr = AllocateSECT<T>(count);

	// saving the pointer to out
	*out.data() = pPtr.data();

	// saving the file relative pointer in RELC
	// todo: hdrx
	m_RELC->Add(0, 0, (void*)out.offset());

	return pPtr;
}

template<typename T>
inline SECTPtr<T> TSFL::AllocateSECT(void* out, int count)
{
	assert(out != nullptr && "The pointer can't be null");
	assert(m_SECT->IsPtrInBounds(out) && "The pointer isn't in bounds of SECT");

	// getting a file relative pointer
	size_t outUnlinked = (size_t)out - (size_t)m_SECT->GetBuffer();

	// allocating the memory
	SECTPtr<T> pData = AllocateSECT<T>(count);

	// getting the pointer that will store a pointer to the allocated memory
	T** outLinked = (T**)(m_SECT->GetBuffer() + outUnlinked);

	// saving the pointer to allocated memory
	*outLinked = pData.data();

	// saving the file relative pointer in RELC
	// todo: hdrx
	m_RELC->Add(0, 0, (void*)outUnlinked);

	return pData;
}
