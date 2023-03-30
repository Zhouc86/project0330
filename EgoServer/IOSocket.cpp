#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#include <assert.h>
#include <iostream>
#include <mutex>
#include "IOSocket.h"
#include "../common/Pool.h"
#include "../common/IOBuffer.h"


bool CIOSocket::Init(HANDLE hCom)
{
	ZeroMemory(&this->m_OverlappedRead, sizeof(m_OverlappedRead));
	ZeroMemory(&m_OverlappedWrite, sizeof(m_OverlappedWrite));
	m_pFirstHead = m_pLastHead = NULL;
	m_dwBufferCount = 0;
	m_IOCount = 0;
	ZeroMemory(m_WriteBuff, sizeof(m_WriteBuff));
	if (!CreateIoCompletionPort((HANDLE)m_hSocket, hCom, (ULONG_PTR)this, 0))
	{
		assert(false);
		std::cout << "failed" << std::endl;
		return false;
	}
	OnCreate();
	return true;
}
CIOSocket::CIOSocket(HANDLE hCom, SOCKET s) :m_hSocket(s), m_lRef(1), m_hComplete(hCom), m_pFirstHead(nullptr), m_pLastHead(nullptr)
{
	m_pBuffer = CIOBuffer::Alloc();

}

void CIOSocket::Read(DWORD dwLeft)
{
	if (m_hSocket == INVALID_SOCKET) return;

	if (m_pBuffer->Length > dwLeft)
	{
		m_pBuffer->Length -= dwLeft;
		CIOBuffer* pNextBuffer = CIOBuffer::Alloc();
		int8_t* ptr = m_pBuffer->Buffer;
		ptr += m_pBuffer->Length;
		memcpy(pNextBuffer->Buffer, ptr, dwLeft);
		m_pBuffer->Release();
		m_pBuffer = pNextBuffer;
		pNextBuffer->Length = dwLeft;
	}
	AddRef();
	WSABUF buf;
	buf.len = CIOBuffer::IO_BUFFER_SIZE - m_pBuffer->Length;
	int8_t* ptr = m_pBuffer->Buffer;
	ptr += m_pBuffer->Length;
	buf.buf = (char*)ptr;
	DWORD dwFlag = 0;
	DWORD dwRecv;
	if (WSARecv(m_hSocket, &buf, 1, &dwRecv, &dwFlag, &this->m_OverlappedRead, NULL) != 0)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			std::cout << "WSARecg Error " << WSAGetLastError() << std::endl;
			Close();
			return;
		}
	}
}
void CIOSocket::Write(CIOBuffer* pBuffer)
{
	if (m_hSocket == INVALID_SOCKET) return;
	if (pBuffer->Length == 0) return;
	if (pBuffer->Length > CIOBuffer::IO_BUFFER_SIZE) return;
	pBuffer->AddRef();

	std::lock_guard<std::mutex> l(m_lock);
	if (m_dwBufferCount == 0)
	{
		AddRef();
		m_WriteBuff[m_dwBufferCount].buf = (char*)pBuffer->Buffer;
		m_WriteBuff[m_dwBufferCount].len = pBuffer->Length;
		m_dwWriteCount = pBuffer->Length;
		m_dwBufferCount = 1;
		DWORD dwSent = 0;
		if (WSASend(m_hSocket, m_WriteBuff, m_dwBufferCount, &dwSent, NULL, &m_OverlappedWrite, NULL) != 0)
		{
			DWORD error = WSAGetLastError();
			if (error != WSA_IO_PENDING)
			{
				m_dwWriteCount = 0;
				pBuffer->Release();
				m_dwBufferCount = 0;
				Release();
				PostQueuedCompletionStatus(m_hComplete, -1, (DWORD_PTR)this, NULL);

			}
		}
	}
	else
	{
		IO_STRUCT* ptr = Alloc_Pool<IO_STRUCT>::GetInstance().Alloc();
		ptr->pBuffer = pBuffer;
		if (m_pFirstHead == NULL)
		{
			m_pFirstHead = m_pLastHead = ptr;
		}
		else
		{
			m_pLastHead->pNext = ptr;
			m_pLastHead = ptr;
		}
		m_IOCount++;

	}

}
void CIOSocket::WriteCallback(DWORD dwTransferred)
{
	if (m_hSocket == INVALID_SOCKET) return;
	if (dwTransferred == 0 || dwTransferred == -1)
	{
		Close();
		return;
	}
	if (m_dwWriteCount != dwTransferred)
	{
		Close();
		return;
	}
	std::lock_guard<std::mutex> l(m_lock);
	for (DWORD i = 0; i < m_dwBufferCount; i++)
	{
		CIOBuffer* ptr = (CIOBuffer*)(m_WriteBuff[i].buf - offsetof(CIOBuffer, Buffer));
		ptr->Release();
	}
	ZeroMemory(&m_WriteBuff, sizeof(m_WriteBuff));
	m_dwBufferCount = m_dwWriteCount = 0;

	if (m_pFirstHead == NULL)
	{
		m_pLastHead = NULL;
		m_bSending = false;
	}
	else
	{
		while (m_pFirstHead != NULL && m_dwWriteCount < CIOBuffer::IO_BUFFER_SIZE)
		{
			m_WriteBuff[m_dwBufferCount].buf = (char*)m_pFirstHead->pBuffer->Buffer;
			m_WriteBuff[m_dwBufferCount].len = m_pFirstHead->pBuffer->Length;

			m_dwWriteCount += m_pFirstHead->pBuffer->Length;
			IO_STRUCT* temp = m_pFirstHead;
			m_pFirstHead = m_pFirstHead->pNext;
			Alloc_Pool<IO_STRUCT>::GetInstance().Free(temp);
			m_IOCount--;
			m_dwBufferCount++;
			if (m_dwBufferCount == _countof(m_WriteBuff))break;
		}

		DWORD dwSent;
		AddRef();
		if (WSASend(m_hSocket, m_WriteBuff, m_dwBufferCount, &dwSent, NULL, &m_OverlappedWrite, NULL) != 0)
		{
			DWORD error = WSAGetLastError();
			if (error != WSA_IO_PENDING)
			{
				for (DWORD i = 0; i < m_dwBufferCount; i++)
				{
					CIOBuffer* pBuffer = (CIOBuffer*)(m_WriteBuff[i].buf - offsetof(CIOBuffer, Buffer));
					pBuffer->Release();
				}
				PostQueuedCompletionStatus(m_hComplete, -1, (ULONG_PTR)this, NULL);
				Release();
			}
		}
	}

}
CIOSocket::~CIOSocket()
{
	for (DWORD i = 0; i < m_dwBufferCount; i++)
	{
		CIOBuffer* ptr = (CIOBuffer*)(m_WriteBuff[i].buf - offsetof(CIOBuffer, Buffer));
		ptr->Release();
	}
	m_dwBufferCount = 0;
	IO_STRUCT* temp = m_pFirstHead;
	while (temp)
	{
		IO_STRUCT* t = temp;
		temp = temp->pNext;
		t->pBuffer->Release();
		Alloc_Pool<IO_STRUCT>::GetInstance().Free(t);
		m_IOCount--;
	}
	if (m_IOCount != 0)
	{
		std::cout << "memory leack" << std::endl;
	}
	m_pBuffer->Release();

}
void CIOSocket::AddRef()
{
	InterlockedIncrement(&m_lRef);
}
void CIOSocket::Release()
{
	if (InterlockedDecrement(&m_lRef) == 0)
	{
		//Free();
	}
}
void CIOSocket::Close()
{
	SOCKET s = (SOCKET)InterlockedExchange((UINT64*)&m_hSocket, INVALID_SOCKET);
	if (s != INVALID_SOCKET)
	{

		linger lin = { TRUE, 0 };
		setsockopt(s, SOL_SOCKET, SO_LINGER, (char*)&lin, sizeof(lin));
		closesocket(s);
		for (DWORD i = 0; i < m_dwBufferCount; i++)
		{
			CIOBuffer* p = (CIOBuffer*)(m_WriteBuff[i].buf - offsetof(CIOBuffer, Buffer));
			p->Release();
		}
		m_dwBufferCount = 0;
		IO_STRUCT* temp = m_pFirstHead;
		while (temp != NULL)
		{
			//long c = temp->pBuffer->m_wSize;
			IO_STRUCT* t = temp;
			temp = temp->pNext;
			t->pBuffer->Release();
			Alloc_Pool<IO_STRUCT>::GetInstance().Free(t);
			m_IOCount--;
		}
		m_dwWriteCount = 0;
		m_pFirstHead = m_pLastHead = NULL;
		m_IOCount = 0;
		OnClose();
	}
}
unsigned int CIOSocket::IOThread(void* param)
{
	//srand((unsigned int)time(NULL));
	LPOVERLAPPED lpOverlapped;
	CIOSocket* pObject;
	DWORD dwTransferred;
	HANDLE hCom = (HANDLE)param;
	while (true)
	{
		BOOL bSucc = GetQueuedCompletionStatus(hCom, &dwTransferred, (PULONG_PTR)&pObject, &lpOverlapped, WSA_INFINITE);
		if (dwTransferred == -1 || dwTransferred == 0)
		{
			pObject->Close();
		}
		else
		{
			pObject->OnIOCallback(bSucc, dwTransferred, lpOverlapped);
		}
	}
}
void CIOSocket::OnIOCallback(BOOL bSucc, DWORD dwTransferred, LPOVERLAPPED lpOverlapped)
{
	if (!bSucc)
	{
		if (lpOverlapped == &this->m_OverlappedRead)
		{
			Close();
		}

	}
	else
	{
		if (lpOverlapped == &this->m_OverlappedWrite)
		{
			WriteCallback(dwTransferred);
		}
		else if (lpOverlapped == &m_OverlappedRead)
		{
			ReadCallback(dwTransferred);
		}
	}
}
void CIOSocket::ReadCallback(DWORD dwTransferred)
{
	if (m_hSocket == INVALID_SOCKET || dwTransferred <= 0) return;

	m_pBuffer->Length += dwTransferred;
	OnRead();
}
