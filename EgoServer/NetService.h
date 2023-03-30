#pragma once
class CNetService
{
public:
	void Start();
	void Stop();
	void Run();
	static CNetService& GetInstance();
private:
	HANDLE m_hCompletePort;

	bool CreateIOThread();
	SOCKET m_hSocket;
	HANDLE m_hEvent;
private:
	std::thread _thread;
	bool _bRunning;
	std::vector<std::thread> _walkarray;
};
