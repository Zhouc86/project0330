#include "api.h"
#include "./include/EgoInterface.h"
#include "../common/peer_connection.h"
#include "EgoWindow.h"
#include "../common/iobuffer.h"
#include "libyuv/convert_argb.h"
#include "libyuv/rotate.h"
#include "./include/EgoInterface.h"
#include "out_sim.h"
#include <assert.h>
#include "Protocol.pb.h"
#define  PI 3.141526f


CEgoWindow::CEgoWindow(IRender * render,RenderPosition pos, SocketClient* c) :_render(render),_pos(pos), _client(c)
{
	//_channelcreated = false;
	_control = ControlState::Check;
	 
}
void CEgoWindow::Start()
{
	//assert(n != nullptr);
	CMessageQueue::Start();
	 
}
void CEgoWindow::Stop()
{
	CMessageQueue::Stop();
	 
}
void CEgoWindow::PostMessage(int32_t type, int64_t l /* = 0 */, int64_t r /* = 0 */)
{
	CIOBuffer* pBuffer = CIOBuffer::Alloc();
	 
	Message* message = reinterpret_cast<Message*>(pBuffer->Buffer);
	message->cmd = type;
	message->param_l = l;
	message->param_r = r;
	EnQueue(pBuffer);
}
void CEgoWindow::SetPeerNotify()
{
	 
}
void CEgoWindow::OnNotifyRep(int32_t peer)
{
	int32_t width = 1280;
	int32_t height = 720;
	ChannelType channel = (ChannelType)(_pos - RenderPosition::FRONT);
 
	_peerconnection = std::make_unique<CPeerConnection>(channel, _client);
	SetRenderWindow(width, height);
	InitPeerConnection(peer);
	_peerconnection->CreateOffer();
}
void CEgoWindow::SetRenderWindow(int32_t width, int32_t height)
{
	ZeroMemory(&bmi_, sizeof(bmi_));
	bmi_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi_.bmiHeader.biPlanes = 1;
	bmi_.bmiHeader.biBitCount = 32;
	bmi_.bmiHeader.biCompression = BI_RGB;
	if (_pos == RenderPosition::LEFT || _pos == RenderPosition::RIGHT)
	{
		bmi_.bmiHeader.biWidth = height;
		bmi_.bmiHeader.biHeight = -width;
	}
	else {
		bmi_.bmiHeader.biWidth = width;
		bmi_.bmiHeader.biHeight = -height;
	}
	bmi_.bmiHeader.biSizeImage =
		width * height * (bmi_.bmiHeader.biBitCount >> 3);
	image_.reset(new uint8_t[bmi_.bmiHeader.biSizeImage]);
	if (_pos == RenderPosition::LEFT || _pos == RenderPosition::RIGHT)
		_rotate.reset(new uint8_t[bmi_.bmiHeader.biSizeImage]);
}
void CEgoWindow::InitPeerConnection(int32_t peer)
{
	 
	_peerconnection->Initialize(peer, _pos);
	
	_peerconnection->AddDataChannel(true, false);

	
	_peerconnection->AddRemoteArgb32VideoFrameReady(&CEgoWindow::FrameCallback, this);
	//m_PeerConnection->AddLocalAudioTrack();
	StartRender(true);
}
void CEgoWindow::FrameCallback(void* user_data, const uint8_t* yptr, int32_t strideY, const uint8_t* uptr, int32_t strideU, const uint8_t* vptr, int32_t strideV, const int32_t stride, const int frame_width, const int frame_height)
{
	auto ptr = static_cast<CEgoWindow*>(user_data);

	ptr->OnArgb32FrameReady(yptr, strideY, uptr, strideU, vptr, strideV, stride, frame_width, frame_height);
}
void CEgoWindow::StartRender(bool b)
{
	start_ = b;
}
void CEgoWindow::OnArgb32FrameReady(const uint8_t* yptr, int32_t strideY, const uint8_t* uptr, int32_t strideU, const uint8_t* vptr, int32_t strideV, const int32_t stride, const int width, const int height)
{
	{
		//	int32_t*pixels = (int32_t*)data;

			//std::lock_guard < std::mutex> l(buffer_lock_);


		if (RenderPosition::LEFT == _pos)
		{
			SetSize(height, width);
			uint8_t* i420_image_rotated = _rotate.get();////(uint8_t*)malloc(width * height * 3 ) ;
			uint8_t* i420_image_rotated_y_ptr = i420_image_rotated;
			uint8_t* i420_image_rotated_u_ptr = i420_image_rotated_y_ptr + (height * width);
			uint8_t* i420_image_rotated_v_ptr = i420_image_rotated_u_ptr + (int)(height * width * 0.25);


			libyuv::I420Rotate(yptr, width, uptr, width >> 1, vptr, width >> 1, i420_image_rotated_y_ptr,
				height,
				i420_image_rotated_u_ptr,
				height >> 1,
				i420_image_rotated_v_ptr,
				height >> 1,
				width,
				height, libyuv::RotationMode::kRotate270);
			yptr = i420_image_rotated_y_ptr;
			strideY = height;
			uptr = i420_image_rotated_u_ptr;
			strideU = height >> 1;
			vptr = i420_image_rotated_v_ptr;
			strideV = height >> 1;
			libyuv::I420ToARGB(yptr, strideY, uptr, strideU, vptr, strideV, image_.get(), height * 4, height, width);

		}
		else if (RenderPosition::RIGHT == _pos)
		{
			SetSize(height, width);
			uint8_t* i420_image_rotated = _rotate.get();
			uint8_t* i420_image_rotated_y_ptr = i420_image_rotated;
			uint8_t* i420_image_rotated_u_ptr = i420_image_rotated_y_ptr + (height * width);
			uint8_t* i420_image_rotated_v_ptr = i420_image_rotated_u_ptr + (int)(height * width * 0.25);


			libyuv::I420Rotate(yptr, width, uptr, width >> 1, vptr, width >> 1, i420_image_rotated_y_ptr,
				height,
				i420_image_rotated_u_ptr,
				height >> 1,
				i420_image_rotated_v_ptr,
				height >> 1,
				width,
				height, libyuv::RotationMode::kRotate90);
			yptr = i420_image_rotated_y_ptr;
			strideY = height;
			uptr = i420_image_rotated_u_ptr;
			strideU = height >> 1;
			vptr = i420_image_rotated_v_ptr;
			strideV = height >> 1;
			libyuv::I420ToARGB(yptr, strideY, uptr, strideU, vptr, strideV, image_.get(), height * 4, height, width);

		}
		else
		{
			SetSize(width, height);
			libyuv::I420ToARGB(yptr, strideY, uptr, strideU, vptr, strideV, image_.get(), stride, width, height);
		}
		 

	}
	_render->OnRender(image_,width,height);
}
 
