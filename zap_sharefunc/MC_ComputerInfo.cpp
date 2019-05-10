#include "MC_ComputerInfo.h"
#include "MC_Log.h"

#include <Iphlpapi.h>
//---------------------------------------------------------------------------
bool MC_ComputerInfo::PowerOff(bool vRestart)
{
	HANDLE	tToken;
	LUID  	tLuid;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &tToken);
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tLuid);

	TOKEN_PRIVILEGES tPri;
	tPri.PrivilegeCount           = 1;
	tPri.Privileges[0].Luid       = tLuid;
	tPri.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(tToken, false, &tPri, sizeof(TOKEN_PRIVILEGES), 0, 0);

	if(true == vRestart)
	{
		ExitWindowsEx(EWX_REBOOT|EWX_FORCE, 0);
	}
	else
	{
		ExitWindowsEx(EWX_SHUTDOWN|EWX_FORCE, 0);
	}
	return true;
}
//---------------------------------------------------------------------------
bool MC_ComputerInfo::GetScreenResolution(int& vLateral, int& vVertical)
{
	vLateral	= GetSystemMetrics(SM_CXSCREEN);	
	vVertical	= GetSystemMetrics(SM_CYSCREEN); 
	if(0 == vLateral || 0 == vVertical)
		return false;

	////不包括任务栏等区域
	//vLateral	= GetSystemMetrics(SM_CXFULLSCREEN);
	//vVertical	= GetSystemMetrics(SM_CYFULLSCREEN);
	//if(0 == vLateral || 0 == vVertical)
	//	return false;

	////第二种
	//DEVMODE tDevMode = {0};
	//if(FALSE == EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &tDevMode))
	//	return false;

	//vLateral	= tDevMode.dmPelsWidth;
	//vVertical	= tDevMode.dmPelsHeight;

	return true;
}
//---------------------------------------------------------------------------
int MC_ComputerInfo::GetCpuUsedPercent(void)
{
	FILETIME tTempIdleTime		= {0};
	FILETIME tTempKernelTime	= {0};
	FILETIME tTempUserTime		= {0};

	ULARGE_INTEGER tPreIdleTime		= {0};
	ULARGE_INTEGER tPreKernelTime	= {0};
	ULARGE_INTEGER tPreUserTime		= {0};
	if(FALSE == GetSystemTimes(&tTempIdleTime,&tTempKernelTime,&tTempUserTime))
		return -1;
	tPreIdleTime.LowPart	= tTempIdleTime.dwLowDateTime;
	tPreIdleTime.HighPart	= tTempIdleTime.dwHighDateTime;
	tPreKernelTime.LowPart	= tTempKernelTime.dwLowDateTime;
	tPreKernelTime.HighPart	= tTempKernelTime.dwHighDateTime;
	tPreUserTime.LowPart	= tTempUserTime.dwLowDateTime;
	tPreUserTime.HighPart	= tTempUserTime.dwHighDateTime;

	Sleep(1000);

	ULARGE_INTEGER tCurIdleTime		= {0};
	ULARGE_INTEGER tCurKernelTime	= {0};
	ULARGE_INTEGER tCurUserTime		= {0};
	if(FALSE == GetSystemTimes(&tTempIdleTime,&tTempKernelTime,&tTempUserTime))
		return -1;
	tCurIdleTime.LowPart	= tTempIdleTime.dwLowDateTime;
	tCurIdleTime.HighPart	= tTempIdleTime.dwHighDateTime;
	tCurKernelTime.LowPart	= tTempKernelTime.dwLowDateTime;
	tCurKernelTime.HighPart	= tTempKernelTime.dwHighDateTime;
	tCurUserTime.LowPart	= tTempUserTime.dwLowDateTime;
	tCurUserTime.HighPart	= tTempUserTime.dwHighDateTime;

	//计算
	ULARGE_INTEGER tIdleTime	= {0};
	ULARGE_INTEGER tKernelTime	= {0};
	ULARGE_INTEGER tUserTime	= {0};
	tIdleTime.QuadPart		= tCurIdleTime.QuadPart - tPreIdleTime.QuadPart;
	tKernelTime.QuadPart	= tCurKernelTime.QuadPart - tPreKernelTime.QuadPart;
	tUserTime.QuadPart		= tCurUserTime.QuadPart - tCurUserTime.QuadPart;

	if(0 != (tKernelTime.QuadPart + tUserTime.QuadPart))
		return  (int)((tKernelTime.QuadPart + tUserTime.QuadPart - tIdleTime.QuadPart)*100 / (tKernelTime.QuadPart + tUserTime.QuadPart));
	
	return -1;
}
//---------------------------------------------------------------------------
DWORD MC_ComputerInfo::GetRamUserPercent(void)
{
	MEMORYSTATUSEX tMemoryInfo = {sizeof(MEMORYSTATUSEX)};
	GlobalMemoryStatusEx(&tMemoryInfo);

	printf("%f\n", (double)tMemoryInfo.ullTotalPhys / (1024*1024*1024));
	return tMemoryInfo.dwMemoryLoad;
}	
//---------------------------------------------------------------------------
std::string MC_ComputerInfo::GetMac(void)
{
	std::string tMacStr;
	ULONG		tOutBufLen			= sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO pAdapterInfo	= (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	if(pAdapterInfo == NULL)
		return false;

	//获取地址大小
	if(GetAdaptersInfo(pAdapterInfo, &tOutBufLen) == ERROR_BUFFER_OVERFLOW) 
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(tOutBufLen);
		if(pAdapterInfo == NULL) 
			return false;
	}

	if(GetAdaptersInfo(pAdapterInfo, &tOutBufLen) == NO_ERROR)
	{
		for(PIP_ADAPTER_INFO pAdapter = pAdapterInfo; pAdapter != NULL; pAdapter = pAdapter->Next)
		{
			// 确保是以太网
			if(pAdapter->Type != MIB_IF_TYPE_ETHERNET)
				continue;

			// 确保MAC地址的长度为 00-00-00-00-00-00
			if(pAdapter->AddressLength != 6)
				continue;

			char tMac[32];
			sprintf(tMac, "%02X:%02X:%02X:%02X:%02X:%02X",
				int (pAdapter->Address[0]),
				int (pAdapter->Address[1]),
				int (pAdapter->Address[2]),
				int (pAdapter->Address[3]),
				int (pAdapter->Address[4]),
				int (pAdapter->Address[5]));
			tMacStr = std::string(tMac);
			break;
		}
	}

	free(pAdapterInfo);
	return tMacStr;
}
//---------------------------------------------------------------------------