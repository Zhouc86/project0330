#include <atomic>
#include <string>
#include "../common/comm.h"
#include "api.h"
#include "message_queue.h"
#include "../common/iobuffer.h"
#include "../common/peer_connection.h"
 
#include <json/json.h>
#include "protocol.pb.h"
#include <fstream>
#include <cstring>
CMessageQueue::CMessageQueue():_head(nullptr),_tail(nullptr)
{
    _ping = false;
}
CMessageQueue::~CMessageQueue()
{

}
void CMessageQueue::Start()
{
  
    _thread = std::thread(&CMessageQueue::Run, this);


}
void CMessageQueue::Stop()
{
    _run = false;
    _thread.join();
}
 
void CMessageQueue::EnQueue(CIOBuffer* pBuffer)
{
    bool bNullBuffer=false;
    std::unique_lock <std::mutex>  lck(_lock);
    if(_head ==nullptr)
    {
        _head =_tail=pBuffer;
        bNullBuffer=true;
    }
    else{
        _tail->NextBuf=pBuffer;
        _tail = _tail->NextBuf;
    } 
    pBuffer->NextBuf=nullptr;
    if(bNullBuffer)
    {
        _cv.notify_one();
    }
}
void CMessageQueue::Run()
{
    _run = true;
	CIOBuffer* ptr = nullptr;

    while(_run)
    {
        std::unique_lock <std::mutex>  lck(_lock);
        /*
        while(Head==nullptr)
        {
            _cv.wait(lck);
        }
        */
        while(_head==nullptr&&_cv.wait_for(lck,std::chrono::seconds(1))==std::cv_status::timeout)
        {
            OnIdle();
        //  CheckSignal();
         // std::cout<<".";
         // std::cout.flush();
          
        }
		while (_head != nullptr)
		{
			ptr = _head;
            _head = _head->NextBuf;
			if (ptr != nullptr)
			{
                Process(ptr);
				ptr->Release();
			}
		}
        
    }
   
}
 