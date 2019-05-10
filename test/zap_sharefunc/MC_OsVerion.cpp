#include "MC_OsVerion.h"
#include "MC_Log.h"
//---------------------------------------------------------------------------
std::string MC_OsVerion::GetOsNameByReg()
{
	//打开注册表  
	HKEY	tKey;  
	LSTATUS	tRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0,  KEY_ALL_ACCESS,  &tKey);  
	if(tRet != ERROR_SUCCESS)  
	{
		LOG_ERROR("RegOpenKeyEx fail! errno: %d", GetLastError());
		return "";  
	}

	//获取CurrentVersion
	char	tCurrentVersion[100]	= {0};  
	DWORD	tSize					= 100;  
	DWORD	tType					= 0;  

	tRet = RegQueryValueExA(tKey, "CurrentVersion", NULL, &tType, (BYTE*)tCurrentVersion, &tSize);  
	if(tRet != ERROR_SUCCESS)  
	{
		LOG_ERROR("RegQueryValueEx fail! errno: %d", GetLastError());
		return "";  
	}

	std::string tOsName = "Unknow OS";
	if		(0 == strncmp(tCurrentVersion, "5.1", 3))		tOsName = "The OS is XP";
	else if	(0 == strncmp(tCurrentVersion, "6.0", 3))		tOsName = "The OS is vista and 2008";
	else if	(0 == strncmp(tCurrentVersion, "6.1", 3))		tOsName = "The OS is Win7";
	else if	(0 == strncmp(tCurrentVersion, "6.2", 3))		tOsName = "The OS is Win8";
	else if	(0 == strncmp(tCurrentVersion, "6.3", 3))		tOsName = "The OS is Win10";
	else													tOsName = "Unknow OS";

	return tOsName;
}
//---------------------------------------------------------------------------
std::string MC_OsVerion::GetOsNameByApi()
{
	SYSTEM_INFO tSystemInfo;        //用SYSTEM_INFO结构判断64位AMD处理器   
	GetSystemInfo(&tSystemInfo);    //调用GetSystemInfo函数填充结构   

	OSVERSIONINFOEX tOsVerInfo;   
	tOsVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);     

	string tOsName = "Unknow OS";  
	if(GetVersionEx((OSVERSIONINFO *)&tOsVerInfo))  
	{   
		//下面根据版本信息判断操作系统名称   
		switch(tOsVerInfo.dwMajorVersion)//判断主版本号  
		{  
		case 4:  
			switch(tOsVerInfo.dwMinorVersion)//判断次版本号   
			{   
			case 0:  
				if(VER_PLATFORM_WIN32_NT == tOsVerInfo.dwPlatformId)  
					tOsName = "Microsoft Windows NT 4.0"; //1996年7月发布   
				else if(VER_PLATFORM_WIN32_WINDOWS == tOsVerInfo.dwPlatformId)  
					tOsName = "Microsoft Windows 95";  
				break;  

			case 10:  
				tOsName = "Microsoft Windows 98";  
				break;  

			case 90:  
				tOsName = "Microsoft Windows Me";  
				break;  
			}  
			break;  

		case 5:  
			switch(tOsVerInfo.dwMinorVersion)		
			{   
			case 0:  
				tOsName = "Microsoft Windows 2000";	//1999年12月发布  
				break;  

			case 1:  
				tOsName = "Microsoft Windows XP";	//2001年8月发布  
				break;  

			case 2:  
				if(VER_NT_WORKSTATION == tOsVerInfo.wProductType && PROCESSOR_ARCHITECTURE_AMD64 == tSystemInfo.wProcessorArchitecture)  
					tOsName = "Microsoft Windows XP Professional x64 Edition";  
				else if(0 == GetSystemMetrics(SM_SERVERR2))  
					tOsName = "Microsoft Windows Server 2003";//2003年3月发布   
				else if(0 != GetSystemMetrics(SM_SERVERR2))  
					tOsName = "Microsoft Windows Server 2003 R2";  
				break;  
			}  
			break;  

		case 6:  
			switch(tOsVerInfo.dwMinorVersion)  
			{  
			case 0:  
				if(VER_NT_WORKSTATION == tOsVerInfo.wProductType)  
					tOsName = "Microsoft Windows Vista";  
				else  
					tOsName = "Microsoft Windows Server 2008";//服务器版本   
				break;  
			case 1:  
				if(VER_NT_WORKSTATION == tOsVerInfo.wProductType)  
					tOsName = "Microsoft Windows 7";  
				else  
					tOsName = "Microsoft Windows Server 2008 R2";  
				break;  
			case 2:  
				if(VER_NT_WORKSTATION == tOsVerInfo.wProductType)  
					tOsName = "Microsoft Windows 8";  
				else  
					tOsName = "Microsoft Windows Server 2012";  
				break;  
			case 3:  
				if(VER_NT_WORKSTATION == tOsVerInfo.wProductType)  
					tOsName = "Microsoft Windows 8.1";  
				else  
					tOsName = "Microsoft Windows Server 2012 R2";  
				break;  
			}  
			break;  

		case 10:  
			switch(tOsVerInfo.dwMinorVersion)  
			{  
			case 0:  
				if(VER_NT_WORKSTATION == tOsVerInfo.wProductType)  
					tOsName = "Microsoft Windows 10";  
				else  
					tOsName = "Microsoft Windows Server 2016 Technical Preview";//服务器版本   
				break;  
			}  
			break;  
		}  
	}
	//https://msdn.microsoft.com/en-us/library/ms724832.aspx  
	return tOsName;  
}
//---------------------------------------------------------------------------
bool MC_OsVerion::IsWin7OS()
{
	OSVERSIONINFOEX  tVersion;
	tVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX );
	if(0 == GetVersionEx((OSVERSIONINFO *)&tVersion))
	{
		LOG_ERROR("GetVersionEx errno: %d", GetLastError());
		return false;
	}

	LOG_INFO("dwMajorVersion: %d, dwMinorVersion: %d", tVersion.dwMajorVersion, tVersion.dwMinorVersion);
	if(tVersion.dwMajorVersion == 6 && tVersion.dwMinorVersion == 1)
	{
		if(tVersion.wProductType == 1)
		{
			LOG_INFO("the os is Win7");
			return true;
		}

		else
		{
			LOG_INFO("the os is server 2008");
			return false;
		}
	}

	return false;
}
//---------------------------------------------------------------------------
BOOL MC_OsVerion::Is64System()
{
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;
	BOOL bIsWow64 = FALSE;
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress( GetModuleHandleA("kernel32"),"IsWow64Process");
	if (NULL != fnIsWow64Process)
	{
		fnIsWow64Process(GetCurrentProcess(),&bIsWow64);
	}
	return bIsWow64;
}
//---------------------------------------------------------------------------