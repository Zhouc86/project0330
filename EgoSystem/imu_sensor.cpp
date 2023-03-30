#include <stdint.h>
#include "../common/comm.h"
#include "../common/iobuffer.h"
#include "../common/sensor_socket.h"
#include "imu_sensor.h"
#include "message_queue.h"
                     
CImuSensor::CImuSensor(CMessageQueue* q):_message(q)
{
    
}
void CImuSensor::SetSensorSocket(SensorSocket<CImuSensor>* can)
{
    _can=can;
}
void CImuSensor::Notify(int8_t * buffer,int32_t size)
{
  
     //struct cannet_fram frame;
     int32_t receivedCnt = size/CAN_MSG_LEN;
	   cannet_frame* p = (cannet_frame*)buffer;

     for(int i=0; i<receivedCnt; i++)
     {
         p->canid=htonl(p->canid);
        
         
         if((p->canid&0xFF0)==0x580)
         {
           ImuProcess(*p);
         }
         p++;
     }
     if(receivedCnt>0)
     {
       //std::cout<<"rot :"<<_imu.rx<<","<<_imu.ry<<","<<std::endl;
 
       _message->WriteIMUData(&_imu);
     }
     
      
}
int32_t CImuSensor::bcd2dec(const int8_t *data)
{
   
   int32_t h0=(data[0]>>4)&0xF;
   int32_t l0=data[0]&0xF;
   int32_t h1=(data[1]>>4)&0xF;
   int32_t l1=data[1]&0xF;
   int32_t ret=((h0*10+l0)*10+h1)*10+l1;
   return ret;
}
void CImuSensor::ImuProcess(const cannet_frame& frame)
{
         const int8_t* p=frame.data;
         
         if(p[0]==0x50&&p[1]==0x00)  //角速度
         {
         
         /*
            
            p+=2;

            float temp=bcd2dec(p);// (((((p[0]>>4)&0xF*10)+(p[0]&0xF))*10)+((p[1]>>4)&0xF)) 00/16+p[1]*10/16;
            float val=temp-5000;
            _imu.AngVel.X=val/10.f;
            p+=2;
             temp=bcd2dec(p);
             val=temp-5000;
            _imu.AngVel.Y=val/10.f;
            p+=2;
             temp=bcd2dec(p);
             val=temp-5000;
            _imu.AngVel.Z=val/10.f;
          */

         }
         else if(p[0]==0x54&&p[1]==0x00) //加速度
         {
            
            /*
            p+=2;
            float temp=bcd2dec(p);
            float val=temp-5000;
            _imu.Accel.X=val/2500.f;
            p+=2;
             temp=bcd2dec(p);
             val=temp-5000;
            _imu.Accel.Y=val/2500.f;
            p+=2;
             temp=bcd2dec(p);
             val=temp-5000;
            _imu.Accel.Z=val/2500.f;
            */
           
         }
         else
         {

            
        
             float sign=((p[0]>>4)&0xF) !=0?-1.f:1.f;
             int32_t h2=p[0]&0xF;
             int32_t h1=(p[1]>>4)&0xF;
             int32_t h0=p[1]&0xf;
             int32_t l1=((p[2]>>4)&0xF)*10+(p[2]&0xF);
               
             float dec=h2*100+h1*10+h0+(l1/100.f);
            // float fraction=((p[2]>>4)&0xF)*10+(p[2]&0xF);
             _imu.rx=dec*sign;//(dec+fraction/100.f)*sign;
             //p+=3;
             sign=((p[3]>>4)&0xF) !=0?1.f:-1.f;
             h2=p[3]&0xF;
             h1=(p[4]>>4)&0xF;
             h0=p[4]&0xf;
             l1=((p[5]>>4)&0xF)*10+(p[5]&0xF);
             dec=h2*100+h1*10+h0+(l1/100.f);
             _imu.ry=dec*sign;
            /*
             p+=3;
             h2=(p[0]>>4)&0xF;
             h1=(p[0])&0xF;
             h0=(p[1]>>4)&0xF;
             int32_t l0=(p[1])&0xF;

             dec=h2*100+h1*10+h0+l0/10.f;
             _imu.rz=dec;
             */ 
         }
         
}
 
void CImuSensor::Start()
{
    CIOBuffer buffer;
    cannet_frame* frame=(cannet_frame *)buffer.Buffer;
    frame->canid=htons(0x600+0x05);
    frame->dlc=8;
    frame->data[0]=0x40;
    frame->data[1]=0x53;
    frame->data[2]=0x10;
    frame->data[3]=0;
    frame->data[4]=0;
    frame->data[5]=0;
    frame->data[6]=0;
    frame->data[7]=0;
    buffer.Length=sizeof(cannet_frame);
   _can->Write(&buffer);
   _can->Read(&buffer);

}
void CImuSensor::Stop()
{

}
void CImuSensor::PreProcess()
{
   
}