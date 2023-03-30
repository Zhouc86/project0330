#include "iobuffer.h"
#include "pool.h"
CIOBuffer::CIOBuffer()
{
	Length = (0);
	NextBuf = (nullptr);
	PrevBuf = (nullptr);
}
CIOBuffer::~CIOBuffer()
{
}
CIOBuffer* CIOBuffer::Alloc()
{
	return Alloc_Pool<CIOBuffer>::GetInstance().Alloc();
}

void  CIOBuffer::OnFree()
{

	Alloc_Pool<CIOBuffer>::GetInstance().Free(this);

}
CRef::CRef() :m_lRef(1)
{

}
void CRef::AddRef()
{
	m_lRef++;
}
long CRef::Release()
{
	long result = --m_lRef;
	if (result == 0)
		OnFree();
	return result;
}