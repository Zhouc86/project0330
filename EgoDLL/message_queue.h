#pragma once
#include<mutex>
#include<memory>
#include<string>
#include<condition_variable>
#include <vector> 
#include "protocol.pb.h"
#include "../common/socket_client.h"
 
#include "../common/update_thread.h"
 
class SocketClient;
class PeerConnectionWrapper;
class VideoRenderer;
/*
enum MessageType:int32_t
{
    ReqVideo,
    RepVideo,
    Connected,
    Leave,
    AsyncMessage
};
*/
struct Message
{
    int32_t cmd;
    int64_t param_l;
    int64_t param_r;
    
};

class CIOBuffer;
class CMessageQueue 
{
public:
   CMessageQueue();
   virtual ~CMessageQueue();
   virtual void Start();
   void EnQueue(CIOBuffer* pBuffer);
   virtual void Stop();
   virtual void Process(CIOBuffer * pBuffer) = 0;
protected:
    virtual void OnIdle() = 0;
private:
   void Run();
  
   
 
   
   
private:
	std::mutex _lock;
	std::condition_variable _cv;
	CIOBuffer* _head;
	CIOBuffer* _tail; 
	std::thread _thread;
	bool _run;
protected:
    bool _ping;
 
};
