#pragma once
class CIOBuffer;
struct IO_STRUCT
{
	enum
	{
		BASE_SIZE = 64,
		ALLOC_ARG = 256
	};
	IO_STRUCT* pNext;
	CIOBuffer* pBuffer;
	IO_STRUCT() :pNext(nullptr), pBuffer(nullptr)
	{

	}
};
class CIOSocket
{
public:
	enum {
		ALLOC_ARG = 5,
		BASE_SIZE = 2
	};
	CIOSocket(HANDLE hCom, SOCKET s);
	 
	~CIOSocket();
	static unsigned int __stdcall IOThread(void* param);
	virtual void OnCreate() = 0;
	virtual void OnClose() = 0;
	virtual void OnRead() = 0;
	void Read(DWORD dwLeft);
	void Close();
	void Write(CIOBuffer* pBuffer);
	void WriteCallback(DWORD dwTransferred);
	void ReadCallback(DWORD dwTransferred);
	void AddRef();
	void Release();
	 
	void OnIOCallback(BOOL bSucc, DWORD dwTransferred, LPOVERLAPPED lpOverlapped);
protected:
	bool Init(HANDLE hCom);
	HANDLE m_hComplete;
	SOCKET m_hSocket;
	CIOBuffer* m_pBuffer;
	IO_STRUCT* m_pFirstHead;
	IO_STRUCT* m_pLastHead;
	OVERLAPPED m_OverlappedRead;
	OVERLAPPED m_OverlappedWrite;
	std::mutex m_lock;
	DWORD m_dwBufferCount;
	WSABUF  m_WriteBuff[64];
	DWORD m_dwWriteCount;
	DWORD m_IOCount;
	bool m_bSending;

	long  m_lRef;
};

