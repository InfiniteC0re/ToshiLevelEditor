#pragma once
#include "TSFL.h"

template<typename T>
class SECTPtr
{
public:
	SECTPtr(TSFL* tsfl, size_t unlinkedOffset);
	inline T* data();
	size_t offset();

private:
	TSFL* m_tsfl;
	size_t m_unlinkedOffset;
};

template<typename T>
inline SECTPtr<T>::SECTPtr(TSFL* tsfl, size_t unlinkedOffset) : m_tsfl(tsfl), m_unlinkedOffset(unlinkedOffset) { }

template<typename T>
inline T* SECTPtr<T>::data()
{
	return (T*)(m_tsfl->GetSECT()->GetBuffer() + m_unlinkedOffset);
}

template<typename T>
inline size_t SECTPtr<T>::offset()
{
	return m_unlinkedOffset;
}
