#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "DBConnect.h"
#include "scoped_ptr.h"


#include "IOBuffer.h"

#include "WebHandler.h"


#include "WebServer.h"

#include "UserManager.h"


CWebHandler::CWebHandler(CWebServer* s, int32_t u, int32_t cid, websocketpp::connection_hdl hdl) :server(s), connect_hdl(hdl)
{

	bWait = true;
	bRun = false;
	uid = u;
	companyid = cid;
	video_peer = -1;



	beat_tick = GetTickCount64();


}
CWebHandler::~CWebHandler()
{

}
bool CWebHandler::Write(CIOBuffer* pBuffer)
{
	try {
		server->Write(connect_hdl, (char*)pBuffer->Buffer);
		return true;
	}
	catch (std::exception& e)
	{

	}
	return false;
}
void CWebHandler::OnClose()
{

	if (video_peer != -1)
	{
		CWebUserManager::GetInstance().LeavePeerVideo(video_peer, uid);
	}



}


void CWebHandler::OnWebReqVideo(int32_t peer, int32_t width, int32_t height, int32_t fps)
{
	if (peer == uid) return;
	if (video_peer == -1)
	{


		auto state = CWebUserManager::GetInstance().ConnectPeerVideo(peer, uid, width, height, fps);


		if (state == VideoDesc::OK)
			video_peer = peer;

		else
		{
			rapidjson::StringBuffer strBuf;
			rapidjson::Writer<rapidjson::StringBuffer> root(strBuf);
			root.StartObject();
			root.Key("type");
			root.String(kRepVideo);
			root.Key("peer");
			root.Int(peer);
			root.Key("ret");
			if (state == VideoDesc::Busy)
				root.String("busy");
			else if (state == VideoDesc::NoFound)
				root.String("nofound");
			else if (state = VideoDesc::Reject)
				root.String("Reject");
			root.EndObject();
			CIOBuffer* pBuffer = CIOBuffer::Alloc();
			strcpy((char*)pBuffer->Buffer, strBuf.GetString());
			pBuffer->Length = strBuf.GetLength() + 1;
			Write(pBuffer);
			pBuffer->Release();

		}
	}
	else
	{
		rapidjson::StringBuffer strBuf;
		rapidjson::Writer<rapidjson::StringBuffer> root(strBuf);
		root.StartObject();
		root.Key("type");
		root.String(kRepVideo);
		root.Key("peer");
		root.Int(peer);
		root.Key("ret");
		root.String("IsVideoing");
		root.EndObject();
		CIOBuffer* pBuffer = CIOBuffer::Alloc();
		strcpy((char*)pBuffer->Buffer, strBuf.GetString());
		pBuffer->Length = strBuf.GetLength() + 1;
		Write(pBuffer);
		pBuffer->Release();
	}


}
void CWebHandler::OnWebRepVideo(int32_t peer, std::string& desc, int32_t width, int32_t height, int32_t fps)
{
	VideoDesc type = VideoDesc::OK;
	if (desc == "OK")
		type = VideoDesc::OK;
	else if (desc == "busy")
		type = VideoDesc::Busy;
	else if (desc == "nofound")
		type = VideoDesc::NoFound;
	else if (desc == "IsVideoing")
		type = VideoDesc::IsVideoing;
	if (type == VideoDesc::OK)
	{
		video_peer = peer;
	}
	else
	{
		video_peer = -1;
	}


	CWebUserManager::GetInstance().ReplyPeerVideo(peer, uid, type, width, height, fps);
}
void CWebHandler::KickOff()
{
	rapidjson::StringBuffer strBuf;
	rapidjson::Writer<rapidjson::StringBuffer> root(strBuf);
	root.StartObject();
	root.Key(kSessionDescriptionTypeName);
	root.String(kKickOff);
	root.EndObject();
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	strcpy((char*)pBuffer->Buffer, strBuf.GetString());
	pBuffer->Length = strBuf.GetLength() + 1;
	Write(pBuffer);
	pBuffer->Release();

}
websocketpp::connection_hdl CWebHandler::GetConnectHandle()
{
	return connect_hdl;
}


void CWebHandler::OnWebLeave(int32_t peer)
{

	video_peer = -1;
	CWebUserManager::GetInstance().LeavePeerVideo(peer, uid);


}
void CWebHandler::OnWebCancelReq(int32_t did)
{
	if (video_peer != did) return;
	video_peer = -1;
	CWebUserManager::GetInstance().CancelReq(did, uid);
}

