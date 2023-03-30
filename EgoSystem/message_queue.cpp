#include <atomic>
#include <string>
#include "../common/comm.h"
#include "api.h"
#include "../common/iobuffer.h"
#include "../common/sensor_socket.h"
#include "../common/peer_connection.h"
#include "VideoRenderer.h"
#include <jsoncpp/json/json.h>
#include "protocol.pb.h"
#include "radar_sensor.h"
#include "imu_sensor.h"
#include "robot_sensor.h"

#include "message_queue.h"
#include <fstream>
#include <iostream>
#include <cstring>
 
CMessageQueue::CMessageQueue():Head(nullptr),Tail(nullptr)
{
   _peerId=-1;
  
}
CMessageQueue::~CMessageQueue()
{

}
void CMessageQueue::Create()
{
  
    Json::Value root;
	Json::Reader jsonReader;
    std::ifstream ifile("./config.json");
    std::string serverip;
    _curTick=0;
    bStopedCar=false;
    if(jsonReader.parse(ifile,root))
    {
        std::cout<<"enter config json"<<std::endl;
        _serial=root["serial"].asString();
         
       int32_t _imuport=root["imu_port"].asInt();
       int32_t _imuhost=root["imu_host"].asInt();
       std::string _imuip=root["imu_ip"].asString();
       std::cout<<"imu ip:"<<_imuip<<std::endl;
       _imu=std::make_unique<SensorSocket<CImuSensor>>(this,_imuip,_imuport,_imuhost);
      // _imu->Start(); 
       int32_t _radarport=root["radar_port"].asInt();
       int32_t _radarhost=root["radar_host"].asInt();
       std::string _radarip=root["radar_ip"].asString();
       std::cout<<"radar ip:"<<_radarip<<std::endl;
       _radar=std::make_unique<SensorSocket<CRadarSensor>>(this,_radarip,_radarport,_radarhost);

       //20230328
       //_radar->Start();
       

       int32_t _robotport=root["robot_port"].asInt();
       int32_t _robothost=root["robot_host"].asInt();
       std::string _robotip=root["robot_ip"].asString();
       std::cout<<"robot ip:"<<_robotip<<std::endl;
       _robot=std::make_unique<SensorTCP<CRobotSensor>>(this,_robotip,_robotport,_robothost);
       // _robot->Start();
      // _robot->Get()->Analog(1,2,3,4,5);
     //  _robot->Get()->Emergency();
           
    
       // std::this_thread::sleep_for(std::chrono::milliseconds(300));
      
       /*
       _robot->Get()->OnStartup();\
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
       while (true)
       {
         std::this_thread::sleep_for(std::chrono::milliseconds(30));	
         _robot->Get()->Analog(5,8,7,8,9);
       }
         
       */
        
        serverip=root["ip"].asString();
        _name=root["name"].asString();
        const Json::Value arrayObj=root["camerainfo"];
        int32_t count=arrayObj.size();
        for(int32_t i=0;i<count;i++)
        {
                LocalCameraInfo info;
                const Json::Value& value=arrayObj[i];
                info.index=value["index"].asInt();
                info.label=value["label"].asString();
                _cameraArray.push_back(info);
        }
        for(int i=0;i<_cameraArray.size();i++)
            {
                _peerArray.push_back({nullptr});
        //      _windowArray.push_back({nullptr});
            }
        
       
      // _peerArray.reserve(_cameraArray.size());
       //_windowArray.reserve(_cameraArray.size());
       
    }
    else{
        std::string error=jsonReader.getFormattedErrorMessages();
        std::cout<<error<<std::endl;
    }
    _client=std::make_unique<SocketClient>(this);
    _client->Start(serverip.c_str());
   
   
  
    //_can=std::make_unique<SocketCan>(this);
    //_can->Start(_canip,_canport,_hostport);
      
    
  
    std::this_thread::yield();
 //    OnNotifyReq(0);

}
/*
void CMessageQueue::WriteCanMessage(std::unordered_map<int32_t, cannet_frame>& node,bool islidar)
{
    if(!bDataChannelCreated) return;
     // std::lock_guard<std::mutex> l(_canLock);
      RemoNet::CCCanMesage Req;
      Req.set_islidar(islidar);
      for(auto& p:node)
      {
         int32_t lidar=p.second.canid;
         auto m=Req.add_message();
         m->set_head(p.second.dlc);
         m->set_canid(lidar);
         m->set_data(p.second.data,8);
        
      }
      
      MessageHead Head;
      CIOBuffer pBuffer;
      Head.Command = RemoNet::CC_CAN;
      Head.Length = Req.ByteSizeLong();
      Head.Serialize(pBuffer.Buffer);
      auto ptr = pBuffer.Buffer + MessageHead::Size();
      Req.SerializeToArray(ptr, Head.Length);
      pBuffer.Length = MessageHead::Size() + Head.Length;
      _peerArray[0]->SendData(&pBuffer);
}

bool CMessageQueue::IsCarId(int32_t value)
{
    return std::find(_carArray.begin(),_carArray.end(),value)!=_carArray.end();
}
*/
void CMessageQueue::EnQueue(CIOBuffer* pBuffer)
{
    bool bNullBuffer=false;
    std::unique_lock <std::mutex>  lck(_lock);
    if(Head==nullptr)
    {
        Head=Tail=pBuffer;
        bNullBuffer=true;
    }
    else{
        Tail->NextBuf=pBuffer;
        Tail=Tail->NextBuf;
    } 
    pBuffer->NextBuf=nullptr;
    if(bNullBuffer)
    {
        _cv.notify_one();
    }
}  
void CMessageQueue::Process()
{
    CIOBuffer * ptr=nullptr;
    {
        std::unique_lock <std::mutex>  lck(_lock);
        /*
        while(Head==nullptr)
        {
            _cv.wait(lck);
        }
        */
        while(Head==nullptr&&_cv.wait_for(lck,std::chrono::seconds(1))==std::cv_status::timeout)
        {
          CheckSignal();
         // std::cout<<".";
         // std::cout.flush();
          
        }
        
    }
    while(Head!=nullptr)
    {
        ptr=Head;
        Head=Head->NextBuf;
        if(ptr!=nullptr)
        {
            Message* message=reinterpret_cast<Message *>(ptr->Buffer);
            switch (message->cmd)
            {
            case MessageType::ReqVideo:
            OnNotifyReq((int32_t)message->param_l);
            break;
            case MessageType::RepVideo:
            OnNotifyRep((int32_t)message->param_l);
            break;
            case MessageType::Connected:
            OnNotifyConnected((bool)message->param_l);
            break;
            case MessageType::Leave:
            OnNotifyLeave();
            break;
            case MessageType::AsyncMessage:
            OnNotifyMessage();
            break;
            case MessageType::StopSensor:
            OnNotifyStopSensor();
            break;        
            case MessageType::Ping:
            OnNotifyPing(message->param_l);
            break; 
            case MessageType::BootStrap:
             {
                 _robot->Get()->OnBootstrap();
             }
             break;
            case MessageType::Startup:
            {
                _robot->Get()->OnStartup();
            }
            break;
            }
            ptr->Release();
        }   
    }    
}
void CMessageQueue::SetTick(long long tick)
{
    _curTick=tick;
}
void CMessageQueue::OnNotifyConnected(bool bRet)
{
    if(bRet)
    {
        
        _client->WriteAddRobot(_serial,_name,static_cast<int32_t>(EgoType::Car));
         _updatethread.start(_client.get());
    }
    else
    {
        if(_peerId!=-1)
        {
            OnVideoLeave(_peerId,EgoType::User);
          /*  for (size_t i = 0; i < _peerArray.size(); i++)
            {
                if(_peerArray[i]!=nullptr)
                {
                    _peerArray[i]->Close();
                    _peerArray[i].reset();
                }
               
            }
            */
         //   for (size_t i = 0; i < _windowArray.size(); i++)
         //   {
                /* code */
         //       if(_windowArray[i]!=nullptr)
         //         _windowArray[i].reset();
         //   }
            _peerId=-1;
//            StopCar();
        }
      
        _updatethread.stop();
    }
}
void CMessageQueue::OnNotifyReq(int32_t index)
{ 
   // std::cout<<__FUNCTION__<<","<<__LINE__<<std::endl;
   if(_peerId==-1) return;
     if(index==0)
       {
          mrsWebrtcCreateFactory(true);
          _curTick=0;
          bStopedCar=false;
         _imu->Start();
          _radar->Start(); 
          _robot->Start();
        
       }
    
   
//	PeerConnectionWrapper* peer = nullptr;
//	VideoRenderer* window = nullptr;
   // _windowArray[temp]=std::make_unique<VideoRenderer>();// .reset(new VideoRenderer());
    _peerArray[index]=std::make_unique<CPeerConnection>(static_cast<ChannelType>(index),_client.get());
   
	
	_client->WriteVideoRep(_peerId, RemoNet::VideoDesc::OK, index);
}
void CMessageQueue::OnNotifyRep(int32_t index)
{
   // std::cout<<__FUNCTION__<<","<<__LINE__<<std::endl;
    
	 
   // index-=_indexOffset;
//	PeerConnectionWrapper* peer = nullptr;
//	VideoRenderer* window = nullptr;
    //_windowArray[temp]=std::make_unique<VideoRenderer>();
    _peerArray[index]=std::make_unique<CPeerConnection>(static_cast<ChannelType>(index), _client.get());
    
	 
	InitPeerConnection(_peerId,index);
	_peerArray[index]->CreateOffer();
	
}
void CMessageQueue::InitPeerConnection(int32_t peer,int32_t index)
{
	 
    //bool NeedData=index==RenderPosition::CAR_FRONT;
   
	_peerArray[index]->Initialize(peer,index);
    _peerArray[index]->AddDataChannel(true, false);
  	
//	window.reset(new VideoRenderer());
//	window->SetRenderWindow(GetDlgItem(IDC_REMOTE), 1, 1);
	//_peerArray[index]->AddLocalArgb32VideoFrameReady(&VideoRenderer::FrameCallback, _windowArray[index].get());
   // if(_cameraArray[index].type==CaptureType::RTSP)
    //    {
    
   
    _peerArray[index]->AddLocalVideoTrack(static_cast<RenderPosition>(index),_cameraArray[index].index);
    //    }
   // else
    //  {
     //      _peerArray[index]->AddLocalVideoTrack(_cameraArray[index].type, _cameraArray[index].solution, 30);   
    //  } 
     if(index==RenderPosition::BACK)
    {
        void * front=_peerArray[RenderPosition::FRONT]->GetCurrentCtx();
        while(front==nullptr)
        {
            std::cout<<"front==nullptr"<<std::endl;
             std::this_thread::sleep_for(std::chrono::microseconds(50));  
             front=_peerArray[RenderPosition::FRONT]->GetCurrentCtx();
        }
        void * back=_peerArray[RenderPosition::BACK]->GetCurrentCtx();
        while (back==nullptr)
        {
            /* code */
             std::cout<<"back==nullptr"<<std::endl;
            std::this_thread::sleep_for(std::chrono::microseconds(50));  
            back=_peerArray[RenderPosition::BACK]->GetCurrentCtx();
        }
        
       // assert(front!=nullptr);
       // assert(back!=nullptr);
        _peerArray[RenderPosition::FRONT]->SetOtherCtx(back);
        _peerArray[RenderPosition::BACK]->SetOtherCtx(front);
    }
    if(index==RenderPosition::FRONT)
	    _peerArray[index]->AddLocalAudioTrack();
	//[temp]->StartRender(true);
}
 
