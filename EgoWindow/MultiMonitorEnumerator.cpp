#include "stdafx.h"

#include "MultiMonitorEnumerator.h"
CMultiMonitorEnumerator::CMultiMonitorEnumerator()
{
	m_iMontorNumber = 0;
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)this);
}
BOOL CALLBACK CMultiMonitorEnumerator::MonitorEnumProc( HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData )
{
	CMultiMonitorEnumerator* pThis = (CMultiMonitorEnumerator*)dwData;

	//The rectangle coordinates are virtual-screen coordinates
	if (lprcMonitor)
	{
		// 			TRACE(_T("%p (l=%d t=%d r=%d b=%d)\n"),
		// 				hMonitor,
		// 				lprcMonitor->left, lprcMonitor->top,
		// 				lprcMonitor->right, lprcMonitor->bottom);
	}

	//GetMonitorInfo 获取显示器信息
	MONITORINFOEX infoEx;
	memset(&infoEx, 0, sizeof(infoEx));
	infoEx.cbSize = sizeof(infoEx);
	if (GetMonitorInfo(hMonitor, &infoEx))
	{
		// 			TRACE(_T("%p [%s] %s rcMonitor(l=%d t=%d r=%d b=%d) rcWork(l=%d t=%d r=%d b=%d)\n"),
		// 				hMonitor,
		// 				infoEx.szDevice,
		// 				infoEx.dwFlags == MONITORINFOF_PRIMARY ? _T("Primary") : _T("Slave"),
		// 				infoEx.rcMonitor.left, infoEx.rcMonitor.top, infoEx.rcMonitor.right, infoEx.rcMonitor.bottom,
		// 				infoEx.rcWork.left, infoEx.rcWork.top, infoEx.rcWork.right, infoEx.rcWork.bottom);
	}

	//保存显示器信息
	if (pThis->m_iMontorNumber < _countof(pThis->m_MonitorInfoExArray) - 1)
	{
		VMONITORINFO* pInfo = &pThis->m_MonitorInfoExArray[pThis->m_iMontorNumber];
		pInfo->hMonitor = hMonitor;
		if (lprcMonitor)
		{
			pInfo->rcVirtual = *lprcMonitor;
		}
		pInfo->rcMonitor = infoEx.rcMonitor;
		pInfo->rcWork = infoEx.rcWork;
		pInfo->bPrimary = infoEx.dwFlags == MONITORINFOF_PRIMARY;
		_tcscpy_s(pInfo->szDevice, infoEx.szDevice);

		pThis->m_iMontorNumber++;

		return TRUE; //continue the enumeration
	}

	return FALSE; //stop the enumeration
}