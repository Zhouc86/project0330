#include <stdint.h>
#include "../common/comm.h"
#include "../common/iobuffer.h"
#include "../common/sensor_socket.h"
#include "protocol.pb.h"
#include "robot_sensor.h"
#include "message_queue.h"
uint8_t HIBYTE(int16_t value)
{
  return (uint8_t)((value>>8)&0xFF);
}
uint8_t LOBYTE(int16_t value)
{
  return (uint8_t)((value)&0xFF);
}

int16_t make_int16(int8_t h,int8_t l)
{
  int16_t hi=(int16_t)(h&0x00FF);
  int16_t low= (int16_t)(l&0x00FF);

   return (hi<<8)|low;
}
#define ADDR_BOORSTRAP 201
CRobotSensor::CRobotSensor(CMessageQueue * q):_message(q)
{
  
}

void CRobotSensor::Start()
{
   _bootstrap=false;
   _startup=false;
    while(OnBootstrap()==false)
    {
      std::cout<<"Bootstrap failed"<<std::endl;
     
    }
  /*   while(OnStartup()==false)
     {
      {
      std::cout<<"Startup failed"<<std::endl;
    
    }
     }
     */
   _thread=std::thread(&CRobotSensor::Run,this);
/*
      modbus_wtu data;
   //memset(&data,0,sizeof(data));
   
   data.RegAddr[1]=130;
   data.ValueCount=2;
   data.RegValue[1]=2;
   data.crc=CRC16((uint8_t *)&data,sizeof(modbus_wtu)-2);
   CIOBuffer buffer;
    memcpy(buffer.Buffer,&data,sizeof(data));
    buffer.Length=sizeof(data);
    _socket->Write(&buffer);
    */
    
  
}

