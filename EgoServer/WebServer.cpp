#define WIN32_LEAN_AND_MEAN
#include <stddef.h>
#include <Windows.h>

#include "DBConnect.h"
#include "scoped_ptr.h"

#include "IOBuffer.h" 
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "WebHandler.h"

#include "UserManager.h"

#include "WebServer.h"


#include <string>

CWebServer::CWebServer()
{

}
CWebServer::~CWebServer()
{
	websocket_thread.join();
}
void CWebServer::on_close(Server* s, websocketpp::connection_hdl hdl)
{
	Server::connection_ptr conInput = m_server.get_con_from_hdl(hdl);
	std::lock_guard<std::shared_mutex> l(lock);
	auto it = UserHandler.find(conInput);
	if (it != UserHandler.end())
	{

		CWebUserManager::GetInstance().Remove(it->second->GetId());
		rapidjson::StringBuffer strBuf;
		rapidjson::Writer<rapidjson::StringBuffer> root(strBuf);
		root.StartObject();
		root.Key("type");
		root.String(kNotify);
		root.Key("uid");
		root.Int(it->second->GetId());
		root.Key("state");
		root.Bool(false);
		root.EndObject();

		CWebUserManager::GetInstance().BroadCastUserState(it->second->GetId(), strBuf.GetString());
		it->second->OnClose();


		UserHandler.erase(it);
	}

	//DeleteClientConnection(hdl);

}
CWebServer& CWebServer::GetInstance()
{
	static CWebServer s;
	return s;
}
void CWebServer::on_message(Server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
	/*
	hdl.lock().get() 获得连接标识
	msg->get_payload() 是收到的消息内容
	msg->get_opcode() 是收到消息的类型 ，包含：文本TEXT,二进制BINARY等等
	*/
	try {
		//std::cout <<  msg->get_payload()	<< std::endl;
		std::string str = msg->get_payload();
		rapidjson::Document document;
		document.Parse(str.c_str());
		if (document.IsObject() || !document.HasParseError())
		{

			if (!document.HasMember("type") || !document["type"].IsString()) return;
			auto type = document["type"].GetString();


			auto it = MapFn.find(type);
			if (it != MapFn.end())
			{
				(this->*it->second)(document, hdl);
			}


		}
	}
	catch (websocketpp::exception const& e)
	{
		std::cout << "exception: " << e.what() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << "exception: " << e.what() << std::endl;
	}
	catch (...)
	{


	}
	// 	try {
	// 		/*
	// 		发送消息
	// 		s->send(
	// 		hdl, //连接
	// 		msg->get_payload(), //消息
	// 		msg->get_opcode());//消息类型
	// 		*/
	// 		s->send(hdl, msg->get_payload(), msg->get_opcode());
	// 	}
	// 	catch (websocketpp::exception const & e) {
	// 		std::cout << "Echo failed because: "
	// 			<< "(" << e.what() << ")" << std::endl;
	// 	}
}
int32_t CWebServer::GetId(websocketpp::connection_hdl hdl)
{
	Server::connection_ptr conInput = m_server.get_con_from_hdl(hdl);
	auto it = UserHandler.find(conInput);
	if (it != UserHandler.end())
		return it->second->GetId();
	return -1;
}
 
 
int CWebServer::Init(unsigned short usPort, const char* pBaseUri)
{
	// 	MapFn.insert(make_pair(LOGIN, &CWebSocketServer::OnWebsocketLogin));
	// 	MapFn.insert(make_pair(ACTION, &CWebSocketServer::OnWebsocketAction));
	// 	MapFn.insert(std::make_pair(CONNECT, &CWebSocketServer::OnWebSocketConnect));
	// 	MapFn.insert(std::make_pair(CHAT, &CWebSocketServer::OnWebSocketChat));
	int nRet = 0;
	//m_usPort = usPort;
	//strcpy_s(m_szBaseUri, pBaseUri);



	MapFn.insert(make_pair(kReqVideo, &CWebServer::OnWebReqVideo));
	MapFn.insert(make_pair(kRepVideo, &CWebServer::OnWebRepVideo));

	MapFn.insert(make_pair(kSigin, &CWebServer::OnWebSigin));
	MapFn.insert(make_pair(kOffer, &CWebServer::OnWebOffer));
	MapFn.insert(make_pair(kAnswer, &CWebServer::OnWebAnswer));
	MapFn.insert(make_pair(kCandidate, &CWebServer::OnWebCadidate));
	MapFn.insert(make_pair(kLeave, &CWebServer::OnWebLeave));

	MapFn.insert(make_pair(kCancelReq, &CWebServer::OnWebCancelReq));

	MapFn.insert(make_pair(kHeartbit, &CWebServer::OnWebHeartbeat));

 




	try {
		// Set logging settings

		m_server.set_access_channels(websocketpp::log::alevel::devel);
		m_server.set_error_channels(websocketpp::log::elevel::devel);

		// Register our message handler
		m_server.set_message_handler(std::bind(&CWebServer::on_message, this, &m_server, ::_1, ::_2));
		 
		// 		m_server.set_fail_handler(std::bind(&CWebSocketServer::on_fail, this, &m_server, ::_1));
				//m_server.set_open_handler(std::bind(&CWebSocketServer::on_open, this, &m_server, ::_1));
		m_server.set_close_handler(std::bind(&CWebServer::on_close, this, &m_server, ::_1));
		//m_server.set_validate_handler(std::bind(&CWebSocketServer::validate, this, &m_server, ::_1));

		// Initialize ASIO
		m_server.init_asio();
		m_server.set_reuse_addr(true);
 
		// Listen on port
		m_server.listen(usPort);
		// Start the server accept loop
		m_server.start_accept();
		//	m_bThreadExit = false;
		websocket_thread = std::move(std::thread(std::bind(&CWebServer::ThreadProccess, this)));






	}
	catch (websocketpp::exception const& e) {
		std::cout << e.what() << std::endl;
		nRet = -1;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		nRet = -2;
	}
	catch (...) {
		std::cout << "other exception" << std::endl;
		nRet = -3;
	}

	return nRet;
}
int CWebServer::Uninit()
{
	return 0;
}

