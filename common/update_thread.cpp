#include "protocol.pb.h"
#include "update_thread.h"
#include "socket_client.h"
CUpdateThread::CUpdateThread()
{
      _run=false;

}
void CUpdateThread::start(SocketClient* c)
{
     _run=false;
    _client=c;
    _thread=std::thread(&CUpdateThread::run,this);
}
void CUpdateThread::run()
{
    _run=true;
    long long tick = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    while(_run)
    {
       std::this_thread::sleep_for(std::chrono::microseconds(500));   
       if(!_run) break;
       long long diff=std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
       if(diff-tick>3)
       {
           _client->WriteKeepAlive();
           tick=std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
       } 
    }
}
void CUpdateThread::stop()
{
    if(_run)
    {
        _run=false;
        _thread.join();
    }
}
CUpdateThread::~CUpdateThread()
{
    _run = false;
    _thread.join();
}