void CRobotSensor::Stop()
{
     
     _run=false;
     Emergency();
     _thread.join();


} 
void CRobotSensor::Notify(int8_t * buffer,int32_t size)
{   
   
   // long long _curTick=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
 ///  _message->SetTick(_curTick);
   
    ReadRet * ret=(ReadRet *) buffer;
    _bootstrap=ntohs(ret->boot_value)==1;
     /*for(int i=0;i<size;i++)
     {
        int32_t temp=(int32_t)(buffer[i]&0xFF);
        std::cout<<std::hex<<temp<<",";
     }
     std::cout<<std::endl;
     */
    /*
     if(buffer[1]==0x10) return;
     if(buffer[1]!=0x03||buffer[2]!=0x04) return;
     int16_t addr=make_int16(buffer[3],buffer[4]);
     int16_t value=make_int16(buffer[5],buffer[6]);
     if(addr==201||addr==202)
     {
       _bootstrap=value==1;
     
       //

     }
     if(addr==203) //启动反馈
     {
      _startup=value==1;
    
     }
     if(addr==205)
     {
        _startup=value==1?false:true;
     }
     if(addr==207)
     {
       _emergency=value==1;
     }
      */
}
unsigned int CRobotSensor:: CRC16(uint8_t *buf, int len)
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
    
     
    return crc;
}
bool CRobotSensor::OnBootstrap()
{
  
   
   //if(_bootstrap==true) return true;
   _bootstrap=false;
    CIOBuffer buffer;
    
    OpState data;
    //data.func=htons(data.func);
    data.boot_value=htons(1);
   
    memcpy(buffer.Buffer,&data,sizeof(data));
    buffer.Length=sizeof(data);
    _socket->Write(&buffer);
     
    int32_t count=0;
    ReadCmd rtu;
    
    memcpy(buffer.Buffer,&rtu,sizeof(rtu));
    buffer.Length=sizeof(ReadCmd);
    while(!_bootstrap&&count<30)
    {
       std::this_thread::sleep_for(std::chrono::milliseconds(100));	
     
      _socket->Write(&buffer);
      count++;
    }
    /*
    RemoNet::CCBooStrapRep Rep;
    Rep.set_ret(_bootstrap);
    CIOBuffer Buffer;
    MessageHead Head;
    Head.Command = RemoNet::CC_BootRep;
    Head.Length = Rep.ByteSizeLong();
    Head.Serialize(Buffer.Buffer);
    auto ptr = Buffer.Buffer + MessageHead::Size();
    Rep.SerializeToArray(ptr, Head.Length);
    Buffer.Length = Head.Length + MessageHead::Size();
     _message->WritePacket(ChannelType::CHANNEL_CAR,&Buffer);    
 */
    
  
  return _bootstrap;
}
bool CRobotSensor::Emergency()
{
    OpState  state;
    state.emergency_value=htons(1);
    CIOBuffer buffer;
    memcpy(buffer.Buffer,&state,sizeof(state));
    buffer.Length=sizeof(state);
    _socket->Write(&buffer);
  /*
  if(_ctx!=nullptr)
  {
    int res = modbus_write_register(_ctx, 104, 1);  //急停
    return res==1;
  }
  */
  return false;
}
bool CRobotSensor::AutoRise()  //一键举升
{
     _autorise=true;
     CIOBuffer buffer;
    /*
    modbus_wtu data;
    data.RegAddr[1]=105;
    
    data.ValueCount=2;
    data.RegValue[1]=1;
    data.crc=CRC16((uint8_t *)&data,sizeof(modbus_wtu)-2);
    memcpy(buffer.Buffer,&data,sizeof(data));
    buffer.Length=sizeof(data);
    _socket->Write(&buffer);

  */
  return true;
}
bool CRobotSensor::AutoDown()
{
   CIOBuffer buffer;
    /*
    modbus_wtu data;
    data.RegAddr[1]=211;
    
    data.ValueCount=2;
    data.RegValue[1]=1;
    data.crc=CRC16((uint8_t *)&data,sizeof(modbus_wtu)-2);
    memcpy(buffer.Buffer,&data,sizeof(data));
    buffer.Length=sizeof(data);
    _socket->Write(&buffer);
    */
    return true;
}
void CRobotSensor::OnChangeState()
{
   OpState  state;
  state.flight_value=htons(_frontLight?1:0);
  state.wip_value=htons(_wipe?1:0);
  state.blight_value=htons(_backLight?1:0);
    CIOBuffer buffer;
    memcpy(buffer.Buffer,&state,sizeof(state));
    buffer.Length=sizeof(state);
       _socket->Write(&buffer);
}
void CRobotSensor::OnFLight(bool on)
{
   OpState  state;
  state.flight_value=htons(on?1:0);
  state.wip_value=htons(_wipe?1:0);
    CIOBuffer buffer;
    memcpy(buffer.Buffer,&state,sizeof(state));
    buffer.Length=sizeof(state);
    _socket->Write(&buffer);
  /*
     modbus_wtu data;
    data.RegAddr[1]=133;
    
    data.ValueCount=2;
    data.RegValue[1]=on?1:0;
    data.crc=CRC16((uint8_t *)&data,sizeof(modbus_wtu)-2);
    CIOBuffer buffer;
    memcpy(buffer.Buffer,&data,sizeof(data));
    buffer.Length=sizeof(data);
   // _socket->Write(&buffer);
   */
}
void CRobotSensor::OnBLight(bool on)
{
   OpState  state;
  state.blight_value=htons(on?1:0);
    CIOBuffer buffer;
    memcpy(buffer.Buffer,&state,sizeof(state));
    buffer.Length=sizeof(state);
    _socket->Write(&buffer);
  /*
    modbus_wtu data;
    data.RegAddr[1]=134;
    
    data.ValueCount=2;
    data.RegValue[1]=on?1:0;
    data.crc=CRC16((uint8_t *)&data,sizeof(modbus_wtu)-2);
    CIOBuffer buffer;
    memcpy(buffer.Buffer,&data,sizeof(data));
    buffer.Length=sizeof(data);
   // _socket->Write(&buffer);
   */
}
void CRobotSensor::OnWiper(bool on)
{
   OpState  state;
  state.wip_value=htons(on?1:0);
    CIOBuffer buffer;
    memcpy(buffer.Buffer,&state,sizeof(state));
    buffer.Length=sizeof(state);
    _socket->Write(&buffer);
  /*
    modbus_wtu data;
    data.RegAddr[1]=132;
    
    data.ValueCount=2;
    data.RegValue[1]=on?1:0;
    data.crc=CRC16((uint8_t *)&data,sizeof(modbus_wtu)-2);
    CIOBuffer buffer;
    memcpy(buffer.Buffer,&data,sizeof(data));
    buffer.Length=sizeof(data);
    _socket->Write(&buffer);
    */
}
bool CRobotSensor::OnStartup() 
{
  /*

  if(!_bootstrap) return true;
   
   CIOBuffer buffer;
    
    modbus_wtu data;
    data.RegAddr[1]=102;
    
    data.ValueCount=2;
    data.RegValue[1]=1;
    data.crc=CRC16((uint8_t *)&data,sizeof(modbus_wtu)-2);
   
    memcpy(buffer.Buffer,&data,sizeof(data));
    buffer.Length=sizeof(data);
    _socket->Write(&buffer);
    */
   /*
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));	
   int32_t count=0;
    modbus_rtu rtu;
    rtu.Addr=0x03;
    rtu.Func=0x03;
    rtu.RegAddr[1]=203;
    rtu.crc=CRC16((uint8_t *)&rtu,sizeof(modbus_rtu)-2);
    memcpy(buffer.Buffer,&rtu,sizeof(rtu));
    buffer.Length=sizeof(modbus_rtu);
    while(!_startup&&count<30)
    {
       std::this_thread::sleep_for(std::chrono::milliseconds(100));	
     
      _socket->Write(&buffer);
      count++;
    }
    */
    /* RemoNet::CCStartupRep Rep;
     Rep.set_ret(_startup);
     CIOBuffer Buffer;
     MessageHead Head;
     Head.Command = RemoNet::CC_StartupRep;
     Head.Length = Rep.ByteSizeLong();
     Head.Serialize(Buffer.Buffer);
     auto ptr = Buffer.Buffer + MessageHead::Size();
     Rep.SerializeToArray(ptr, Head.Length);
     Buffer.Length = Head.Length + MessageHead::Size();
     _message->WritePacket(ChannelType::CHANNEL_CAR, &Buffer);*/
  return _startup;
}
/*
void CRobotSensor::OnGears(int16_t value)
{
      OpState  state;
     state.gear_value=htons(value);
    CIOBuffer buffer;
    memcpy(buffer.Buffer,&state,sizeof(state));
    buffer.Length=sizeof(state);
    _socket->Write(&buffer);
}
*/
void CRobotSensor::OnPark(bool on)
{
    OpState  state;
     state.park_value=htons(on?1:0);
    CIOBuffer buffer;
    memcpy(buffer.Buffer,&state,sizeof(state));
    buffer.Length=sizeof(state);
    _socket->Write(&buffer);
}
void CRobotSensor::OnHangup()
{
  /*
    CIOBuffer buffer;
    
    modbus_wtu data;
    data.RegAddr[1]=103;
    
     
   
    data.crc=CRC16((uint8_t *)&data,sizeof(modbus_wtu)-2);
   
    memcpy(buffer.Buffer,&data,sizeof(data));
    buffer.Length=sizeof(data);
    _socket->Write(&buffer);
    modbus_rtu rtu;
    rtu.RegAddr[1]=205;  //停止
    rtu.crc=CRC16((uint8_t *)&rtu,sizeof(modbus_rtu)-2);
    memcpy(buffer.Buffer,&rtu,sizeof(rtu));
    buffer.Length=sizeof(modbus_rtu);
    _socket->Write(&buffer);
   
   _startup=false;
   */
}
void CRobotSensor::Run()
{
  return;
  _run=true;
 /*
  while(_run)
  {
      std::this_thread::yield();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      if(_startup&&!_emergency)
      {
          CIOBuffer buffer;
          int32_t count=0;
        
            modbus_rtu rtu;
          rtu.Addr=0x03;
          rtu.Func=0x03;
          rtu.RegAddr[1]=207; //急停
          rtu.crc=CRC16((uint8_t *)&rtu,sizeof(modbus_rtu)-2);
          memcpy(buffer.Buffer,&rtu,sizeof(rtu));
          buffer.Length=sizeof(modbus_rtu);
          _socket->Write(&buffer);
      }


  }
  */
}
bool CRobotSensor::Analog(int16_t  steer,int16_t arm,int16_t bucket,int16_t throttle,int16_t brake)
{
 // uint16_t tab_reg1[4] = { 0 };
 // int	regs1 = modbus_read_registers(_ctx, 202, 4, tab_reg1);
    std::cout<<"steer :"<<steer<<", arm :"<<arm<<",bucket:"<<bucket<<",throttle:"<<throttle<<",brake:"<<brake<<std::endl;
    //return true;
    if(_autorise==false)
     {

         CIOBuffer buffer;
          int32_t count=0;
          Analog_rtu rtu;
          rtu.arm_value=htons(arm);
          rtu.bucket_value=htons(bucket);
          rtu.steer_value=htons(steer);
          rtu.brake_value=htons(brake);
          rtu.throttle_value=htons(throttle);
          /*
          rtu.Value[0]= HIBYTE(throttle);
          rtu.Value[1]=LOBYTE(throttle);
          rtu.Value[2]=0;
          rtu.Value[3]=0;
          rtu.Value[4]=HIBYTE(steer);
          rtu.Value[5]=LOBYTE(steer) ;
          rtu.Value[6]=0;
          rtu.Value[7]=0;
          rtu.Value[8]=HIBYTE(brake);
          rtu.Value[9]=LOBYTE(brake);
          rtu.Value[10]=0;
          rtu.Value[11]=0;
          rtu.Value[12]=HIBYTE(arm);
          rtu.Value[13]=LOBYTE(arm);
          rtu.Value[14]=0;
          rtu.Value[15]=0;
          rtu.Value[16]=HIBYTE(bucket);
          rtu.Value[17]=LOBYTE(bucket);
          rtu.Value[18]=0;
          rtu.Value[19]=0;
          
          rtu.crc=CRC16((uint8_t *)&rtu,sizeof(rtu)-2);
          */
          memcpy(buffer.Buffer,&rtu,sizeof(rtu));
          buffer.Length=sizeof(rtu);
          _socket->Write(&buffer);
           //for(int i=0;i< buffer.Length;i++)
     //{
     //   int32_t temp=(int32_t)(buffer.Buffer[i]&0xFF);
     //   std::cout<<std::hex<<temp<<",";
   //  }
    // std::cout<<std::endl;
     /*     modbus_wtu_qs  qs;
          qs.Value[1]=1;
          qs.Value[3]=1;
          qs.Value[5]=1;
          qs.Value[7]=1;
          qs.Value[9]=1;
          qs.crc=CRC16((uint8_t *)&qs,sizeof(qs)-2);
          memcpy(buffer.Buffer,&rtu,sizeof(rtu));
          buffer.Length=sizeof(qs);
          _socket->Write(&buffer);
     */
        

     }
     return true;   
   

}
void CRobotSensor::OnGears(int16_t value)
{
  OpState  state;
  state.gear_value=htons(value);
    CIOBuffer buffer;
    memcpy(buffer.Buffer,&state,sizeof(state));
    buffer.Length=sizeof(state);
    _socket->Write(&buffer);
  /*
    modbus_wtu data;
    data.RegAddr[1]=101;
   // data.RegCount=1;
    data.ValueCount=2;
    data.RegValue[1]=1;
   */
   /*
   modbus_wtu data;
   //memset(&data,0,sizeof(data));
   
   data.RegAddr[1]=130;
   data.ValueCount=2;
   data.RegValue[1]=value;
   data.crc=CRC16((uint8_t *)&data,sizeof(modbus_wtu)-2);
   
*/

}
void CRobotSensor::OnAnalog(RemoNet::CCRobotAnalog& req)
{
   
     
     if(_autorise==true)
     {
        // uint16_t tab_reg1[4] = { 0 };
      /* int	regs1 = modbus_read_registers(_ctx, 205, 4, tab_reg1);

         if(tab_reg1[1]==1)
         {
          _autorise=false;
         }
         else 
           {
            return;
           }
           */
           return;
     }
     int32_t brake=req.brake();
     int32_t throttle=req.throttle();
     int32_t steer=req.steer();
     int32_t arm=req.arm();
     int32_t bucket=req.bucket();
     RemoNet::Gears g=req.gears();
     bool front=req.frontlight();
     bool back=req.backlight();
     bool wipe=req.wipe();
     bool park=req.emergency();
     
    // if(_bootstrap)
    {
      
       if(_gears!=g)
        {
            int8_t value=1;
            if(g==RemoNet::Gears::D)
            {
               value=2;
               _message->SwitchCamera(true);
            }
            else if(g==RemoNet::R)
            {
              _message->SwitchCamera(false);
              value=3;
            }
            else if(g==RemoNet::Gears::N)
            {
              _message->SwitchCamera(true);
            }
            OnGears(value);
            _gears=g;
       }
       else if(_frontLight!=front)
       {
         _frontLight=front;
         OnChangeState();
        //  OnFLight(front);
         
       }
       else if(_backLight!=back)
       {
          _backLight=back;
          OnChangeState();
        //  OnBLight(back);
        
       }
       else if(_wipe!=wipe)
       {
          _wipe=wipe;
          OnChangeState();
          // OnWiper(wipe);
         
       }
       else if(_park!=park)
       {
        _park=park;
        OnPark(park);
       }
       else{
         Analog(steer,arm,bucket,throttle,brake);
       }
       
    }
   
     
     
 
}

void CRobotSensor::SetSensorSocket(SensorTCP<CRobotSensor>* can)
{
  _socket=can;
}