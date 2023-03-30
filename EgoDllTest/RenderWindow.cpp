#include "stdafx.h"
#include "RenderWindow.h"
CRenderWindow::CRenderWindow()
{

}
void CRenderWindow::Start()
{
	ZeroMemory(&bmi_, sizeof(bmi_));
	bmi_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi_.bmiHeader.biPlanes = 1;
	bmi_.bmiHeader.biBitCount = 32;
	bmi_.bmiHeader.biCompression = BI_RGB;
	bmi_.bmiHeader.biWidth = 1280;
	bmi_.bmiHeader.biHeight = -720;
	
	bmi_.bmiHeader.biSizeImage =
		1280 * 720 * (bmi_.bmiHeader.biBitCount >> 3);
	image_.reset(new uint8_t[bmi_.bmiHeader.biSizeImage]);
	m_Thread = std::thread(&CRenderWindow::Run, this);
}
void CRenderWindow::Run()
{
	m_bRun = true;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CRenderWindow::WndProc;
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
	 
	m_hWnd = CreateWindow(szApp, nullptr, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0, 0, 1280, 720, NULL, NULL, wcex.hInstance, 0);
	 
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (msg.message == WM_DESTROY) break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
LRESULT CRenderWindow::WndProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	switch (wMsg)
	{
	case WM_PAINT:
	{
		CRenderWindow* lThis = (CRenderWindow*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if (lThis != nullptr)
		{
			lThis->OnPaint(hWnd);
			return 1;
		}

	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
	break;
	}
	return  DefWindowProc(hWnd, wMsg, wParam, lParam);
}
void CRenderWindow::OnPaint(HWND hWnd)
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
void CRenderWindow::OnRender(std::unique_ptr<uint8_t>& pBuffer, int32_t width, int32_t height)
{
	SetSize(width, height);
	if (pBuffer==nullptr) return;
	memcpy(image_.get(), pBuffer.get(), bmi_.bmiHeader.biSizeImage);

	::InvalidateRect(m_hWnd, NULL, FALSE);
}
void CRenderWindow::SetSize(int32_t width, int32_t height)
{
	if (width == bmi_.bmiHeader.biWidth && height == -bmi_.bmiHeader.biHeight) {
		return;
	}
	bmi_.bmiHeader.biWidth = width;
	bmi_.bmiHeader.biHeight = -height;
	bmi_.bmiHeader.biSizeImage =
		width * height * (bmi_.bmiHeader.biBitCount >> 3);


	image_.reset(new uint8_t[bmi_.bmiHeader.biSizeImage]);
}