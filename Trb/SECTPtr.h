#pragma once
#include "TSFL.h"

template<typename T>
class SECTPtr
{
public:
	SECTPtr(TSFL* tsfl, unsigned short hdrx, size_t unlinkedOffset);
	inline T* data();
	size_t hdrx();
	size_t offset();
	bool valid();

private:
	TSFL* m_tsfl;
	unsigned short m_hdrx;
	size_t m_unlinkedOffset;
};

template<typename T>
inline SECTPtr<T>::SECTPtr(TSFL* tsfl, unsigned short hdrx, size_t unlinkedOffset) : m_tsfl(tsfl), m_hdrx(hdrx), m_unlinkedOffset(unlinkedOffset) { }

template<typename T>
inline T* SECTPtr<T>::data()
{
	return (T*)(m_tsfl->GetSECT()->GetFile(m_hdrx)->GetBuffer() + m_unlinkedOffset);
}

template<typename T>
inline size_t SECTPtr<T>::hdrx()
{
	return m_hdrx;
}

template<typename T>
inline size_t SECTPtr<T>::offset()
{
	return m_unlinkedOffset;
}

template<typename T>
inline bool SECTPtr<T>::valid()
{
	return m_tsfl != nullptr;
}