int CWebServer::StopWork()
{
	//stop
	//m_bThreadExit = true;
	m_server.stop();
	return 0;
}

int CWebServer::ThreadProccess()
{
	m_server.run();
	return 0;
}



// 字符串分割
int CWebServer::StringSplit(std::vector<std::string>& dst, const std::string& src, const std::string& separator)
{
	if (src.empty() || separator.empty())
		return 0;

	int nCount = 0;
	std::string temp;
	size_t pos = 0, offset = 0;

	// 分割第1~n-1个
	while ((pos = src.find_first_of(separator, offset)) != std::string::npos)
	{
		temp = src.substr(offset, pos - offset);
		if (temp.length() > 0) {
			dst.push_back(temp);
			nCount++;
		}
		offset = pos + 1;
	}

	// 分割第n个
	temp = src.substr(offset, src.length() - offset);
	if (temp.length() > 0) {
		dst.push_back(temp);
		nCount++;
	}

	return nCount;
}
//去前后空格
std::string& CWebServer::StringTrim(std::string& str)
{
	if (str.empty()) {
		return str;
	}
	str.erase(0, str.find_first_not_of(" "));
	str.erase(str.find_last_not_of(" ") + 1);
	return str;
}
//获取请求命令与参数
bool CWebServer::GetReqeustCommandAndParmeter(std::string strUri, std::string& strRequestOperateCommand, std::vector<KeyValue>& listRequestOperateParameter)
{
	bool bRet = false;
	std::vector<std::string> vecRequest;
	int nRetSplit = StringSplit(vecRequest, strUri, "?");
	if (nRetSplit > 0)
	{
		if (vecRequest.size() == 1)
		{
			strRequestOperateCommand = vecRequest[0];
		}
		else if (vecRequest.size() > 1)
		{
			strRequestOperateCommand = vecRequest[0];
			std::string strRequestParameter = vecRequest[1];
			std::vector<std::string> vecParams;
			nRetSplit = StringSplit(vecParams, strRequestParameter, "&");
			if (nRetSplit > 0)
			{
				std::vector<std::string>::iterator iter, iterEnd;
				iter = vecParams.begin();
				iterEnd = vecParams.end();
				for (iter; iter != iterEnd; iter++)
				{
					std::vector<std::string> vecNameOrValue;
					nRetSplit = StringSplit(vecNameOrValue, *iter, "=");
					if (nRetSplit > 0)
					{
						KeyValue nvNameAndValue;
						nvNameAndValue.strKey = vecNameOrValue[0];
						nvNameAndValue.strValue = "";
						if (vecNameOrValue.size() > 1)
						{
							nvNameAndValue.strValue = vecNameOrValue[1];
						}
						//insert
						listRequestOperateParameter.push_back(nvNameAndValue);
					}
				}
			}
		}
		else
		{

		}
	}
	return bRet;
}




