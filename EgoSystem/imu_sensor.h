#pragma once
#include "../common/comm.h"
#include "../common/notifier.h"
class CMessageQueue;
class SocketCan;

struct ImuData
{
    /* FVector3<float> AngVel;  // 3 floats, angular velocity vector
     FVector3<float> Rotation;
     FVector3<float> Accel;
*/
      float rx;
      float ry;
     // float rz;

     

};
class CImuSensor 
{
public:
      CImuSensor(CMessageQueue* q);
      void Start();
	void Stop();
      void Notify(int8_t * buffer,int32_t size);
      void PreProcess();
      void SetSensorSocket(SensorSocket<CImuSensor>* can); 
private:
      void ImuProcess(const cannet_frame& frame);
      int32_t bcd2dec(const int8_t *data);
     
private:
    CMessageQueue * _message;
    ImuData _imu; 
    SensorSocket<CImuSensor>* _can;
    
};