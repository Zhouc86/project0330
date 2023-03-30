#pragma once
#include<thread>
class SocketClient;
class INativeNotify;
class CUpdateThread 
{
public:
 
	CUpdateThread();
 
   ~CUpdateThread();
   void start(SocketClient *c);
   void stop();
 
private:
   void run();   
private:
   SocketClient* _client;
   std::thread _thread;
   bool _run;
 
};