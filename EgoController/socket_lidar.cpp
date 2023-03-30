#include "socket_lidar.h"
#include "../common/iobuffer.h"
#include <stdio.h>
#include <iostream>
#include<string>
#include <cstring>
#include "message_queue.h"

SocketLidar::SocketLidar(CMessageQueue* n):_canNotify(n)
{
 
}
bool SocketLidar::Start(int32_t port)
{
#ifdef WIN32
	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);
#endif
	_fd = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (::bind(_fd, (sockaddr*)&sin, sizeof(sin)) == -1)
		return false;
	_thread = std::thread(&SocketLidar::Run, this);	 
	return true;
}
void SocketLidar::Run()
{
	_run = true;
	int8_t stopCnt;
	long long  k = 0;
	long long tick = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	while (_run)
	{
		CIOBuffer pBuffer;
		sockaddr_in from;
		socklen_t fromlen=sizeof(sockaddr_in);
		int32_t ret = recvfrom(_fd,(char *)pBuffer.Buffer, CIOBuffer::IO_BUFFER_SIZE,0,(sockaddr*)&from,&fromlen);
		if (ret <= 0)
		{	 
			continue;
		}

		int32_t receivedCnt = ret/CAN_MSG_LEN;
		cannet_frame* p = (cannet_frame*)pBuffer.Buffer;
		
		memset(&CANThread::can_receive_struct, 0, sizeof(CANThread::can_receive_struct));

		for(int i=0; i<receivedCnt; i++)
		{
			_message[p->canid] = *p;
			 
			uint32_t tmp = ntohl(_message[p->canid].canid);
			//std::cout << i <<  "###" <<ntohl(_message[p->canid].canid) << std::endl;
			uint32_t radarMsgID = tmp & 0xFFFFFF0F;
			uint32_t radarID = (tmp & 0xF0) / 0x10;
		//	if(radarID == 3 || radarID == 4 || radarID == 5)
			{
				CANThread::can_receive_struct[i].DataLen = _message[p->canid].dlc;
				CANThread::can_receive_struct[i].ID = radarMsgID;
				CANThread::can_receive_struct[i].NodeId = radarID;
				for (uint32_t j = 0; j < 8; j++) {
					CANThread::can_receive_struct[i].Data[j] = _message[p->canid].data[j];
				}
				radarDeal.receiveCanData();

				bool stop = false;
				for(uint32_t i=0; i<radarDeal.radar_object_counter; i++)
				{
					float dist = 0;
					//list_of_externally_accessible_object为1时，radar_object_list1[]可用
					//list_of_externally_accessible_object为2时，radar_object_list2[]可用					
					if(CANThread::list_of_externally_accessible_object == 1)
					{
						dist = CANThread::radar_object_list1[i].Object_DistLong;
					}
					else
					{
						dist = CANThread::radar_object_list2[i].Object_DistLong;
					}
					// if(stopCnt >= 3)
					// {
					// 	_canNotify->StopCar();
					// 	stop=true;
					// 	break;
					// }
					// if(dist < 3.0)
					// {
					// 	stopCnt++;
					// } 
					// else
					// {
					// 	if(stop)
					// 	_canNotify->StartCar();
					// 	stopCnt = 0;
					// }
				}
			}
			p++;
		}
 
		long long diff = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		diff -= tick;
		if (diff > 300)
		{
			_canNotify->WriteCanMessage(_message,true);
			_message.clear();
			tick = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		}
	}
}