#include <atomic>
#include <string>
#include "../common/comm.h"
#include "api.h"
#include "message_queue.h"
#include "../common/iobuffer.h"
#include "PeerConnection.h"
#include "VideoRenderer.h"
#include <jsoncpp/json/json.h>
#include "Protocol.pb.h"
#include <fstream>
#include <cstring>
CMessageQueue::CMessageQueue():Head(nullptr),Tail(nullptr)
{
   _peerId=-1;
   bDataChannelCreated=false;
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
    

    if(jsonReader.parse(ifile,root))
    {
        std::cout<<"enter config json"<<std::endl;
      _serial=root["serial"].asString();
      _egoType=root["type"].asString()=="cave"?EgoType::Cave:EgoType::Car;
      if(_egoType==EgoType::Car)
      {
          _canport=root["can_port"].asInt();
          _hostport=root["host_port"].asInt();
          _canip=root["can_ip"].asString();
          _lidarport=root["lidar_port"].asInt();
      }
      serverip=root["ip"].asString();
      _name=root["name"].asString();
      _indexOffset=_egoType==EgoType::Cave?0:2;
       const Json::Value arrayObj=root["camerainfo"];
       int32_t count=arrayObj.size();
       
       for(int32_t i=0;i<count;i++)
       {
           LocalCameraInfo info;
           const Json::Value& value=arrayObj[i];
           info.index=value["index"].asInt();
           info.label=value["label"].asString();
           info.uri=value["uri"].asString();
         
          // if(_rtspCamera)
          //    info.type=CaptureType::RTSP;
          /*
           else
           {
              if(info.uri.find("zed")!=info.uri.npos)
              {
                  info.type=CaptureType::Zed2;
              }    
              else if(info.uri.find("realsense")!=std::string::npos)
              {
                  info.type=CaptureType::RealSense;
              }
           }
           */
         //  info.master=value["master"].asBool();
         std::string hd=value["res"].asString();
         if(hd=="hd1080") info.solution=DisplayResolution::HD1080;
         else if(hd=="hd720") info.solution=DisplayResolution::HD720;

           _cameraArray.push_back(info);
       }
       for(int i=0;i<_cameraArray.size();i++)
        {
            _peerArray.push_back({nullptr});
            _windowArray.push_back({nullptr});
        }
        const Json::Value lidars=root["lidar"];
        count=lidars.size();
        for(int32_t i=0;i<count;i++)
        {
            int32_t value=lidars[i].asInt();
            _lidarArray.push_back(value);
        }
        const Json::Value emergency=root["emergency"];
        count=emergency.size();
        for(int32_t i=0;i<count;i++)
        {
            std::string str=emergency[i].asString();
            int32_t value=htonl(strtol(str.c_str(),nullptr,16));
            _emergencyArray.push_back(value);
        }
        const Json::Value carId=root["car_id"];
        count=carId.size();
        for(int32_t i=0;i<count;i++)
        {
            std::string str = carId[i].asString();
            int32_t value = htonl(strtol(str.c_str(), nullptr, 16));
            // int32_t value=carId[i].asInt();
            _carArray.push_back(value);
        }
      // _peerArray.reserve(_cameraArray.size());
       //_windowArray.reserve(_cameraArray.size());
       
    }
    _client=std::make_unique<SocketClient>(this);
    _client->Start(serverip.c_str());
   
    if(_egoType==EgoType::Car)
   {
      _can=std::make_unique<SocketCan>(this);
      _can->Start(_canip,_canport,_hostport);
      _lidar=std::make_unique<SocketLidar>(this);
      _lidar->Start(_lidarport);
   } 
  
    std::this_thread::yield();
 //    OnNotifyReq(0);

}
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
        //  CheckSignal();
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
            }
            ptr->Release();
        }   
    }    
}
void CMessageQueue::OnNotifyConnected(bool bRet)
{
    if(bRet)
    {
         CameraInfo  info[4];//=(CameraInfo *)alloca(sizeof(CameraInfo)*_cameraArray.size());
         for(auto i=0;i<_cameraArray.size();i++)
        {
            info[i].index= _cameraArray[i].index;
            info[i].label=_cameraArray[i].label;
            info[i].solution=_cameraArray[i].solution;
        }
        _client->WriteAddRobot(_serial,_name,static_cast<int32_t>(_egoType),_cameraArray.size(), info);
         _updatethread.start(_client.get());
    }
    else
    {
        if(_peerId!=-1)
        {
            for (size_t i = 0; i < _peerArray.size(); i++)
            {
                if(_peerArray[i]!=nullptr)
                {
                    _peerArray[i]->Close();
                    _peerArray[i].reset();
                }
                /* code */
            }
            for (size_t i = 0; i < _windowArray.size(); i++)
            {
                /* code */
                if(_windowArray[i]!=nullptr)
                  _windowArray[i].reset();
            }
            _peerId=-1;
            StopCar();
        }
      
        _updatethread.stop();
    }
}
void CMessageQueue::OnNotifyReq(int32_t index)
{ 
   // std::cout<<__FUNCTION__<<","<<__LINE__<<std::endl;
     if(index-_indexOffset==0)
         mrsWebrtcCreateFactory(true);
     auto temp=index-_indexOffset;
    int32_t width = _cameraArray[temp].solution == DisplayResolution::HD1080 ? 1920 : 1280;
	int32_t height = _cameraArray[temp].solution == DisplayResolution::HD1080 ? 1080 : 720;
   
//	PeerConnectionWrapper* peer = nullptr;
//	VideoRenderer* window = nullptr;
    _windowArray[temp]=std::make_unique<VideoRenderer>();// .reset(new VideoRenderer());
    _peerArray[temp]=std::make_unique<PeerConnectionWrapper>(_client.get());
    
    //InitPeerConnection(index);
	
	_client->WriteVideoRep(EgoType::User,_peerId, RemoNet::VideoDesc::OK, index);
}
void CMessageQueue::OnNotifyRep(int32_t index)
{
   // std::cout<<__FUNCTION__<<","<<__LINE__<<std::endl;
    auto temp=index-_indexOffset;
	int32_t width = _cameraArray[temp].solution == DisplayResolution::HD1080 ? 1920 : 1280;
	int32_t height = _cameraArray[temp].solution == DisplayResolution::HD1080 ? 1080 : 720;
   // index-=_indexOffset;
//	PeerConnectionWrapper* peer = nullptr;
//	VideoRenderer* window = nullptr;
    _windowArray[temp]=std::make_unique<VideoRenderer>();
    _peerArray[temp]=std::make_unique<PeerConnectionWrapper>(_client.get());
    
	 
	InitPeerConnection(_peerId,index);
	_peerArray[temp]->CreateOffer();
	
}
void CMessageQueue::InitPeerConnection(int32_t peer,int32_t index)
{
	 
    bool NeedData=index==CameraPosition::CAR_FRONT;
    auto temp=index-_indexOffset;
	_peerArray[temp]->Initialize(peer,index,NeedData);
    if(NeedData)
    {
        _peerArray[temp]->AddDataChannel(true, false);
      

    }	
//	window.reset(new VideoRenderer());
//	window->SetRenderWindow(GetDlgItem(IDC_REMOTE), 1, 1);
	//_peerArray[index]->AddLocalArgb32VideoFrameReady(&VideoRenderer::FrameCallback, _windowArray[index].get());
   // if(_cameraArray[index].type==CaptureType::RTSP)
    //    {
    int32_t width=_cameraArray[temp].solution==DisplayResolution::HD1080?1920:1280;
    int32_t height=_cameraArray[temp].solution==DisplayResolution::HD1080?1080:720;
    int32_t rotation=0;
    if(index==CameraPosition::CAR_LEFT)
     {  rotation=270; int32_t temp=width;width=height;height=temp;}
    else if(index==CameraPosition::CAR_RIGHT)
      { rotation=90;int32_t temp=width;width=height;height=temp;}
  
    _peerArray[temp]->AddLocalVideoTrack(rotation,_cameraArray[temp].label,_cameraArray[temp].uri,width,height);
    //    }
   // else
    //  {
     //      _peerArray[index]->AddLocalVideoTrack(_cameraArray[index].type, _cameraArray[index].solution, 30);   
    //  } 
	_peerArray[temp]->AddLocalAudioTrack();
	_windowArray[temp]->StartRender(true);
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
     CIOBuffer * pBuffer=CIOBuffer::Alloc();
    Message* message=reinterpret_cast<Message *>(pBuffer->Buffer);
    message->cmd=MessageType::Leave;
     
    EnQueue(pBuffer);
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
    
     bDataChannelCreated=false;
     
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
     for (size_t i = 0; i < _windowArray.size(); i++)
     {
          /* code */
         _windowArray[i].reset();
     }
      _peerId=-1;
      
      StopCar();

      
      
}
void CMessageQueue::OnVideoOffer(int32_t video,const char* type, const char* sdp)
{ 
 //   std::cout<<__FUNCTION__<<","<<__LINE__<<std::endl;
    InitPeerConnection(_peerId,video); 
    auto temp=video-_indexOffset;
    _peerArray[temp]->SetRemoteDescription(type,sdp);
    _peerArray[temp]->CreateAnswer();
     
}
void CMessageQueue::OnVideoAnswer(int32_t video, const char* type, const char* sdp)
{ 
  //  std::cout<<__FUNCTION__<<","<<__LINE__<<std::endl;
     auto temp=video-_indexOffset;
     _peerArray[temp]->SetRemoteDescription(type,sdp);
    
}
void CMessageQueue::OnVideoCandidate(int32_t video,const char* candidate,
		int32_t sdp_mline_index,
		const char* sdp_mid)
{ 
     auto temp=video-_indexOffset;
    _peerArray[temp]->AddIceCandidate(candidate,sdp_mline_index,sdp_mid);
     
}

