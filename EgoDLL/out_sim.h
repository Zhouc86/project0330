#pragma once
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include "../common/comm.h"
#include "../common/notifier.h"

class CIOBuffer;
#pragma pack(1)
struct OutSimPack
{
	/*
	unsigned int time;
	float AVelocityX; 
	float AVelocityY;
	float AVelocitZ;
	float yaw;
	float pitch;
	float roll;
	float accX;
	float accY;
	float accZ;
	float velX;
	float vely;
	float velz;
	int posx;
	int posy;
	int posz;

	*/
	int8_t  start1 = 0x55;
	int8_t  start2 = 0xaa;
	int8_t  start3 = 0xbb;
	int8_t  cmd =6;
	float tx=0;
	float ty=0;
	float tz=0;
	float rx;
	float ry;
	float rz=0;
	float var1=0;
	float var2=0;
	float var3=0;

	float var4=0;
	float var5=0;
	float var6=0;

	int time=50;



};
#pragma pack()
class COutSim
{
public:
	COutSim(const char * ip,int32_t port);
	void Start();
	void Stop();
	void PushSimPack(CIOBuffer* pBuffer);
	void OnPeerMessage(int16_t cmd, int16_t length, const void* data);
private:
	void Run();
	bool SendOutSim(CIOBuffer* pBuffer);
private:
	SOCKET _socket;
	std::thread _thread;
	bool _run;
	std::mutex Mtx;
	std::condition_variable cv;
	CIOBuffer* _head;
	std::string _ip;
	int32_t _port;
	
};

