#pragma once
#include "EgoInterface.h"
#include <thread>
class CRenderWindow:public IRender
{
public:
	CRenderWindow();
	void Start();
	void Stop();
	virtual void OnRender(std::unique_ptr<uint8_t>& pBuffer,int32_t width,int32_t height) override;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
	void OnPaint(HWND hWnd);
	void SetSize(int32_t width, int32_t height);
private:
	void Run();
	HWND m_hWnd;
	std::thread m_Thread;
	bool m_bRun;
	BITMAPINFO bmi_;
	bool start_;
	std::unique_ptr<uint8_t> image_;
};