void CEgoWindow::SetSize(int32_t width, int32_t height)
{

	if (width == bmi_.bmiHeader.biWidth && height == -bmi_.bmiHeader.biHeight) {
		return;
	}

	// 	if (m_Pos == CameraPosition::CAR_LEFT || m_Pos == CameraPosition::CAR_RIGHT)
	// 	{
	// 		bmi_.bmiHeader.biWidth = height;
	// 		bmi_.bmiHeader.biHeight = -width;
	// 	}
	// 	else
	{
		bmi_.bmiHeader.biWidth = width;
		bmi_.bmiHeader.biHeight = -height;
	}
	bmi_.bmiHeader.biSizeImage =
		width * height * (bmi_.bmiHeader.biBitCount >> 3);


	image_.reset(new uint8_t[bmi_.bmiHeader.biSizeImage]);
	if (_pos == RenderPosition::LEFT || _pos == RenderPosition::RIGHT)
		_rotate.reset(new uint8_t[bmi_.bmiHeader.biSizeImage]);
}
// void CEgoWindow::WriteCanMessage(std::unordered_map<int32_t, cannet_frame>& node, bool)
// {
// 	if (_channelcreated == false || node.empty()) return;
// 	RemoNet::CCCanMesage Req;
// 	Req.set_islidar(false);
// 	for (auto& it : node)
// 	{
// 		// if (it.second.canid == htonl(0x486))
// 		{
// 			auto m = Req.add_message();
// 			m->set_head(it.second.dlc);
// 			m->set_canid(it.second.canid);
// 			m->set_data(it.second.data, 8);
// 			//	 m->set_islidar(false);
// 		}
// 		// it->second.canid
// 	}
// 	MessageHead Head;
// 	CIOBuffer pBuffer;
// 	Head.Command = RemoNet::CC_CAN;
// 	Head.Length = Req.ByteSizeLong();
// 	Head.Serialize(pBuffer.Buffer);
// 	auto ptr = pBuffer.Buffer + MessageHead::Size();
// 	Req.SerializeToArray(ptr, Head.Length);
// 	pBuffer.Length = MessageHead::Size() + Head.Length;
// 	_peerconnection->SendData(&pBuffer);
// }
void CEgoWindow::Process(CIOBuffer* pBuffer)
{
	Message* message = reinterpret_cast<Message*>(pBuffer->Buffer);
	switch (message->cmd)
	{
	case WM_NOTIFY_REP:
		OnNotifyRep((int32_t)message->param_l);
		break;;
	case WM_NOTIFY_ANSWER:
		OnNotifyAnswer((CIOBuffer*)message->param_l);
		break;
	case WM_NOTIFY_LEAVE:
		OnNotifyLeave();
		break;
	case WM_NOTIFY_CANDIDATE:
		OnNotifyCandidate((CIOBuffer*)message->param_l);
		break;
	case WM_NOTIFY_OFFER:
		OnNotifyOffer((CIOBuffer*)message->param_l);
		break;
	}
}
void CEgoWindow::OnNotifyAnswer(CIOBuffer* pBuffer)
{
	AnswerDesc* desc = (AnswerDesc*)pBuffer->Buffer;
	const char* type = desc->type;
	const char* sdp = desc->sdp;
	_peerconnection->SetRemoteDescription(type, sdp);
	pBuffer->Release();
}
void CEgoWindow::OnNotifyOffer(CIOBuffer* pBuffer)
{
	OfferDesc* desc = (OfferDesc*)pBuffer->Buffer;
	auto type = desc->type;
	auto sdp = desc->sdp;
	_peerconnection->SetRemoteDescription(type, sdp);
	_peerconnection->CreateAnswer();
	pBuffer->Release();
}
void CEgoWindow::OnNotifyCandidate(CIOBuffer* pBuffer)
{
	CandidateDesc* desc = (CandidateDesc*)pBuffer->Buffer;
	auto candidate = desc->candidate;
	auto sdp_mid = desc->sdp_mid;
	auto sdp_mline_index = desc->sdp_mline_index;
	_peerconnection->AddIceCandidate(candidate, sdp_mline_index, sdp_mid);
	pBuffer->Release();
}
void CEgoWindow::OnNotifyLeave()
{
	if (!start_) return;
	start_ = false;
	StopPing();
//	_channelcreated = false;
	_peer = -1;
	_peerconnection->Close();
	_peerconnection.reset();
	_peerconnection = nullptr;
	image_ = nullptr;
	if(_render!=nullptr)
		_render->OnRender(image_,1280,720);
}
void CEgoWindow::DelayDataChannel()
{
	_delayThread = std::thread(&CEgoWindow::AskDataChannel, this);
	_delayThread.detach();
}
void CEgoWindow::DelayStartPing()
{
	_delayThread = std::thread(&CEgoWindow::StartPing, this);
	_delayThread.detach();
}
void CEgoWindow::OnIdle()
{
	if (!_ping) return;
	if (_pos==RenderPosition::FRONT)
	{
		int64_t tick=GetTickCount64();
		RemoNet::CCPing Req;
		Req.set_tick(tick);
		CIOBuffer Buffer;
		MessageHead Head;
		Head.Command = RemoNet::CC_Ping;
		Head.Length = Req.ByteSizeLong();
		Head.Serialize(Buffer.Buffer);
		auto ptr = Buffer.Buffer + MessageHead::Size();
		Req.SerializeToArray(ptr, Head.Length);
		Buffer.Length = Head.Length + MessageHead::Size();
		_peerconnection->SendData(&Buffer);
	 }
}
void CEgoWindow::StartPing()
{
	Sleep(300);
	_control = ControlState::Process;
	_ping = true;
}
void CEgoWindow::StopPing()
{
	_ping = false;
}
void CEgoWindow::AskDataChannel()
{
	Sleep(300);
	_peerconnection->SetDataReady();
	RemoNet::CCAskDataChannel Req;

	CIOBuffer Buffer;
	MessageHead Head;
	Head.Command = RemoNet::CC_ASKDATACHANNEL;
	Head.Length = Req.ByteSizeLong();
	Head.Serialize(Buffer.Buffer);
	auto ptr = Buffer.Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	Buffer.Length = Head.Length + MessageHead::Size();
	_peerconnection->SendData(&Buffer);
	//_channelcreated = true;
}
void CEgoWindow::DelayNextVideoReq()
{
	_delayThread = std::thread(&CEgoWindow::AskVideoReq, this);
	_delayThread.detach();
}
void CEgoWindow::AskVideoReq()
{
	Sleep(300);


	_client->WriteVideoReq(_peer, _pos);
}

void CEgoWindow::CreateVideoReq()
{
	_client->WriteVideoReq(_peer, _pos);
}
void CEgoWindow::SetPeer(int32_t peer)
{
	_peer = peer;
}
void CEgoWindow::SendData(CIOBuffer* pBuffer)
{
	_peerconnection->SendData(pBuffer);
}
ControlState CEgoWindow::GetControlState()
{
	return _control;;
}
void CEgoWindow::SetControlState(ControlState state)
{
	_control = state;
}
 