void CWebServer::OnWebReqVideo(rapidjson::Document& value, websocketpp::connection_hdl hdl)
{

	Server::connection_ptr conInput = m_server.get_con_from_hdl(hdl);
	int32_t peer = value["peer"].GetInt();
	int32_t width = value["width"].GetInt();
	int32_t height = value["height"].GetInt();
	int32_t fps = value["fps"].GetInt();

	std::shared_lock<std::shared_mutex> l(lock);
	auto it = UserHandler.find(conInput);
	if (it != UserHandler.end())
	{
		it->second->OnWebReqVideo(peer, width, height, fps);
	}
}
void CWebServer::OnWebRepVideo(rapidjson::Document& value, websocketpp::connection_hdl hdl)
{
	Server::connection_ptr conInput = m_server.get_con_from_hdl(hdl);
	int32_t peer = value["peer"].GetInt();
	std::string ret = value["ret"].GetString();
	int32_t width = value["width"].GetInt();
	int32_t height = value["height"].GetInt();
	int32_t fps = value["fps"].GetInt();
	std::shared_lock<std::shared_mutex> l(lock);
	auto it = UserHandler.find(conInput);
	if (it != UserHandler.end())
	{
		it->second->OnWebRepVideo(peer, ret, width, height, fps);
	}
}


void CWebServer::Write(websocketpp::connection_hdl connect_hdl, const char* buffer)
{
	m_server.send(connect_hdl, buffer, websocketpp::frame::opcode::value::TEXT);
}
void CWebServer::OnWebSigin(rapidjson::Document& value, websocketpp::connection_hdl hdl)
{

	if (!value.HasMember("account") || value["password"].IsNull()) return;
	Server::connection_ptr conInput = m_server.get_con_from_hdl(hdl);
	if (UserHandler.find(conInput) != UserHandler.end()) return;

	std::string account = value["account"].GetString();
	std::string password = value["password"].GetString();






	CQPtr<CConnectionPtr<sql::Connection>> Conn = CDBConnectPool::GetInstance().QueryConnect();

	scoped_ptr<sql::Statement> stmt = (*Conn.get())->createStatement();
	char sql[1024];
	int32_t id;

	sprintf_s(sql, "select id,name,cid from user where account=\'%s\' and password=MD5(\'%s\')", account.c_str(), password.c_str());
	scoped_ptr<sql::ResultSet>  resultSet = stmt->executeQuery(sql);


	rapidjson::StringBuffer strBuf;
	rapidjson::Writer<rapidjson::StringBuffer> root(strBuf);
	root.StartObject();
	root.Key("type");
	root.String(kSigin);
	root.Key("ret");

	if (resultSet->next())
	{
		id = resultSet->getInt(1);
		std::string name = resultSet->getString(2).c_str();
		int32_t cid = resultSet->getInt(3);



		// = std::make_shared(new CWebMessageHandler(hdl));
		std::cout << cid << "," << id << "," << name << std::endl;
		root.String("OK");
		root.Key("name");
		root.String(name.c_str());
		root.Key("uid");
		root.Int(id);
		root.Key("company");
		root.Int(cid);
		websocketpp::connection_hdl handle;
		auto type = CWebUserManager::GetInstance().Find(id);


		if (type) {
			printf("Kick Off");
			rapidjson::StringBuffer strBuf;
			rapidjson::Writer<rapidjson::StringBuffer> root(strBuf);
			root.StartObject();
			root.Key("type");
			root.String(kKickOff);
			root.EndObject();
			CIOBuffer* pBuffer = CIOBuffer::Alloc();
			strcpy((char*)pBuffer->Buffer, strBuf.GetString());
			pBuffer->Length = strBuf.GetLength() + 1;
			Write(hdl, (char*)pBuffer->Buffer);
			pBuffer->Release();
		}
		else
		{
			std::shared_ptr<CWebHandler> ptr(new CWebHandler(this, id, cid, hdl));
			CWebUserManager::GetInstance().Add(ptr);
			UserHandler.insert(make_pair(conInput, ptr));
		}


	}
	else
	{
		root.String("ERROR");
	}
	root.EndObject();
	Write(hdl, strBuf.GetString());


}

void CWebServer::OnWebOffer(rapidjson::Document& value, websocketpp::connection_hdl hdl)
{

	if (value["peer"].IsNull() || !value["peer"].IsInt()) return;
	int32_t peer = value["peer"].GetInt();

	Server::connection_ptr conInput = m_server.get_con_from_hdl(hdl);
	std::shared_lock<std::shared_mutex> l(lock);
	auto it = UserHandler.find(conInput);
	if (it == UserHandler.end()) return;
	int32_t id = it->second->GetId();
	if (id == -1) return;
	rapidjson::StringBuffer strBuf;
	rapidjson::Writer<rapidjson::StringBuffer> root(strBuf);
	root.StartObject();
	root.Key("type");
	root.String(kOffer);
	root.Key("peer");
	root.Int(id);

	root.Key("sdp");
	root.String(value["sdp"].GetString());
	root.EndObject();
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	strcpy((char*)pBuffer->Buffer, strBuf.GetString());
	pBuffer->Length = strBuf.GetLength() + 1;
	CWebUserManager::GetInstance().Write(peer, pBuffer);
	pBuffer->Release();


}

