#include <stdint.h>
#include <chrono>
#include "../common/comm.h"

#include "../common/iobuffer.h"
#include "../common/sensor_socket.h"
#include "radar_sensor.h"
#include "message_queue.h"
extern int16_t make_int16(int8_t h,int8_t l);
CRadarSensor::CRadarSensor(CMessageQueue* q):_message(q)
{
       memset(&_data,0,sizeof(_data));
}
 
uint16_t CRadarSensor::bcd2dec(const uint8_t *data)
{
     int32_t h=data[0]&0x00FF;
     int32_t l=data[1]&0x00FF;
     return h*10+l;

}
 
void CRadarSensor::Notify(int8_t * buffer,int32_t size)
{
    
 
    cannet_frame* p=(cannet_frame *)buffer;
    int32_t  count=size/sizeof(cannet_frame);
   
    for(auto i=0;i<count;i++)
    {
        int32_t canid=ntohl(p[i].canid);
        if(canid==0x0611)
        {
          _data.r0=bcd2dec((const uint8_t*)&p[i].data[0]);
          _data.r1=bcd2dec((const uint8_t*)&p[i].data[2]);
          _data.r2=bcd2dec((const uint8_t*)&p[i].data[4]);
          _data.r3=bcd2dec((const uint8_t*)&p[i].data[6]);
        }
        else if(canid==0x0612)
        {
          _data.r4=bcd2dec((const uint8_t*)&p[i].data[0]);
          _data.r5=bcd2dec((const uint8_t*)&p[i].data[2]);
        }

    }
    _count++;
    if(_count >200)
    {
         _message->WriteRadarData(_data);
         _count=0;
    }
  
  
  

 

}
/*
unsigned int CRadarSensor::CRC16(uint8_t *buf, int len)
{
    unsigned int crc = 0xFFFF;

    for (int pos = 0; pos < len; pos++)
    {
        crc ^= (unsigned int)buf[pos]; // XOR byte into least sig. byte of crc

        for (int i = 8; i != 0; i--)
        { // Loop over each bit
            if ((crc & 0x0001) != 0)
            {              // If the LSB is set
                crc >>= 1; // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else           // Else LSB is not set
                crc >>= 1; // Just shift right
        }
    }
    //uint8_t* t=(uint8_t *)&crc;
    //std::cout<<std::hex<<(int32_t )t[0]<<std::endl;
     
    return crc;
}
 */
void CRadarSensor::Start()
{
  //_thread=std::thread(&CRadarSensor::Run,this);

    //20230329
    /*
    cannet_frame frame;
  memset(&frame,0,sizeof(frame));
  frame.canid=htons(0x0601);
  frame.dlc=3;
  frame.data[0]=0xb3;
  frame.data[1]=0x10;
  frame.data[2]=0xff;
  CIOBuffer buffer;
  memcpy(buffer.Buffer,&frame,sizeof(frame));
    

  buffer.Length=sizeof(frame);
  _socket->Write(&buffer);
    */
  
  
    CIOBuffer pBuffer;
    pBuffer.Length = (sizeof(Radar_Frame) / sizeof(char));
    Radar_Frame _CheckRadarStatus;
    memcpy(pBuffer.Buffer, (uint8_t*)&_CheckRadarStatus, pBuffer.Length);
      // _radar->Write(&pBuffer);
    _socket->Write(&pBuffer);
  
    
}
/*
void CRadarSensor::PreProcess()
{
     
  
}

void CRadarSensor::Run()
{
    _run=true;
    Radar_Frame frame1;
    frame1.crc=CRC16((uint8_t *)&frame1,6);
    CIOBuffer buffer1;
    memcpy(buffer1.Buffer,&frame1,sizeof(Radar_Frame));
    

    buffer1.Length=sizeof(Radar_Frame);
    Radar_Frame frame2;
    frame2.Addr=2;
    //frame2.RegData[1]=2;
    frame2.crc=CRC16((uint8_t *)&frame2,6);
    CIOBuffer buffer2;
    memcpy(buffer2.Buffer,&frame2,sizeof(Radar_Frame));
    

    buffer2.Length=sizeof(Radar_Frame);
    while(_run)
    {
        _socket->Write(&buffer1);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
      //  _socket->Write(&buffer2);
     //  std::this_thread::sleep_for(std::chrono::milliseconds(200));
        _message->WriteRadarData(_data);
     //  std::cout<<_data.r0<<","<<_data.r1<<","<<_data.r2<<","<<_data.r3<<","<<_data.r4<<","<<_data.r5<<","<<_data.r6<<","<<_data.r7<<std::endl;
    }
    std::cout<<__FUNCTION__<<" finished"<<std::endl;
}
  */
void CRadarSensor::SetSensorSocket(SensorSocket<CRadarSensor>* can)
{
    _socket=can;
}
void CRadarSensor::Stop()
{

  cannet_frame frame;
  memset(&frame,0,sizeof(frame));
  frame.canid=htons(0x0601);
  frame.dlc=3;
  frame.data[0]=0xb3;
  frame.data[1]=0x10;
  frame.data[2]=0x00;
  CIOBuffer buffer;
  memcpy(buffer.Buffer,&frame,sizeof(frame));
    

  buffer.Length=sizeof(frame);
  _socket->Write(&buffer);
    
}