void CMessageQueue::OnAdd(bool bRet)
{

}
void CMessageQueue::OnConnected(bool bRet)
{
    
    CIOBuffer * pBuffer=CIOBuffer::Alloc();
    Message* message=reinterpret_cast<Message *>(pBuffer->Buffer);
    message->cmd=MessageType::Connected;
    message->param_l=bRet;
    EnQueue(pBuffer);
   
}
 
void CMessageQueue::OnVideoLeave(int32_t peer,EgoType type)
{
    {
         CIOBuffer * pBuffer=CIOBuffer::Alloc();
        Message* message=reinterpret_cast<Message *>(pBuffer->Buffer);
        message->cmd=MessageType::StopSensor;
        EnQueue(pBuffer);
    }
    {
        CIOBuffer * pBuffer=CIOBuffer::Alloc();
        Message* message=reinterpret_cast<Message *>(pBuffer->Buffer);
        message->cmd=MessageType::Leave;
     
        EnQueue(pBuffer);
    }
}
#ifdef WIN32
void CMessageQueue::OnVideoRep(int32_t index,RemoNet::VideoDesc desc)
{
   if (desc == RemoNet::VideoDesc::OK)
   {
        assert(_peerId!=-1);
        CIOBuffer * pBuffer=CIOBuffer::Alloc();
        Message* message=reinterpret_cast<Message *>(pBuffer->Buffer);
        message->cmd=MessageType::ReqVideo;
        message->param_l=index;
        
        EnQueue(pBuffer);
   }

}
#else
void CMessageQueue::OnVideoReq(int32_t video,int32_t peer)
{
   // std::cout<<__FUNCTION__<<","<<__LINE__<<std::endl;
    _peerId=peer;
    CIOBuffer * pBuffer=CIOBuffer::Alloc();
    Message* message=reinterpret_cast<Message *>(pBuffer->Buffer);
    message->cmd=MessageType::ReqVideo;
    message->param_l=video;
    
    EnQueue(pBuffer);
    
}
#endif
void CMessageQueue::OnNotifyLeave()
{
  
  
     std::this_thread::sleep_for(std::chrono::milliseconds(100));   
   
	 
     for (size_t i = 0; i < _peerArray.size(); i++)
     {
         if(_peerArray[i]!=nullptr)
         {
             _peerArray[i]->Close();
             _peerArray[i].reset();
         }
          /* code */
     }
     //for (size_t i = 0; i < _windowArray.size(); i++)
    // {
          /* code */
    //     _windowArray[i].reset();
   //  }
      _peerId=-1;
      
      //StopCar();

      
      
}
void CMessageQueue::OnNotifyStopSensor()
{
      std::cout<<"radar stop"<<std::endl;
    _radar->Stop(); //装好后恢复
      std::cout<<"robot stop"<<std::endl;
    _curTick=0;
    _robot->Stop();
    std::cout<<"imu stop"<<std::endl;
    _imu->Stop();

     RemoNet::StopAck Rep;
    
     CIOBuffer Buffer;
     MessageHead Head;
     Head.Command = RemoNet::CC_StopACK;
     Head.Length = Rep.ByteSizeLong();
     Head.Serialize(Buffer.Buffer);
     auto ptr = Buffer.Buffer + MessageHead::Size();
     Rep.SerializeToArray(ptr, Head.Length);
     Buffer.Length = Head.Length + MessageHead::Size();
     _peerArray[RenderPosition::FRONT]->SendData(&Buffer);


}
void CMessageQueue::OnVideoOffer(int32_t index,const char* type, const char* sdp)
{ 
 //   std::cout<<__FUNCTION__<<","<<__LINE__<<std::endl;
    InitPeerConnection(_peerId,index); 
    
    _peerArray[index]->SetRemoteDescription(type,sdp);
    _peerArray[index]->CreateAnswer();
     
}
void CMessageQueue::OnVideoAnswer(int32_t index, const char* type, const char* sdp)
{ 
  //  std::cout<<__FUNCTION__<<","<<__LINE__<<std::endl;
      
     _peerArray[index]->SetRemoteDescription(type,sdp);
    
}
void CMessageQueue::OnVideoCandidate(int32_t index,const char* candidate,
		int32_t sdp_mline_index,
		const char* sdp_mid)
{ 
      
    _peerArray[index]->AddIceCandidate(candidate,sdp_mline_index,sdp_mid);
     
}
/*
void CMessageQueue::SwitchCamera(bool front)
{
     _peerArray[RenderPosition::FRONT_BACK]->SwitchCapture(front);
}
*/
void CMessageQueue::OnMessageFrameNotify(ChannelType type,int16_t cmd,int16_t length,const void * data)
{
     
    // std::cout<<"cmd:" <<std::hex<<cmd<<std::endl;
     if(cmd==RemoNet::CC_Text)
     {
          RemoNet::TestTextReq Req;
       
          Req.ParseFromArray(data,length);
          std::cout<<Req.text()<<std::endl;
         CIOBuffer * pBuffer=CIOBuffer::Alloc();
        Message* message=reinterpret_cast<Message *>(pBuffer->Buffer);
        message->cmd=MessageType::AsyncMessage;
        EnQueue(pBuffer);
    
     }
     else if(cmd==RemoNet::CC_Switch)
     {
         RemoNet::CCSwitch Req;
         Req.ParseFromArray(data,length);
         bool front=Req.front();
         _peerArray[RenderPosition::FRONT]->SwitchCapture(front);

     }
     else if(cmd==RemoNet::CC_Ping)
     {
           RemoNet::CCPing Req;
           Req.ParseFromArray(data,length);
          

           CIOBuffer * pBuffer=CIOBuffer::Alloc();
           Message* message=reinterpret_cast<Message *>(pBuffer->Buffer);
           message->cmd=MessageType::Ping;
           message->param_l=Req.tick();
           EnQueue(pBuffer);
    

     }
     else if(cmd==RemoNet::CC_SensorStop)
     {

        CIOBuffer * pBuffer=CIOBuffer::Alloc();
        Message* message=reinterpret_cast<Message *>(pBuffer->Buffer);
        message->cmd=MessageType::StopSensor;
        EnQueue(pBuffer);
     }
     else if(cmd==RemoNet::CC_RobotAnalog)
     {
        _curTick=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
          RemoNet::CCRobotAnalog Req;
          Req.ParseFromArray(data,length);
          _robot->Get()->OnAnalog(Req);
     }
     /*
     else if(cmd==RemoNet::CC_BootReq)
     {
         CIOBuffer * pBuffer=CIOBuffer::Alloc();
         Message* message=reinterpret_cast<Message *>(pBuffer->Buffer);
         message->cmd=MessageType::BootStrap;
         EnQueue(pBuffer);
     }
     else if(cmd==RemoNet::CC_StartupReq)
     {
         CIOBuffer * pBuffer=CIOBuffer::Alloc();
         Message* message=reinterpret_cast<Message *>(pBuffer->Buffer);
         message->cmd=MessageType::Startup;
         EnQueue(pBuffer);
        
     }
     */
   
     
}
 
