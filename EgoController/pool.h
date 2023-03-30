#pragma once
#include <mutex>
#include <assert.h>
#include <iostream>

template<typename T>
class Alloc_Pool
{
private:
	struct NODE
	{
		NODE* next;
		char t[sizeof(T)];
	};
public:
	Alloc_Pool();
	~Alloc_Pool();

	//T* Alloc();
	template<typename...X>
	T* Alloc(X...Args);
	 
	void Free(T* ptr);
	static Alloc_Pool& GetInstance();
private:
	bool InternalAlloc();
	NODE* Head;
	int m_BaseIndex;
	NODE* PtrBase[T::BASE_SIZE];
	NODE* next;
	std::mutex lock;

};
template<typename T>
bool Alloc_Pool<T>::InternalAlloc()
{
	NODE* p = NULL;
	if (m_BaseIndex < T::BASE_SIZE)
		p = PtrBase[m_BaseIndex++] = (NODE*)malloc(sizeof(NODE) * T::ALLOC_ARG);
	else
	{
		//p = (NODE*)malloc(sizeof(NODE)*T::ALLOC_ARG);
		assert(false);

	}
    assert(p!=nullptr);
	if (p == NULL)
	{
		std::cout << "no memory" << std::endl;
	    exit(0);
		return false;
	}
	for (int i = 0; i < T::ALLOC_ARG; i++)
	{
		if (Head == NULL)
		{
			Head = &p[i];
			Head->next = NULL;
		}
		else
		{
			p[i].next = Head;
			Head = &p[i];
		}
	}
	return true;
}
template<typename T>
Alloc_Pool<T>::Alloc_Pool()
{
	Head = NULL;
	m_BaseIndex = 0;
	//InitializeCriticalSection(&m_cs);
	InternalAlloc();
}
template<typename T>
Alloc_Pool<T>::~Alloc_Pool()
{

	for (int i = 0; i < m_BaseIndex; i++)
		free(PtrBase[i]);
}
 
template<typename T>
template<typename...X>
T* Alloc_Pool<T>::Alloc(X...Args)
{
	std::lock_guard<std::mutex> lg(lock);
	if (Head == NULL)
	{
		InternalAlloc();
	}
	NODE* temp = Head;
	Head = Head->next;
	new(temp->t) T(Args...);

	return reinterpret_cast<T*>(temp->t);
}
 
template<typename T>
Alloc_Pool<T>& Alloc_Pool<T>::GetInstance()
{
	static Alloc_Pool<T> s;
	return s;
}
template<typename T>
void Alloc_Pool<T>::Free(T* ptr)
{
	ptr->~T();
	int8_t* p = (int8_t*)ptr;
	p -= sizeof(NODE*);
	NODE* node = (NODE*)p;
	std::lock_guard<std::mutex> lg(lock);
	node->next = Head;
	Head = node;



}