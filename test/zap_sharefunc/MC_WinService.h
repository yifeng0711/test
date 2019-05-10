#ifndef MC_WinService_H
#define MC_WinService_H
//---------------------------------------------------------------------------
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//---------------------------------------------------------------------------
#include <Windows.h>
#include <string>
using namespace std;
//---------------------------------------------------------------------------
class MC_WinService
{
public:
	static	bool            InstallService      		(std::string vServiceName, std::string vDisplayName, std::string vServicePath);
	static	bool            UninstallService    		(std::string vServiceName);

	static	bool            MyStartService      		(std::string vServiceName);
	static	bool            MyStopService       		(std::string vServiceName);

	static	bool            IsServiceInstalled  		(std::string vServiceName);
	static	bool            IsServiceRunning    		(std::string vServiceName);

	static	std::string		GetServicePath				(std::string vServiceName);	//返回值为：服务属性- 常规 - 可执行文件的路径 
	static	std::string		GetServiceNameByPid			(DWORD vPid);
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------