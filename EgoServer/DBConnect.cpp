#include <Windows.h>
#include "DBConnect.h"
#include <assert.h>
const uint64_t MYSQL_TIMEOUT = 6 * 3600 * 1000;

CDBConnectPool& CDBConnectPool::GetInstance()
{
	static CDBConnectPool db;
	return db;
}
void CDBConnectPool::initPool(std::string url_, std::string db_, std::string user_, std::string password_, int maxSize_)
{

	User = user_;
	Password = password_;
	url = url_;
	db = db_;
	MaxSize = maxSize_;
	CurSize = 0;
	SqlDriver = sql::mysql::get_driver_instance();
	InitConnection(MaxSize >> 1);
}
void CDBConnectPool::InitConnection(int32_t initSize)
{
	Head = nullptr;
	std::lock_guard<std::mutex> slock(DbLock);//.Enter();
	for (int32_t i = 0; i < initSize; i++)
	{
		CConnectionPtr<sql::Connection>* conn = CreateConnection();
		conn->m_Next = Head;
		Head = conn;
		++CurSize;
	}
	//Lock.Leave();
}
CConnectionPtr<sql::Connection>* CDBConnectPool::CreateConnection()
{
	try {

		sql::Connection* conn = SqlDriver->connect(url.c_str(), User.c_str(), Password.c_str());
		conn->setSchema(db.c_str());

		uint64_t tick = GetTickCount64();
		return new CConnectionPtr<sql::Connection>(conn, tick);
	}
	catch (sql::SQLException& e)
	{

		printf("%s", e.what());
		return nullptr;
	}
	return nullptr;
}
CQPtr<CConnectionPtr<sql::Connection>> CDBConnectPool::QueryConnect()
{
	CConnectionPtr<sql::Connection>* ptr = nullptr;
	std::lock_guard<std::mutex> slock(DbLock);
	ULONGLONG CurTick = GetTickCount64();
	while (Head != nullptr)
	{
		if ((CurTick - Head->OperationTick >= MYSQL_TIMEOUT) || !(*Head)->isValid())
		{
			CConnectionPtr<sql::Connection>* temp = Head;
			Head = Head->m_Next;
			DestroyConnection(temp);
			--CurSize;
		}
		else
		{
			ptr = Head;
			ptr->SetOperationTick(CurTick);
			Head = Head->m_Next;
			--CurSize;
			break;
		}

	}
	if (ptr == nullptr)
	{
		if (CurSize < MaxSize)
		{
			ptr = CreateConnection();
			if (ptr != nullptr)
			{
				++CurSize;
			}
		}
		else
		{
			assert(false);
		}

	}

	return ptr;
}
void CDBConnectPool::ReleaseConnection(CConnectionPtr<sql::Connection>* ptr)
{
	std::lock_guard<std::mutex> slock(DbLock);
	ptr->m_Next = Head;
	Head = ptr;
	++CurSize;

}
void CDBConnectPool::DestoryConnPool()
{
	std::lock_guard<std::mutex> slock(DbLock);
	while (Head != nullptr)
	{
		CConnectionPtr < sql::Connection>* temp = Head;
		Head = Head->m_Next;
		DestroyConnection(temp);
	}
}
void CDBConnectPool::DestroyConnection(CConnectionPtr<sql::Connection>* conn)
{
	(*conn)->close();
	delete conn;
}