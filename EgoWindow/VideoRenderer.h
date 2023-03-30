#pragma once
class CVideoRenderer
{
public:
	CVideoRenderer();
	void SetRenderWindow(HWND wnd, int32_t width, int32_t height);
	void OnArgb32FrameReady(const void* data, const int stride, const int frame_width, const int frame_height);
	static LRESULT PaintProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lparam, UINT_PTR uIdSubclass,
		DWORD_PTR dwRefData);
	LRESULT OnPaintProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lparam, UINT_PTR uIdSubclass);
	void StartRender(bool bStart);
	static   void  FrameCallback(void* user_data,
		void* data,
		const int stride,
		const int frame_width,
		const int frame_height);
private:
	void SetSize(int32_t width, int32_t height);
	HWND wnd_;
	BITMAPINFO bmi_;
	std::unique_ptr<uint8_t> image_;
	std::mutex buffer_lock_;
	bool start_;
};

