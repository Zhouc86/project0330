// EgoServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#include "DBConnect.h"
#include "UserManager.h"
#include "NetService.h"
#define DBHOST "tcp://localhost:3306/egodb"


int main()
{
	char path[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, path);
	char file[MAX_PATH];
	sprintf_s(file, "%s\\config.ini", path);
	char user[64];
	char pass[64];
	GetPrivateProfileStringA("mysql", "user", "", user, sizeof(user), file);
	GetPrivateProfileStringA("mysql", "password", "", pass, sizeof(pass), file);
	CDBConnectPool::GetInstance().initPool(DBHOST, "egodb", user, pass, 12);
	CNetService::GetInstance().Start();
	CUserManager::GetInstance().Start();
	CNetService::GetInstance().Run();
	return 0;
}