void CMessageQueue::StopCar()
{
    std::cout<<"Stop Car"<<std::endl;
    //_robot->Get()->Emergency();
}
 
void CMessageQueue::OnNotifyMessage()
{
         RemoNet::TestTextReq Req;
         Req.set_text("ewqewqewqe");
         CIOBuffer Buffer;
          MessageHead Head;
          Head.Command = RemoNet::CC_Text;
          Head.Length = Req.ByteSizeLong();
          Head.Serialize(Buffer.Buffer);
          auto ptr = Buffer.Buffer + MessageHead::Size();
          Req.SerializeToArray(ptr, Head.Length);
          Buffer.Length = Head.Length + MessageHead::Size();
         _peerArray[RenderPosition::FRONT]->SendData(&Buffer);
         
}
void CMessageQueue::OnNotifyPing(int64_t value)
{
     RemoNet::CCPing Rep;
     Rep.set_tick(value);
     CIOBuffer Buffer;
     MessageHead Head;
     Head.Command = RemoNet::CC_Ping;
     Head.Length = Rep.ByteSizeLong();
     Head.Serialize(Buffer.Buffer);
     auto ptr = Buffer.Buffer + MessageHead::Size();
     Rep.SerializeToArray(ptr, Head.Length);
     Buffer.Length = Head.Length + MessageHead::Size();
    _peerArray[RenderPosition::FRONT]->SendData(&Buffer);
}

