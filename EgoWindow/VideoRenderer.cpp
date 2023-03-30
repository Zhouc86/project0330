#include "stdafx.h"
#include <memory>
#include <mutex>
#include "VideoRenderer.h"
#include "VideoRenderer.h"



CVideoRenderer::CVideoRenderer()
{
	start_ = false;
}
void CVideoRenderer::SetRenderWindow(HWND wnd, int32_t width, int32_t height)
{
	if (!IsWindow(wnd)) return;

	wnd_ = wnd;
	ZeroMemory(&bmi_, sizeof(bmi_));
	bmi_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi_.bmiHeader.biPlanes = 1;
	bmi_.bmiHeader.biBitCount = 32;
	bmi_.bmiHeader.biCompression = BI_RGB;
	bmi_.bmiHeader.biWidth = width;
	bmi_.bmiHeader.biHeight = -height;
	bmi_.bmiHeader.biSizeImage =
		width * height * (bmi_.bmiHeader.biBitCount >> 3);
 

}

void CVideoRenderer::StartRender(bool bStart)
{
	start_ = bStart;
}
LRESULT CVideoRenderer::PaintProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lparam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	CVideoRenderer* lhs = reinterpret_cast<CVideoRenderer*>(dwRefData);
	return lhs->OnPaintProc(hWnd, msg, wParam, lparam, uIdSubclass);
}
LRESULT CVideoRenderer::OnPaintProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lparam, UINT_PTR uIdSubclass)
{
	if (start_ == false) return ::DefWindowProc(wnd_, msg, wParam, lparam);
	if (start_ && msg == WM_PAINT)
	{
		PAINTSTRUCT ps;
		::BeginPaint(hWnd, &ps);
		RECT rc;
		::GetClientRect(hWnd, &rc);
		//const uint8_t* image = 
		//AutoLock<VideoRenderer> local_lock(renderer);
		//const BITMAPINFO& bmi = renderer->bmi();
		std::lock_guard < std::mutex> l(buffer_lock_);
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
		::EndPaint(hWnd, &ps);
	}
	return ::DefWindowProc(wnd_, msg, wParam, lparam);
}
void CVideoRenderer::SetSize(int32_t width, int32_t height)
{

	if (width == bmi_.bmiHeader.biWidth && height == bmi_.bmiHeader.biHeight) {
		return;
	}

	bmi_.bmiHeader.biWidth = width;
	bmi_.bmiHeader.biHeight = -height;
	bmi_.bmiHeader.biSizeImage =
		width * height * (bmi_.bmiHeader.biBitCount >> 3);


	image_.reset(new uint8_t[bmi_.bmiHeader.biSizeImage]);
}
void CVideoRenderer::OnArgb32FrameReady(const void* data, const int stride, const int frame_width, const int frame_height)
{
	SetSize(frame_width, frame_height);
	memcpy(image_.get(), data, bmi_.bmiHeader.biSizeImage);
	::InvalidateRect(wnd_, NULL, FALSE);
}
void  CVideoRenderer::FrameCallback(void* user_data, void* data, const int stride, const int frame_width, const int frame_height)
{
	auto ptr = static_cast<CVideoRenderer*>(user_data);
	ptr->OnArgb32FrameReady(data, stride, frame_width, frame_height);
}