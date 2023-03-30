 
#include <stdio.h>
#include <iostream>
#include "../common/comm.h"
#include "../common/iobuffer.h"
#include "api.h"
#include "protocol.pb.h"
#include "message_queue.h"
#include <signal.h>
#include <cstring>
void daemonize(void) {
  signal(SIGTTOU, SIG_IGN);
  signal(SIGTTIN, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);
  if (0 != fork()) exit(0);
  if (-1 == setsid()) exit(0);
  signal(SIGHUP, SIG_IGN);
  if (0 != fork()) exit(0);
  //if (0 != chdir("/")) exit(0);
}
 
bool checkOnly()
{
    const char filename[]  = "./lockfile";
    int fd = open (filename, O_WRONLY | O_CREAT , 0644);
    int flock = lockf(fd, F_TLOCK, 0 );
    if (fd == -1) {
        return false;
    }
    //给文件加锁
    if (flock == -1) {
        return false;
    }
    //程序退出后，文件自动解锁
    return true;
}
std::string getpath()
{
	  char exec_name [BUFSIZ];
	 readlink ("/proc/self/exe", exec_name, BUFSIZ);
	 int32_t len=strlen(exec_name);
     for(int i=len;i>=0;i--)
     {
         if(exec_name[i]=='/')
           {
               exec_name[i]=0;
               break;
           }
     }
	 return std::string(exec_name);
}
int main(int argc,char *argv[])
{
   
  //  daemonize();
  auto path=getpath();
    std::cout<<path<<std::endl;
   if(-1==chdir(path.c_str())) return 0;
   if(checkOnly()==false) 
   {
	   std::cout<<"进程已经在运行"<<std::endl;
       return 0;
   }
  // 
   std::cout<<"进程启动..."<<std::endl;
 //  char temp[BUFSIZ];
 //  getcwd(temp,BUFSIZ); 
//   std::cout<<temp<<std::endl;
  
 
    CMessageQueue Q;
    Q.Create();
	while(true)
    {
		Q.Process();
	}
    //gtk_main();
     
    return 0;
}
 