/*
void CMessageQueue::StartCar()
{
    _can->SetStartWrite(true);
}
*/
void CMessageQueue::CheckSignal()
{
     
    if(!bStopedCar)
    {
        long long tick=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if(_curTick!=0&&tick-_curTick>1000) 
        {
            StopCar();
            bStopedCar=true;
        }
    }
     
}
void CMessageQueue::WriteIMUData(ImuData* data)
{
     
    MessageHead Head;
    CIOBuffer Buffer;
       
    RemoNet::IMuMessage Req;
  
   
    Req.set_rx(data->ry);
    Req.set_ry(data->rx);
   // Req.set_rz(data->rz);
 
    Head.Command=RemoNet::CC_IMU;
    Head.Length=Req.ByteSizeLong();
    Head.Serialize(Buffer.Buffer);
    auto ptr = Buffer.Buffer + MessageHead::Size();
    Req.SerializeToArray(ptr, Head.Length);
    Buffer.Length = Head.Length + MessageHead::Size();
    
    if( _peerArray[ChannelType::CHANNEL_IMU]!=nullptr)
        _peerArray[ChannelType::CHANNEL_IMU]->SendData(&Buffer);
    
  
}
void CMessageQueue::WritePacket(ChannelType type, CIOBuffer * pBuffer)
{
     if( _peerArray[type]!=nullptr)
        _peerArray[type]->SendData(pBuffer);
}
void CMessageQueue::WriteRadarData(RadarData& data)
{
      MessageHead Head;
      CIOBuffer Buffer;
      RemoNet::CCRadarMessage Req;
      Req.set_radar0(data.r0);
      Req.set_radar1(data.r1);
      Req.set_radar2(data.r2);
      Req.set_radar3(data.r3);
      Req.set_radar4(data.r4);
      Req.set_radar5(data.r5);
      Head.Command=RemoNet::CC_IMU;
      Head.Length=Req.ByteSizeLong();
      Head.Serialize(Buffer.Buffer);
      auto ptr = Buffer.Buffer + MessageHead::Size();
      Req.SerializeToArray(ptr, Head.Length);
      Buffer.Length = Head.Length + MessageHead::Size();
      if( _peerArray[ChannelType::CHANNEL_RADAR]!=nullptr)
         _peerArray[ChannelType::CHANNEL_RADAR]->SendData(&Buffer);
       
}

