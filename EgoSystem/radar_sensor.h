#pragma once
#include "../common/notifier.h"
#include <thread>
#pragma pack(1)

 

struct Radar_Frame
{
      uint8_t Addr=0x01;
      uint8_t Func=0x03;
      uint8_t RegAddr[2]={0x01,0x06};
      uint8_t RegData[2]={0x00,0x04};
      uint8_t uchCRCHi = 0xa5;
      uint8_t uchCRCLo = 0xf4;
      //uint16_t crc;
       

};
struct Radar_Ret
{
      uint8_t Addr=0x01;
      uint8_t Func=0x03;
      uint8_t RegAddr={0x06};
      uint8_t RegData[8];
      uint16_t crc;  
};
#pragma pack()
struct RadarData
{
     int32_t r0;
     int32_t r1;
     int32_t r2;
     int32_t r3;
     int32_t r4;
     int32_t r5;
     int32_t r6;
     int32_t r7;
};

class CMessageQueue;
 
class CRadarSensor//:public ISensorNotify
{
public:
     CRadarSensor(CMessageQueue *q);
     void Notify(int8_t * buffer,int32_t size);
     void SetSensorSocket(SensorSocket<CRadarSensor>* can);
     void Start();
     void Stop();
    // void PreProcess();

private:
     void Run();
     uint16_t bcd2dec(const uint8_t *data);
     unsigned int CRC16(uint8_t *buf, int len);
private:
     CMessageQueue* _message;
     SensorSocket<CRadarSensor> * _socket;
     //std::thread _thread;
     //bool _run;
     RadarData _data;
     int32_t _count=0;
};
