#pragma once
class CMultiMonitorEnumerator
{
public:
	typedef struct _tagVMonitorInfo_t
	{
		HMONITOR hMonitor; //显示器句柄
		TCHAR szDevice[32]; //显示器名
		RECT rcVirtual; //虚拟显示屏坐标
		RECT rcMonitor; //物理显示坐标
		RECT rcWork; //工作显示坐标
		BOOL bPrimary; //主显示器？

		_tagVMonitorInfo_t()
		{
			memset(this, 0, sizeof(*this));
		}
	}VMONITORINFO, * LPVMONITORINFO;
	static BOOL CALLBACK MonitorEnumProc(
		HMONITOR hMonitor,  // handle to display monitor
		HDC hdcMonitor,     // handle to monitor-appropriate device context
		LPRECT lprcMonitor, // pointer to monitor intersection rectangle
		LPARAM dwData       // data passed from EnumDisplayMonitors
	);
 
public:
	CMultiMonitorEnumerator();
public:
	int32_t m_iMontorNumber;
	VMONITORINFO m_MonitorInfoExArray[64];

};