void CMessageQueue::OnMessageFrameNotify(const void* data, const int32_t size)
{
     if(size<MessageHead::Size()) return;
     MessageHead Head;
     int8_t* Data=(int8_t *)data;
     Head.Deserialize(Data);
     if(size<MessageHead::Size()+Head.Length) return;
     auto ptr=Data+MessageHead::Size();
     
     if(Head.Command==RemoNet::CC_Text)
     {
          RemoNet::TestTextReq Req;
       
          Req.ParseFromArray(ptr,Head.Length);
          std::cout<<Req.text()<<std::endl;
         CIOBuffer * pBuffer=CIOBuffer::Alloc();
        Message* message=reinterpret_cast<Message *>(pBuffer->Buffer);
        message->cmd=MessageType::AsyncMessage;
        EnQueue(pBuffer);
    
     }
     else  
     if(Head.Command==RemoNet::CC_CAN)
     {
          _curTick=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
          bStopedCar=false;
          RemoNet::CCCanMesage Req;
          
          Req.ParseFromArray(ptr,size);
          
          int32_t count=Req.message_size();
          
         
          
          for(int32_t i=0;i<count;i++)
          {
                CIOBuffer Buffer;
                cannet_frame* msg=(cannet_frame *)Buffer.Buffer;
                auto& p=Req.message(i);
                msg->canid=p.canid();

                memcpy(msg->data,p.data().data(),8);
               // printf("%x ",ntohl(msg->canid));
                msg->dlc=p.head();
                Buffer.Length+=sizeof(cannet_frame);
               // msg++;
                 _can->Write(&Buffer);
          }
         //printf("\n"); 
         
     }
     else if(Head.Command==RemoNet::CC_ASKDATACHANNEL)
     {
         bDataChannelCreated=true;
         bStopedCar=false;
         _curTick=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
         StartCar();
     }
}
void CMessageQueue::StopCar()
{
    
    for(auto id:_emergencyArray)
    {
       CIOBuffer Buffer;
      cannet_frame* msg=(cannet_frame *)Buffer.Buffer;
      msg->canid=id;
      msg->dlc=8;
      memset(msg->data,0,sizeof(msg->data));
      Buffer.Length=sizeof(cannet_frame);
      _can->Write(&Buffer);
    }  
    _can->SetStartWrite(false);
}
void CMessageQueue::OnNotifyMessage()
{
         RemoNet::TestTextReq Req;
         Req.set_text("ewqewqewqe");
         CIOBuffer Buffer;
          MessageHead Head;
          Head.Command = RemoNet::CC_Text;
          Head.Length = Req.ByteSize();
          Head.Serialize(Buffer.Buffer);
          auto ptr = Buffer.Buffer + MessageHead::Size();
          Req.SerializeToArray(ptr, Head.Length);
          Buffer.Length = Head.Length + MessageHead::Size();
         _peerArray[0]->SendData(&Buffer);
         
}


void CMessageQueue::StartCar()
{
    _can->SetStartWrite(true);
}
void CMessageQueue::CheckSignal()
{
    if(!bDataChannelCreated) return;
    if(!bStopedCar)
    {
        long long tick=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if(tick-_curTick>1000) 
        {
            StopCar();
            bStopedCar=true;
        }
    }
}