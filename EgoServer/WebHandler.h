#pragma once
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <unordered_map>
#include <mutex>
#include <string>
#include <memory>
#include <vector>
#include <cstdint>

 
const char kCandidateSdpMidName[] = "sdpMid";
const char kCandidateSdpMlineIndexName[] = "sdpMLineIndex";
const char kCandidateSdpName[] = "candidate";

// Names used for a SessionDescription JSON object.
const static char kSessionDescriptionTypeName[] = "type";
const static char kSessionDescriptionSdpName[] = "sdp";
const static char kPeerId[] = "peer";
const static char kSigin[] = "sigin";
const static char kOffer[] = "offer";
const static char kCandidate[] = "candidate";
const static char kLeave[] = "leave";
const static char kAnswer[] = "answer";
const static char kReqVideo[] = "reqvideo";
const static char kCloseVideo[] = "closevideo";
const static char kRepVideo[] = "repvideo";
const static char kCancelReq[] = "cancelreq";
const static char kHeartbit[] = "heartbeat";
const static char kMemberList[] = "memberlist";
const static char kKickOff[] = "kickoff";
const static char kNotify[] = "notify";
class CIOBuffer;
class CWebServer;

class CWebHandler : public std::enable_shared_from_this<CWebHandler>
{
public:
	enum {
		ALLOC_ARG = 16,
		BASE_SIZE = 32
	};
	CWebHandler(CWebServer* s, int32_t uid, int32_t cid, websocketpp::connection_hdl hdl);
	~CWebHandler();
	bool Write(CIOBuffer* pBuffer);

	void OnClose();

	websocketpp::connection_hdl GetConnectHandle();

	void OnWebReqVideo(int32_t peer, int32_t width, int32_t height, int32_t fps);
	void OnWebRepVideo(int32_t peer, std::string& desc, int32_t width, int32_t height, int32_t fps);
	void OnWebCancelReq(int32_t did);

	void OnWebLeave(int32_t peer);

	void OnWebHeartbeat();
 






	int32_t GetId() { return uid; }
	void KickOff();
	uint64_t GetBeatTick() {
		return beat_tick;
	}
	VideoDesc ReqVideo(int32_t uid, int32_t width, int32_t height, int32_t fps);
	void RepVideo(int32_t uid, VideoDesc ret, int32_t width, int32_t height, int32_t fps);
	void LeaveVideo(int32_t peer);
	void CancelReq(int32_t peer);
	//bool ChatMessage(int32_t peer, std::string& type, std::string& content);
	bool Notify(const char* content);

private:
	void MessageThread();
private:
	std::thread _thread;

	websocketpp::connection_hdl connect_hdl;
	CWebServer* server;
	int32_t uid;
	int32_t companyid;

	bool bHasCamera;
	std::atomic<bool> bRun;
	std::atomic<bool> bWait;

	std::atomic<int32_t> video_peer;
	std::mutex videolock;
	uint64_t  beat_tick;
};

