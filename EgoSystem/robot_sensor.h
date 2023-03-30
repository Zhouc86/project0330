#pragma once
#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <WS2tcpip.h>
#include <WinSock2.h>
#define socketerrno WSAGetLastError()
#define SOCKET_EAGAIN_EINPROGRESS WSAEINPROGRESS
#define SOCKET_EWOULDBLOCK WSAEWOULDBLOCK
#ifndef _SSIZE_T_DEFINED
typedef int ssize_t;
#define _SSIZE_T_DEFINED
#endif
#ifndef _SOCKET_T_DEFINED
typedef SOCKET socket_t;
#define _SOCKET_T_DEFINED
#endif
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <fcntl.h>
#define socketerrno errno
#define SOCKET_EAGAIN_EINPROGRESS EAGAIN
#define SOCKET_EWOULDBLOCK EWOULDBLOCK
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#ifndef _SOCKET_T_DEFINED
typedef int socket_t;
#define _SOCKET_T_DEFINED
#endif
#endif
#include "../common/notifier.h"
 
class CMessageQueue;
#pragma pack(1)
struct  OpState{
    uint16_t func=htons(10);
    uint16_t boot_addr=htons(101);
    uint16_t boot_value=0;
    uint16_t start_addr=htons(102);
    uint16_t start_value=0;
    uint16_t stop_addr=htons(103);
    uint16_t stop_value=0;
    uint16_t emergency_addr=htons(104);
    uint16_t emergency_value=0;
    uint16_t gear_addr=htons(135);
    uint16_t gear_value=0;
    uint16_t wip_addr=htons(132);
    uint16_t wip_value=0;
    uint16_t flight_addr=htons(133);
    uint16_t flight_value=0;
    uint16_t blight_addr=htons(134);
    uint16_t blight_value=0;
    uint16_t park_addr=htons(135);
    uint16_t park_value=0;

};

struct Analog_rtu {
    int16_t func=htons(10);
    int16_t steer_addr=htons(113);
    int16_t steer_value=0;
    int16_t arm_addr=htons(117);
    int16_t arm_value=0;
    int16_t bucket_addr=htons(119);
    int16_t bucket_value=0;
      int16_t throttle_addr=htons(111);
     int16_t throttle_value=0; 
     int16_t brake_addr=htons(115);
     int16_t brake_value=0;
    
};
 
struct ReadCmd
{
     int16_t func=htons(03);
     int16_t boot_addr=htons(201);
     int16_t start_addr=htons(203);
     int16_t stop_addr=htons(205);
     int16_t emergenc_addr=htons(207);

};
struct ReadRet
{
    int16_t func=3;
    int16_t boot_addr;
    int16_t boot_value;
    int16_t start_addr;
    int16_t start_value;
    int16_t stop_addr;
    int16_t stop_value;
    int16_t emergenc_addr;
    int16_t emergenc_value;
};
/*
struct modbus_wtu
{
   // uint8_t Addr=0x03;
    uint8_t Func=0x10;
    uint16_t RegAddr;
    uint8_t RegCount[2]={0,1};
    uint8_t ValueCount=2;
    uint8_t RegValue[2]={0,0};
    uint16_t crc;
};
struct modbus_wtu_ex
{
    uint8_t Addr=0x03;
    uint8_t Func=0x10;
    uint8_t RegAddr[2]={0,111};
    uint8_t RegCount[2]={0,10};
    uint8_t ValueCount=20;
    uint16_t Value[20]={0,0};
    uint16_t crc;
};
struct modbus_wtu_qs
{
    uint8_t Addr=0x03;
    uint8_t Func=0x10;
    uint8_t RegAddr[2]={0,121};
    uint8_t RegCount[2]={0,5};
    uint8_t ValueCount=10;
    uint8_t Value[10]={0,0};
    uint16_t crc;
};
struct modbus_rtu
{
    uint8_t Addr=0x03;
    uint8_t Func=0x03;
    uint8_t RegAddr[2]={0,0};
    uint8_t RegCount[2]={0,2};
    uint16_t crc;
};
*/
#pragma pack()
class CRobotSensor 
{
public:
    CRobotSensor(CMessageQueue * q);
    void Notify(int8_t * buffer,int32_t size);
    void SetSensorSocket(SensorTCP<CRobotSensor>* can);
    void Start();
    void Stop();
    bool OnStartup();
    void OnHangup();
    bool OnBootstrap(); //自举

    void OnAnalog(RemoNet::CCRobotAnalog& req);
    bool Emergency();  //急停 只局限于程序 和网络
     bool Analog(int16_t steer,int16_t arm,int16_t bucket,int16_t throttle,int16_t brake);
private:
    unsigned int CRC16(uint8_t *buf, int len);
  
    void Run();
    void OnChangeState();
    void OnFLight(bool on); //前车灯
    void OnBLight(bool on); //后车灯
    void OnWiper(bool on);
    void OnGears(int16_t value);
    void OnPark(bool on);
    bool Resume();  //恢复
    bool AutoRise(); //自动举升
    bool AutoDown(); //自动方平
  

 //   bool Throttle(int32_t value);
 //   bool Brake(int32_t value);
 //   bool Steer(int32_t value);
 //   bool Bucket(int32_t value);
   
private:
   
   
    CMessageQueue * _message;
    SensorTCP<CRobotSensor> * _socket;
    bool _bootstrap=false;
    bool _startup=false;
    
    bool _resume=false;
    bool _emergency=false;
    bool _autorise=false;
    RemoNet::Gears _gears=RemoNet::Gears::N;
    bool _frontLight=false;
    bool _backLight=false;
    bool _wipe=false;
    bool _park=false;
    std::thread _thread;
    bool _run;

    
};