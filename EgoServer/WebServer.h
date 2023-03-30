#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <shared_mutex>
#include <functional>
#include <unordered_map>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#define			Base_Uri_Length			128
typedef websocketpp::server<websocketpp::config::asio> Server;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
class CWebHandler;
typedef websocketpp::config::asio::message_type::ptr message_ptr;
struct KeyValue
{
	std::string strKey;
	std::string strValue;
};
class CWebServer
{
public:
	typedef void (CWebServer::* FN)(rapidjson::Document& value, websocketpp::connection_hdl hdl);
	CWebServer();
	~CWebServer();
	static CWebServer& GetInstance();
	int Init(unsigned short usPort, const char* pBaseUri = "/ws");
	int Uninit();

	int StopWork();
protected:
	int ThreadProccess();
public:
	// 	bool validate(Server *s, websocketpp::connection_hdl hdl);
	// 	void on_http(Server* s, websocketpp::connection_hdl hdl);
	// 	void on_fail(Server* s, websocketpp::connection_hdl hdl);
		//连接打开回调函数
		//void on_open(Server* s, websocketpp::connection_hdl hdl);
		//连接关闭回调函数
	void on_close(Server* s, websocketpp::connection_hdl hdl);
	 
	void on_message(Server* s, websocketpp::connection_hdl hdl, message_ptr msg);
	void Write(websocketpp::connection_hdl connect_hdl, const char* buffer);
	void Remove(websocketpp::connection_hdl connect_hdl);
	 
	static int StringSplit(std::vector<std::string>& dst, const std::string& src, const std::string& separator);
	 
	static std::string& StringTrim(std::string& str);
	 
	static bool GetReqeustCommandAndParmeter(std::string strUri, std::string& strRequestOperateCommand, std::vector<KeyValue>& listRequestOperateParameter);
	int32_t GetId(websocketpp::connection_hdl hdl);
private:




	void OnWebReqVideo(rapidjson::Document& value, websocketpp::connection_hdl hdl);
	void OnWebRepVideo(rapidjson::Document& value, websocketpp::connection_hdl hdl);
	void OnWebCancelReq(rapidjson::Document& value, websocketpp::connection_hdl hdl);
	//void OnWebCloseVideo(rapidjson::Document& value, websocketpp::connection_hdl hdl);
	void OnWebOffer(rapidjson::Document& value, websocketpp::connection_hdl hdl);
	void OnWebAnswer(rapidjson::Document& value, websocketpp::connection_hdl hdl);
	void OnWebCadidate(rapidjson::Document& value, websocketpp::connection_hdl hdl);

	void OnWebSigin(rapidjson::Document& value, websocketpp::connection_hdl hdl);

	void OnWebLeave(rapidjson::Document& value, websocketpp::connection_hdl hdl);

	void OnWebHeartbeat(rapidjson::Document& value, websocketpp::connection_hdl hdl);
 


protected:
	//unsigned short m_usPort;
	//char m_szBaseUri[Base_Uri_Length];
	Server m_server;
	std::thread  websocket_thread;
	//bool m_bThreadExit;
	//std::list<websocketpp::connection_hdl> m_listClientConnection;
	std::unordered_map<std::string, FN>  MapFn;
	std::unordered_map<Server::connection_ptr, std::shared_ptr<CWebHandler>> UserHandler;
	std::shared_mutex lock;
};