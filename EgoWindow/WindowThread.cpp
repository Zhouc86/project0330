#include "stdafx.h"
#include <thread>
#include <mutex>
#include <memory>
#include <math.h>
#include "api.h"
#include "Protocol.pb.h"
 
#include "../common/peer_connection.h"
 

#include "../common/socket_client.h"
#include "../common/iobuffer.h"
 
#include "WindowThread.h"
#include "MultiMonitorEnumerator.h"
#include "libyuv/convert_argb.h"
#include "libyuv/rotate.h"
#define  PI 3.141526f

CThreadWindow::CThreadWindow( RenderPosition pos,SocketClient * c): m_Pos(pos),m_Client(c)
{
	start_ = false;
	_control = ControlState::Check;
	//bDataChannelCreated = false;
}
void CThreadWindow::Start()
{
	m_Thread = std::thread(&CThreadWindow::Run, this);
}

void CThreadWindow::Run()
{
	m_bRun = true;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CThreadWindow::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(nullptr);
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;

	char cName[MAX_PATH] = { 0 };
	GetModuleFileNameA(wcex.hInstance, cName, sizeof(cName));
	char* szApp = strrchr(cName, '\\') + 1;
	strchr(szApp, '.')[0] = '\0';

	wcex.lpszClassName = szApp;
	wcex.hIconSm = 0;
	RegisterClassEx(&wcex);
	CMultiMonitorEnumerator Enumerator;
 	if(Enumerator.m_iMontorNumber !=3 )
 		m_hWnd= CreateWindow(szApp, nullptr, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0, 0, 1280, 720, NULL, NULL, wcex.hInstance, 0);
 	else
	m_hWnd= CreateWindow(szApp, nullptr, WS_POPUP, 0, 0, 1024, 768, NULL, NULL, wcex.hInstance, 0);
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
	ShowWindow(m_hWnd,SW_SHOW);
	UpdateWindow(m_hWnd);
	
	
	switch (m_Pos)
	{
	case RenderPosition::FRONT_BACK:
	 {
	//	CMultiMonitorEnumerator::VMONITORINFO* pvInfo = &Enumerator.m_MonitorInfoExArray[2];
		::SetWindowTextW(m_hWnd, L"前视图");
		//if (Enumerator.m_iMontorNumber == 3)
		{
			//::MoveWindow(m_hWnd, pvInfo->rcMonitor.left, pvInfo->rcMonitor.top, 1920, 1280, true);
		}
	 }
	 
	break;
	case RenderPosition::LEFT:
	{
	//	CMultiMonitorEnumerator::VMONITORINFO* pvInfo = &Enumerator.m_MonitorInfoExArray[0];
		::SetWindowTextW(m_hWnd, L"左视图");
	//	::MoveWindow(m_hWnd, pvInfo->rcMonitor.left + 1152, pvInfo->rcMonitor.top, 768, 1024, true);
	}
	break;
	case RenderPosition::RIGHT:
	{
	//	CMultiMonitorEnumerator::VMONITORINFO* pvInfo = &Enumerator.m_MonitorInfoExArray[1];
		::SetWindowTextW(m_hWnd, L"右视图");
	//	::MoveWindow(m_hWnd, pvInfo->rcMonitor.left, pvInfo->rcMonitor.top, 768, 1024, true);
	}
	break;
	case RenderPosition::DASHBOARD:
	{
		//	CMultiMonitorEnumerator::VMONITORINFO* pvInfo = &Enumerator.m_MonitorInfoExArray[1];
		::SetWindowTextW(m_hWnd, L"仪表盘");
		//	::MoveWindow(m_hWnd, pvInfo->rcMonitor.left, pvInfo->rcMonitor.top, 768, 1024, true);
	}
	break;
	}
	

	
	

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if(msg.message==WM_DESTROY) break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
LRESULT CThreadWindow::WndProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	switch (wMsg)
	{
	case WM_PAINT:
	{
		CThreadWindow* lThis = (CThreadWindow*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if (lThis != nullptr)
		{
			lThis->OnPaint(hWnd);
			return 1;
		}
		
	}
	break;
	case WM_NOTIFY_REP:
	{
		CThreadWindow* lThis =(CThreadWindow *)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		lThis-> OnVideoRep((int32_t)wParam);
	}
	break;
	case WM_NOTIFY_ANSWER:
	{
		CThreadWindow* lThis = (CThreadWindow*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		lThis->OnVideoAnswer((CIOBuffer*)wParam);
	}
	break;
	case WM_NOTIFY_CANDIDATE:
	{
		CThreadWindow* lThis = (CThreadWindow*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		lThis->OnVideoCandidate((CIOBuffer*)wParam);
	}
	break;
	case WM_NOTIFY_OFFER:
	{
		CThreadWindow* lThis = (CThreadWindow*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		lThis->OnVideoOffer((CIOBuffer*)wParam);
	}
	break;
	//case WM_ERASEBKGND:
	//{
	//	return 1;
	//}
	//break;
	case WM_NOTIFY_LEAVE:
	{
		CThreadWindow * lThis= (CThreadWindow*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		lThis->OnVideoLeave();
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	 
		 
	}
	return  DefWindowProc(hWnd, wMsg, wParam, lParam);
}
void CThreadWindow::Stop()
{
	PostMessage(WM_DESTROY, 0, 0);
	m_Thread.join();
}
void CThreadWindow::PostMessage(UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	::PostMessage(m_hWnd, wMsg, wParam, lParam);
}
void CThreadWindow::OnVideoRep(int32_t peer)
{

	int32_t width = 1280;
	int32_t height = 720;
	//auto type = m_Pos < RenderPosition::CAR_FRONT ? EgoType::Cave : EgoType::Car;
	m_PeerConnection = std::make_unique<CPeerConnection>((ChannelType)m_Pos, m_Client);
	//m_VideoRender = std::make_unique<CVideoRenderer>();
	SetRenderWindow(width, height);
	InitPeerConnection(peer);
	m_PeerConnection->CreateOffer();
}
void CThreadWindow::OnVideoLeave()
{
	if (!start_) return;
	start_ = false;
	_control = ControlState::Check;
	//bDataChannelCreated = false;
	m_Peer = -1;
	m_PeerConnection->Close();
	m_PeerConnection.reset();
	m_PeerConnection = nullptr; 
	image_ = nullptr;
	::InvalidateRect(m_hWnd, NULL, TRUE);
}
void CThreadWindow::InitPeerConnection(int32_t peer)
{
	 
	m_PeerConnection->Initialize(peer, m_Pos);
	m_PeerConnection->AddDataChannel(true, false);
	m_PeerConnection->AddRemoteArgb32VideoFrameReady(&CThreadWindow::FrameCallback, this);

 
	StartRender(true);
}
void CThreadWindow::SetPeer(int32_t peer)
{
	m_Peer = peer;
}
void CThreadWindow::CreateVideoReq()
{
	 
	//auto type = m_Pos < CameraPosition::CAR_FRONT ? EgoType::Cave : EgoType::Car;
	m_Client->WriteVideoReq(m_Peer, m_Pos);
}
void CThreadWindow::OnVideoAnswer(CIOBuffer* pBuffer)
{
	AnswerDesc* desc = (AnswerDesc*)pBuffer->Buffer;
	const char* type = desc->type;
	const char* sdp = desc->sdp;
	m_PeerConnection->SetRemoteDescription(type, sdp);
	pBuffer->Release();
	 
}
void CThreadWindow::OnVideoCandidate(CIOBuffer* pBuffer)
{
	CandidateDesc* desc = (CandidateDesc*)pBuffer->Buffer;
	auto candidate = desc->candidate;
	auto sdp_mid = desc->sdp_mid;
	auto sdp_mline_index = desc->sdp_mline_index;
	m_PeerConnection->AddIceCandidate(candidate, sdp_mline_index, sdp_mid);
	pBuffer->Release();
}
void CThreadWindow::OnVideoOffer(CIOBuffer* pBuffer)
{
	OfferDesc* desc = (OfferDesc*)pBuffer->Buffer;
	auto type = desc->type;	
	auto sdp = desc->sdp;
	m_PeerConnection->SetRemoteDescription(type, sdp);
	m_PeerConnection->CreateAnswer();
	pBuffer->Release();
}
/*
void CThreadWindow::WriteCanMessage(std::unordered_map<int32_t, cannet_frame>& node,bool)
{
	if (bDataChannelCreated == false||node.empty()) return;
	 RemoNet::CCCanMesage Req;
	 Req.set_islidar(false);
	 for (auto& it:node)
	 {
		// if (it.second.canid == htonl(0x486))
		 {
			 auto m = Req.add_message();
			 m->set_head(it.second.header);
			 m->set_canid(it.second.canid);
			 m->set_data(it.second.data, 8);
		//	 m->set_islidar(false);
		 }
		// it->second.canid
	 }
	 MessageHead Head;
	 CIOBuffer pBuffer;
	 Head.Command = RemoNet::CC_CAN;
	 Head.Length = Req.ByteSizeLong();
	 Head.Serialize(pBuffer.Buffer);
	 auto ptr = pBuffer.Buffer + MessageHead::Size();
	 Req.SerializeToArray(ptr, Head.Length);
	 pBuffer.Length = MessageHead::Size() + Head.Length;
	 m_PeerConnection->SendData(&pBuffer);
}
*/
void CThreadWindow::SetRenderWindow(int32_t width, int32_t height)
{
	ZeroMemory(&bmi_, sizeof(bmi_));
	bmi_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi_.bmiHeader.biPlanes = 1;
	bmi_.bmiHeader.biBitCount = 32;
	bmi_.bmiHeader.biCompression = BI_RGB;
  	if (m_Pos == RenderPosition::LEFT || m_Pos == RenderPosition::RIGHT)
  	{
  		bmi_.bmiHeader.biWidth = height;
  		bmi_.bmiHeader.biHeight = -width;
  	}
  	else{
		bmi_.bmiHeader.biWidth = width;
		bmi_.bmiHeader.biHeight = -height;
	}
	bmi_.bmiHeader.biSizeImage =
		width * height * (bmi_.bmiHeader.biBitCount >> 3);
	image_.reset(new uint8_t[bmi_.bmiHeader.biSizeImage]);
	if(m_Pos==RenderPosition::LEFT||m_Pos== RenderPosition::RIGHT)
		_rotate.reset(new uint8_t[bmi_.bmiHeader.biSizeImage]);

}
void  CThreadWindow::FrameCallback(void* user_data,
	const uint8_t* yptr, int32_t strideY,
	const uint8_t* uptr, int32_t strideU, 
	const uint8_t* vptr, int32_t strideV,
	const int32_t stride, const int frame_width, 
	const int frame_height)
{
	auto ptr = static_cast<CThreadWindow*>(user_data);
	 
	ptr->OnArgb32FrameReady(yptr, strideY, uptr,strideU,vptr,strideV,stride, frame_width, frame_height);
}
void CThreadWindow::StartRender(bool b)
{
	start_ = b;
}
void CThreadWindow::OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: Add any drawing code that uses hdc here...
	if (start_)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);
		//const uint8_t* image = 
		//AutoLock<VideoRenderer> local_lock(renderer);
		//const BITMAPINFO& bmi = renderer->bmi();
	//	std::lock_guard < std::mutex> l(buffer_lock_);
		int height = abs(bmi_.bmiHeader.biHeight);
		int width = bmi_.bmiHeader.biWidth;
		HDC dc_mem = ::CreateCompatibleDC(ps.hdc);
		::SetStretchBltMode(dc_mem, HALFTONE);
		HDC all_dc[] = { ps.hdc, dc_mem };
		for (size_t i = 0; i < _countof(all_dc); ++i) {
			SetMapMode(all_dc[i], MM_ISOTROPIC);
			SetWindowExtEx(all_dc[i], width, height, NULL);
			SetViewportExtEx(all_dc[i], rc.right, rc.bottom, NULL);
		}
		HBITMAP bmp_mem = ::CreateCompatibleBitmap(ps.hdc, rc.right, rc.bottom);
		HGDIOBJ bmp_old = ::SelectObject(dc_mem, bmp_mem);

		POINT logical_area = { rc.right, rc.bottom };
		DPtoLP(ps.hdc, &logical_area, 1);

		HBRUSH brush = ::CreateSolidBrush(RGB(0, 0, 0));
		RECT logical_rect = { 0, 0, logical_area.x, logical_area.y };
		::FillRect(dc_mem, &logical_rect, brush);
		::DeleteObject(brush);
		int x = (logical_area.x / 2) - (width / 2);
		int y = (logical_area.y / 2) - (height / 2);

		StretchDIBits(dc_mem, x, y, width, height, 0, 0, width, height, image_.get(),
			&bmi_, DIB_RGB_COLORS, SRCCOPY);

		BitBlt(ps.hdc, 0, 0, logical_area.x, logical_area.y, dc_mem, 0, 0,
			SRCCOPY);

		// Cleanup.
		::SelectObject(dc_mem, bmp_old);
		::DeleteObject(bmp_mem);
		::DeleteDC(dc_mem);
	}
	EndPaint(hWnd, &ps);
}
void CThreadWindow::OnArgb32FrameReady(const uint8_t* yptr,int32_t strideY, const uint8_t* uptr,int32_t strideU, const uint8_t* vptr,int32_t strideV, const int32_t stride,const int width, const int height)
{
	{
	//	int32_t*pixels = (int32_t*)data;
		
		//std::lock_guard < std::mutex> l(buffer_lock_);
		
		
		if (RenderPosition::LEFT == m_Pos)
		{
			SetSize(height,width);
			uint8_t* i420_image_rotated = _rotate.get();////(uint8_t*)malloc(width * height * 3 ) ;
			uint8_t* i420_image_rotated_y_ptr = i420_image_rotated;
			uint8_t* i420_image_rotated_u_ptr = i420_image_rotated_y_ptr + (height * width);
			uint8_t* i420_image_rotated_v_ptr = i420_image_rotated_u_ptr + (int)(height * width * 0.25);
			 
				 
  			libyuv::I420Rotate(yptr, width, uptr, width>>1, vptr, width>>1, i420_image_rotated_y_ptr,
				height,
				i420_image_rotated_u_ptr,
				height >>1,
				i420_image_rotated_v_ptr,
				height >>1,
				width,
				height, libyuv::RotationMode::kRotate270);
			yptr = i420_image_rotated_y_ptr;
			strideY = height;
			uptr = i420_image_rotated_u_ptr;
			strideU = height >> 1;
			vptr = i420_image_rotated_v_ptr;
			strideV = height >> 1;
			libyuv::I420ToARGB(yptr, strideY, uptr, strideU, vptr, strideV, image_.get(), height*4, height, width );
			 
		}
		else if (RenderPosition::RIGHT == m_Pos)
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
			SetSize(width,height);
			libyuv::I420ToARGB(yptr, strideY, uptr, strideU, vptr, strideV, image_.get(), stride, width, height);
		}
		 
		
	}
	 
	  ::InvalidateRect(m_hWnd, NULL, FALSE);
}
void CThreadWindow::SetSize(int32_t width, int32_t height)
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
	if (m_Pos == RenderPosition::LEFT || m_Pos == RenderPosition::RIGHT)
		_rotate.reset(new uint8_t[bmi_.bmiHeader.biSizeImage]);
}
void CThreadWindow::DelayNextVideoReq()
{
	 
	m_DelayThread = std::thread(&CThreadWindow::AskVideoReq, this);
	m_DelayThread.detach();
}
void CThreadWindow::DelayDataChannel()
{
	m_DelayThread = std::thread(&CThreadWindow::AskDataChannel, this);
	m_DelayThread.detach();
}
void CThreadWindow::DelayControlStart()
{
	m_DelayThread = std::thread(&CThreadWindow::AskControlStart, this);
	m_DelayThread.detach();
}
void CThreadWindow::AskVideoReq()
{
	Sleep(500);
	 
	 
	m_Client->WriteVideoReq(m_Peer, m_Pos);
}
void CThreadWindow::AskDataChannel()
{
	Sleep(500);
	//bDataChannelCreated = true;
	RemoNet::CCAskDataChannel Req;
	m_PeerConnection->SetDataReady();
	CIOBuffer Buffer;
	MessageHead Head;
	Head.Command = RemoNet::CC_ASKDATACHANNEL;
	Head.Length = Req.ByteSize();
	Head.Serialize(Buffer.Buffer);
	auto ptr = Buffer.Buffer + MessageHead::Size();
	Req.SerializeToArray(ptr, Head.Length);
	Buffer.Length = Head.Length + MessageHead::Size();
    SendData(&Buffer);
	
	
}
void CThreadWindow::AskControlStart()
{
	Sleep(500);
	_control = ControlState::Process;
}
void CThreadWindow::SendData(CIOBuffer* pBuffer)
{
	m_PeerConnection->SendData(pBuffer);
}
ControlState CThreadWindow::GetControlState()
{
	return _control;
}
/*
void CThreadWindow::Notify(int8_t* buffer, int32_t size)
{

}
void CThreadWindow::SetCanSocket(SocketCan* can)
{
	
}
*/