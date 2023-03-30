#pragma once
#include <mutex>
#include <mysql_connection.h>

#include <mysql_driver.h>  
#include <cppconn/exception.h>  
#include <cppconn/driver.h>  
#include <cppconn/connection.h>  
#include <cppconn/resultset.h>  
#include <cppconn/prepared_statement.h>  
#include <cppconn/statement.h> 
template<typename T>
class CConnectionPtr
{
public:
	CConnectionPtr(T* conn, uint64_t& tick) :m_Conn(conn), m_Next(nullptr), OperationTick(tick) {}
	~CConnectionPtr() {
		delete m_Conn;
	}

	T* operator->() { return m_Conn; }
	void SetOperationTick(uint64_t& tick);
private:
	T* m_Conn;
public:
	CConnectionPtr* m_Next;
	uint64_t OperationTick;
};

template<typename T>
void CConnectionPtr<T>::SetOperationTick(uint64_t& tick)
{
	OperationTick = tick;
}
template<typename T>
class CQPtr
{
public:
	CQPtr(T* ptr);
	CQPtr& operator=(T* ptr);
	CQPtr& operator=(const CQPtr<T>& ptr);
	//T* operator->() { return m_Conn; }
	~CQPtr();
	T* get();
private:
	T* m_Conn;
};
class CDBConnectPool
{
public:
	static CDBConnectPool& GetInstance();
	void initPool(std::string url_, std::string db, std::string user_, std::string password_, int maxSize_);

	void InitConnection(int32_t initSize);
	CConnectionPtr<sql::Connection >* CreateConnection();
	CQPtr<CConnectionPtr<sql::Connection>> QueryConnect();
	void ReleaseConnection(CConnectionPtr<sql::Connection>* ptr);
	void DestroyConnection(CConnectionPtr<sql::Connection>* conn);

	void DestoryConnPool();
private:
	std::string User;
	std::string Password;
	std::string url;
	std::string db;
	int32_t MaxSize;
	int32_t CurSize;

	sql::Driver* SqlDriver;
	CConnectionPtr<sql::Connection >* Head;
	std::mutex DbLock;

};

template<typename T>
T* CQPtr<T>::get()
{
	return m_Conn;
}
template<typename T>
CQPtr<T>& CQPtr<T>::operator=(const CQPtr<T>& ptr)
{
	if (m_Conn != nullptr)
		CDBConnectPool::GetInstance().ReleaseConnection(m_Conn);

	m_Conn = ptr.get();
	ptr.m_Conn = nullptr;
}
template<typename T>
CQPtr<T>::CQPtr(T* ptr) :m_Conn(ptr)
{

}


template<typename T>
CQPtr<T>& CQPtr<T>::operator=(T* ptr)
{
	if (m_Conn != nullptr)
		CDBConnectPool::GetInstance().ReleaseConnection(m_Conn);

	m_Conn = ptr;

}
template<typename T>
CQPtr<T>::~CQPtr()
{
	if (m_Conn != nullptr)
		CDBConnectPool::GetInstance().ReleaseConnection(m_Conn);
}