void CWebServer::OnWebAnswer(rapidjson::Document& value, websocketpp::connection_hdl hdl)
{
	if (value["peer"].IsNull() || !value["peer"].IsInt()) return;
	int32_t peer = value["peer"].GetInt();
	Server::connection_ptr conInput = m_server.get_con_from_hdl(hdl);
	std::shared_lock<std::shared_mutex> l(lock);
	auto it = UserHandler.find(conInput);
	if (it != UserHandler.end())
	{
		int32_t id = it->second->GetId();
		if (id == -1) return;
		rapidjson::StringBuffer strBuf;
		rapidjson::Writer<rapidjson::StringBuffer> root(strBuf);
		root.StartObject();
		root.Key("type");
		root.String(kAnswer);
		root.Key("peer");
		root.Int(id);
		root.Key("sdp");

		std::string sdp = value["sdp"].GetString();

		root.String(sdp.c_str());

		root.EndObject();
		CIOBuffer* pBuffer = CIOBuffer::Alloc();
		strcpy((char*)pBuffer->Buffer, strBuf.GetString());
		pBuffer->Length = strBuf.GetLength() + 1;
		CWebUserManager::GetInstance().Write(peer, pBuffer);
		pBuffer->Release();

	}
}

void CWebServer::OnWebCadidate(rapidjson::Document& value, websocketpp::connection_hdl hdl)
{
	if (value["peer"].IsNull() || !value["peer"].IsInt()) return;
	int32_t peer = value["peer"].GetInt();
	Server::connection_ptr conInput = m_server.get_con_from_hdl(hdl);
	std::shared_lock<std::shared_mutex> l(lock);
	auto it = UserHandler.find(conInput);
	if (it != UserHandler.end())
	{
		int32_t id = it->second->GetId();
		if (id == -1) return;   
		rapidjson::StringBuffer strBuf;
		rapidjson::Writer<rapidjson::StringBuffer> root(strBuf);
		root.StartObject();
		root.Key("type");
		root.String(kCandidate);
		root.Key("peer");
		root.Int(id);
		root.Key("candidate");

		std::string candidate = value["candidate"].GetString();
		root.String(candidate.c_str());
		int32_t sdpMLineIndex = value["sdpMLineIndex"].GetInt();
		root.Key("sdpMLineIndex");
		root.Int(sdpMLineIndex);
		root.Key("sdpMid");
		std::string sdpMid = value["sdpMid"].GetString();
		root.String(sdpMid.c_str());
		root.EndObject();
		CIOBuffer* pBuffer = CIOBuffer::Alloc();
		strcpy((char*)pBuffer->Buffer, strBuf.GetString());
		pBuffer->Length = strBuf.GetLength() + 1;
		CWebUserManager::GetInstance().Write(peer, pBuffer);
		pBuffer->Release();

	}
}

void CWebServer::OnWebLeave(rapidjson::Document& value, websocketpp::connection_hdl hdl)
{
	if (value["peer"].IsNull() || !value["peer"].IsInt()) return;
	int32_t peer = value["peer"].GetInt();
	Server::connection_ptr conInput = m_server.get_con_from_hdl(hdl);
	std::shared_lock<std::shared_mutex> l(lock);
	auto it = UserHandler.find(conInput);
	if (it != UserHandler.end())
	{
		int32_t id = it->second->GetId();
		it->second->OnWebLeave(peer);
		if (id == -1) return;




	}
}

void CWebServer::OnWebCancelReq(rapidjson::Document& value, websocketpp::connection_hdl hdl)
{
	std::shared_lock<std::shared_mutex> l(lock);
	Server::connection_ptr conInput = m_server.get_con_from_hdl(hdl);
	auto it = UserHandler.find(conInput);
	int32_t peer = value["peer"].GetInt();
	if (it != UserHandler.end())
	{
		it->second->OnWebCancelReq(peer);
	}
}

void CWebServer::OnWebHeartbeat(rapidjson::Document& value, websocketpp::connection_hdl hdl)
{
	Server::connection_ptr conInput = m_server.get_con_from_hdl(hdl);
	std::shared_lock<std::shared_mutex> l(lock);
	auto it = UserHandler.find(conInput);
	if (it != UserHandler.end())
	{
		it->second->OnWebHeartbeat();
	}
}
void CWebServer::Remove(websocketpp::connection_hdl connect_hdl)
{
	Server::connection_ptr conInput = m_server.get_con_from_hdl(connect_hdl);

	std::lock_guard<std::shared_mutex> l(lock);
	auto it = UserHandler.find(conInput);

	if (it != UserHandler.end())
	{

		UserHandler.erase(it);
	}
	conInput->close(0, "");
}

 

