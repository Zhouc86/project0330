#pragma once
class CMultiMonitorEnumerator
{
public:
	typedef struct _tagVMonitorInfo_t
	{
		HMONITOR hMonitor; //��ʾ�����
		TCHAR szDevice[32]; //��ʾ����
		RECT rcVirtual; //������ʾ������
		RECT rcMonitor; //������ʾ����
		RECT rcWork; //������ʾ����
		BOOL bPrimary; //����ʾ����

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

