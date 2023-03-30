#pragma once
#include <atomic>
struct MessageHead
{
	int16_t Command;
	int16_t Length;
	 
	 

	int32_t Serialize(int8_t* Data)
	{

		*(int16_t*)&Data[0] = Command;
		*(int16_t*)&Data[sizeof(int16_t)] = Length;
		 
		return Size();
	}
	void Deserialize(int8_t* Data)
	{
		Command = *(int16_t*)&Data[0];
		Length = *(int16_t*)&Data[2];
		 
	}
	static int32_t Size()
	{
		return sizeof(int16_t) * 2;
	}

};
class CRef
{
public:
	CRef();
	void AddRef();
	long Release();
	virtual void OnFree() = 0;
protected:
	std::atomic<long> m_lRef;
};
class CIOBuffer :public CRef
{
public:
	enum
	{
		IO_BUFFER_SIZE = 4096*4
	};
	enum
	{
		BASE_SIZE = 4,
		ALLOC_ARG = 4
	};

	int32_t Length;
	CIOBuffer* NextBuf;
	CIOBuffer* PrevBuf;
	int8_t Buffer[IO_BUFFER_SIZE];
	virtual void OnFree() override;

	CIOBuffer();
	virtual ~CIOBuffer();
public:

	static CIOBuffer* Alloc();
};