VideoDesc CWebHandler::ReqVideo(int32_t uid, int32_t width, int32_t height, int32_t fps)
{
	if (video_peer != -1 && video_peer != uid)
	{
		return VideoDesc::Busy;
	}
	rapidjson::StringBuffer strBuf;
	rapidjson::Writer<rapidjson::StringBuffer> root(strBuf);
	root.StartObject();
	root.Key("type");
	root.String(kReqVideo);
	root.Key("peer");
	root.Int(uid);

	root.Key("width");
	root.Int(width);
	root.Key("height");
	root.Int(height);
	root.Key("fps");
	root.Int(fps);
	root.EndObject();
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	strcpy((char*)pBuffer->Buffer, strBuf.GetString());
	pBuffer->Length = strBuf.GetLength() + 1;
	Write(pBuffer);
	pBuffer->Release();
	return VideoDesc::OK;
}
void CWebHandler::RepVideo(int32_t peer, VideoDesc ret, int32_t width, int32_t height, int32_t fps)
{
	if (ret != VideoDesc::OK)
	{

		video_peer = -1;
	}
	rapidjson::StringBuffer strBuf;
	rapidjson::Writer<rapidjson::StringBuffer> root(strBuf);
	root.StartObject();
	root.Key("type");
	root.String(kRepVideo);
	root.Key("peer");
	root.Int(peer);
	root.Key("width");
	root.Int(width);
	root.Key("height");
	root.Int(height);
	root.Key("fps");
	root.Int(fps);
	root.Key("ret");
	if (ret == VideoDesc::OK)
		root.String("OK");
	else if (ret == VideoDesc::Busy)
		root.String("busy");
	else if (ret == VideoDesc::Reject)
		root.String("reject");
	root.EndObject();
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	strcpy((char*)pBuffer->Buffer, strBuf.GetString());
	pBuffer->Length = strBuf.GetLength() + 1;
	Write(pBuffer);
	pBuffer->Release();
}
void CWebHandler::LeaveVideo(int32_t peer)
{
	if (video_peer != peer) return;
	video_peer = -1;
	rapidjson::StringBuffer strBuf;
	rapidjson::Writer<rapidjson::StringBuffer> root(strBuf);
	root.StartObject();
	root.Key("type");
	root.String(kLeave);
	root.Key("peer");
	root.Int(uid);

	root.EndObject();
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	strcpy((char*)pBuffer->Buffer, strBuf.GetString());
	pBuffer->Length = strBuf.GetLength() + 1;
	Write(pBuffer);
	pBuffer->Release();

}
void CWebHandler::CancelReq(int32_t peer)
{
	if (video_peer != peer) return;
	video_peer = -1;
	rapidjson::StringBuffer strBuf;
	rapidjson::Writer<rapidjson::StringBuffer> root(strBuf);
	root.StartObject();
	root.Key("type");
	root.String(kCancelReq);
	root.Key("peer");
	root.Int(uid);

	root.EndObject();
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	strcpy((char*)pBuffer->Buffer, strBuf.GetString());
	pBuffer->Length = strBuf.GetLength() + 1;
	Write(pBuffer);
	pBuffer->Release();
}

 
void CWebHandler::MessageThread()
{
	CQPtr<CConnectionPtr<sql::Connection>> Conn = CDBConnectPool::GetInstance().QueryConnect();

	scoped_ptr<sql::Statement> stmt = (*Conn.get())->createStatement();
	char sql[1024];
	sprintf_s(sql, "select id, uid,content,create_time from user_message where uid=%d and processed=0", uid);
	std::vector<int32_t> ids;
	scoped_ptr<sql::ResultSet>  resultSet = stmt->executeQuery(sql);
	while (resultSet->next())
	{
		int32_t id = resultSet->getInt(1);
		int32_t uid = resultSet->getInt(2);

		std::string content = resultSet->getString(3).c_str();
		std::string datetime = resultSet->getString(4).c_str();
		rapidjson::StringBuffer strBuf;
		rapidjson::Writer<rapidjson::StringBuffer> root(strBuf);
		root.StartObject();
		root.Key("type");
		root.String("message");
		root.Key("content");
		root.String(content.c_str());
		root.Key("date");
		root.String(datetime.c_str());
		root.EndObject();

		CIOBuffer* pBuffer = CIOBuffer::Alloc();
		strcpy((char*)pBuffer->Buffer, strBuf.GetString());
		pBuffer->Length = strBuf.GetLength() + 1;
		bool bret = Write(pBuffer);
		pBuffer->Release();
		ids.push_back(id);

		Sleep(10);
	}
	for (auto id : ids)
	{
		sprintf_s(sql, "update user_message set processed=1 where id=%d", id);
		stmt->execute(sql);
	}

}
void CWebHandler::OnWebHeartbeat()
{
	beat_tick = GetTickCount64();
}
bool CWebHandler::Notify(const char* content)
{
	// 	rapidjson::StringBuffer strBuf;
	// 	rapidjson::Writer<rapidjson::StringBuffer> root(strBuf);
	// 	root.StartObject();
	// 	root.Key("type");
	// 	root.String(kNotify);
	// 	root.Key("content");
	// 	root.String(content);
	// 	root.EndObject();
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	strcpy((char*)pBuffer->Buffer, content);
	pBuffer->Length = strlen(content) + 1;
	bool bret = Write(pBuffer);
	pBuffer->Release();
	return bret;
}


 