void CMessageQueue::WriteRobotStatus(int32_t ,int32_t )
{
    
}
#ifdef LIDAR_SENSOR
void CMessageQueue::WriteLidarPoint(const PointCloudMsg<PointXYZI>& msg,ChannelType side)
{
    RemoNet::LidarPoint pt;
    pt.set_is_left(side==ChannelType::CHANNEL_LEFT_LIDAR);
    pt.set_frame_id(msg.frame_id);
    pt.set_height(msg.height);
    pt.set_width(msg.width);
    pt.set_is_dense(msg.is_dense);
    pt.set_seq(msg.seq);
    pt.set_timestamp(msg.timestamp);
    for(int i=0;i<msg.point_cloud_ptr->size();i++)
    {
        pt.add_data((*msg.point_cloud_ptr)[i].x);
        pt.add_data((*msg.point_cloud_ptr)[i].y);
        pt.add_data((*msg.point_cloud_ptr)[i].z);
        pt.add_data((*msg.point_cloud_ptr)[i].intensity);
    }
    MessageHead Head;
    CIOBuffer Buffer;
    Head.Command=RemoNet::CC_LIDARDATA;
    Head.Length=pt.ByteSizeLong();
    Head.Serialize(Buffer.Buffer);
    auto ptr = Buffer.Buffer + MessageHead::Size();
    pt.SerializeToArray(ptr, Head.Length);
    Buffer.Length = Head.Length + MessageHead::Size();
    
    if( _peerArray[side]!=nullptr)
        _peerArray[side]->SendData(&Buffer);
}
#endif
void CMessageQueue::SwitchCamera(bool front)
{
     
      _peerArray[RenderPosition::FRONT]->SwitchCapture(front);
      _peerArray[RenderPosition::BACK]->SwitchCapture(front);
}