#include "MC_WinService.h"
#include "MC_Log.h"

#include <vector>
//---------------------------------------------------------------------------
bool MC_WinService::InstallService(std::string vServiceName, std::string vDisplayName, std::string vServicePath)
{
	if(true == vServiceName.empty())	return false;
	if(true == vDisplayName.empty())	return false;
	if(true == vServicePath.empty())	return false;

	SC_HANDLE tSCHandle = ::OpenSCManager(NULL,  NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == tSCHandle)
	{
		LOG_ERROR("connect to service manager fail! errno: %d", GetLastError());
		return false;
	}

	SC_HANDLE tSvcHandle = CreateServiceA(tSCHandle, vServiceName.c_str(), vDisplayName.c_str(), SERVICE_ALL_ACCESS ,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START,SERVICE_ERROR_NORMAL,
		vServicePath.c_str(),
		NULL,NULL,NULL,NULL,NULL);

	if(NULL == tSvcHandle)
	{
		LOG_ERROR("install service fail! name: %s, path: %s, errno: %d", vServiceName.c_str(), vServicePath.c_str(), GetLastError());
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	::CloseServiceHandle(tSvcHandle);
	::CloseServiceHandle(tSCHandle);

	return true;
}
//---------------------------------------------------------------------------
bool MC_WinService::UninstallService(std::string vServiceName)
{
	if(true == vServiceName.empty())
		return false;

	SC_HANDLE tSCHandle = ::OpenSCManager(NULL,  NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == tSCHandle)
	{
		LOG_ERROR("connect to service manager fail! errno: %d", GetLastError());
		return false;
	}

	SC_HANDLE tSvcHandle = ::OpenServiceA(tSCHandle, vServiceName.c_str(), SERVICE_ALL_ACCESS);
	if(NULL == tSvcHandle)
	{
		LOG_ERROR("open service fail! errno: %d", GetLastError());
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	SERVICE_STATUS tStatus;
	if(FALSE == ::QueryServiceStatus(tSvcHandle, &tStatus))
	{
		LOG_ERROR("query service status fail! errno: %d", GetLastError());
		::CloseServiceHandle(tSvcHandle);
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	if(tStatus.dwCurrentState != SERVICE_STOPPED)
	{
		if(FALSE == ::ControlService(tSvcHandle, SERVICE_CONTROL_STOP, &tStatus))
		{
			LOG_ERROR("contorl service with stop fail! errno: %d", GetLastError());
			::CloseServiceHandle(tSvcHandle);
			::CloseServiceHandle(tSCHandle);
			return false;
		}

		while(TRUE == ::QueryServiceStatus(tSvcHandle, &tStatus))
		{
			::Sleep(1000);
			if(SERVICE_STOPPED == tStatus.dwCurrentState)
			{
				break;
			}
		}
	}
	if(false == DeleteService(tSvcHandle))
	{
		LOG_ERROR("delete service fail! errno: %d", GetLastError());
		::CloseServiceHandle(tSvcHandle);
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	::CloseServiceHandle(tSvcHandle);
	::CloseServiceHandle(tSCHandle);

	return true;
}
//---------------------------------------------------------------------------
bool MC_WinService::MyStartService(std::string vServiceName)
{
	if(true == vServiceName.empty())
		return false;

	SC_HANDLE tSCHandle = ::OpenSCManager(NULL,  NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == tSCHandle)
	{
		LOG_ERROR("connect to service manager fail! errno: %d", GetLastError());
		return false;
	}

	SC_HANDLE tSvcHandle = ::OpenServiceA(tSCHandle, vServiceName.c_str(), SERVICE_ALL_ACCESS|DELETE);
	if(NULL == tSvcHandle)
	{
		LOG_ERROR("open service fail! errno: %d", GetLastError());
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	SERVICE_STATUS tStatus;
	if(FALSE == ::QueryServiceStatus(tSvcHandle, &tStatus))
	{
		LOG_ERROR("query service status fail! errno: %d", GetLastError());
		::CloseServiceHandle(tSvcHandle);
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	if(SERVICE_RUNNING == tStatus.dwCurrentState)
	{
		//gLog.Log("the state of service to start is not SERVICE_STOPPED [%d]", tStatus.dwCurrentState);
		LOG_ERROR("tStatus.dwCurrentState = %d", tStatus.dwCurrentState);
		::CloseServiceHandle(tSvcHandle);
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	if(FALSE == ::StartServiceA(tSvcHandle, NULL, NULL))
	{
		LOG_ERROR("start service fail! errno: %d", GetLastError());
		::CloseServiceHandle(tSvcHandle);
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	while(TRUE == ::QueryServiceStatus(tSvcHandle, &tStatus))
	{
		::Sleep(1000);
		if(SERVICE_RUNNING == tStatus.dwCurrentState)
		{
			break;
		}
	}

	::CloseServiceHandle(tSvcHandle);
	::CloseServiceHandle(tSCHandle);
	return true;
}
//---------------------------------------------------------------------------
bool MC_WinService::MyStopService(std::string vServiceName)
{
	if(true == vServiceName.empty())
		return false;

	SC_HANDLE tSCHandle = ::OpenSCManager(NULL,  NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == tSCHandle)
	{
		LOG_ERROR("connect to service manager fail! errno: %d", GetLastError());
		return false;
	}

	SC_HANDLE tSvcHandle = ::OpenServiceA(tSCHandle, vServiceName.c_str(), SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if(NULL == tSvcHandle)
	{
		LOG_ERROR("open service fail! errno: %d", GetLastError());
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	SERVICE_STATUS tStatus;
	if(FALSE == ::QueryServiceStatus(tSvcHandle, &tStatus))
	{
		LOG_ERROR("query service status fail! errno: %d", GetLastError());
		::CloseServiceHandle(tSvcHandle);
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	if(SERVICE_RUNNING != tStatus.dwCurrentState)
	{
		LOG_ERROR("the state of service to stop is not SERVICE_RUNNING [%d]", tStatus.dwCurrentState);
		::CloseServiceHandle(tSvcHandle);
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	if(FALSE == ::ControlService(tSvcHandle, SERVICE_CONTROL_STOP, &tStatus))
	{
		LOG_ERROR("contorl service with stop fail! errno: %d", GetLastError());
		::CloseServiceHandle(tSvcHandle);
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	while(TRUE == ::QueryServiceStatus(tSvcHandle, &tStatus))
	{
		::Sleep(1000);
		if(SERVICE_STOPPED == tStatus.dwCurrentState)
		{
			break;
		}
	}

	::CloseServiceHandle(tSvcHandle);
	::CloseServiceHandle(tSCHandle);

	return true;
}
//---------------------------------------------------------------------------
bool MC_WinService::IsServiceInstalled(std::string vServiceName)
{
	if(true == vServiceName.empty())
		return false;

	SC_HANDLE tSCHandle = ::OpenSCManager(NULL,  NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == tSCHandle)
	{
		LOG_ERROR("connect to service manager fail! errno: %d", GetLastError());
		return false;
	}

	SC_HANDLE tSvcHandle = ::OpenServiceA(tSCHandle, vServiceName.c_str(), SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if(NULL == tSvcHandle)
	{
		LOG_ERROR("open service fail! errno: %d", GetLastError());
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	::CloseServiceHandle(tSvcHandle);
	::CloseServiceHandle(tSCHandle);
	return true;
}
//---------------------------------------------------------------------------
bool MC_WinService::IsServiceRunning(std::string vServiceName)
{
	if(true == vServiceName.empty())
		return false;

	SC_HANDLE tSCHandle = ::OpenSCManager(NULL,  NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == tSCHandle)
	{
		LOG_ERROR("connect to service manager fail! errno: %d", GetLastError());
		return false;
	}

	SC_HANDLE tSvcHandle = ::OpenServiceA(tSCHandle, vServiceName.c_str(), SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if(NULL == tSvcHandle)
	{
		LOG_ERROR("open service fail! errno: %d", GetLastError());
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	SERVICE_STATUS tStatus;
	if(FALSE == ::QueryServiceStatus(tSvcHandle, &tStatus))
	{
		LOG_ERROR("query service status fail! errno: %d", GetLastError());
		::CloseServiceHandle(tSvcHandle);
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	DWORD tState = tStatus.dwCurrentState;

	::CloseServiceHandle(tSvcHandle);
	::CloseServiceHandle(tSCHandle);

	if(SERVICE_RUNNING == tState)
		return true;
	return false;
}
//---------------------------------------------------------------------------
std::string MC_WinService::GetServicePath(std::string vServiceName)
{
	if(true == vServiceName.empty())
		return false;

	SC_HANDLE tSCHandle = OpenSCManager(NULL,  NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == tSCHandle)
	{
		LOG_ERROR("connect to service manager fail! errno: %d", GetLastError());
		return false;
	}

	SC_HANDLE tSvcHandle = OpenServiceA(tSCHandle, vServiceName.c_str(), SERVICE_ALL_ACCESS);
	if(NULL == tSvcHandle)
	{
		LOG_ERROR("OpenServiceA fail! errno: %d", GetLastError());
		CloseServiceHandle(tSCHandle);
		return false;
	}

	std::string tServicePath = "";
	LPQUERY_SERVICE_CONFIGA tSrvConfig = NULL;
	DWORD tBuffSize = 0;
	DWORD tBuffNeed = 0;
	BOOL tRet = QueryServiceConfigA(tSvcHandle, tSrvConfig, tBuffSize, &tBuffNeed);
	if(0 == tRet)
	{
		int tLastError = GetLastError();
		if(ERROR_INSUFFICIENT_BUFFER == tLastError)
		{
			DWORD tRealNeed = tBuffNeed;
			tSrvConfig = (LPQUERY_SERVICE_CONFIGA)new char[tRealNeed + 1];
			if(NULL == tSrvConfig)
				return "";
			tRet = QueryServiceConfigA(tSvcHandle, tSrvConfig, tRealNeed, &tBuffNeed);
			if(0 != tRet)
				tServicePath = tSrvConfig->lpBinaryPathName;
		}
	}
	else
	{
		tServicePath = tSrvConfig->lpBinaryPathName;
	}

	CloseServiceHandle(tSvcHandle);
	CloseServiceHandle(tSCHandle);

	return tServicePath;
}
//---------------------------------------------------------------------------
std::string MC_WinService::GetServiceNameByPid(DWORD vPid)
{
	if(0 > vPid)
		return "";

	LONG							tLongRet			= 0;
	BOOL							tBoolRet			= FALSE;
	SC_HANDLE						tSCHandle			= NULL;     // 服务数据库句柄
	char*							tBuffer				= NULL;     // 缓冲区指针
	DWORD							tBufferSize			= 0;        // 传入的缓冲长度
	DWORD 							tBufferSizeNeed		= 0;        // 需要的缓冲长度
	DWORD 							tNumberOfService	= 0;        // 返回的服务个数
	ENUM_SERVICE_STATUS_PROCESSA*	tServiceInfo		= NULL;		// 服务信息


	// 建立了一个到服务控制管理器的连接，并打开指定的数据库
	tSCHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CONNECT);
	if(NULL == tSCHandle)
	{
		LOG_ERROR("OpenSCManager fail! errno: %d", GetLastError());
		return "";
	}

	// 获取需要的缓冲区大小
	EnumServicesStatusEx(tSCHandle, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, 
		NULL, tBufferSize, &tBufferSizeNeed, &tNumberOfService, NULL, NULL);

	// 多设置存放1个服务信息的长度
	tBufferSize = tBufferSizeNeed + sizeof(ENUM_SERVICE_STATUS_PROCESS);
	tBuffer = (char *)malloc(tBufferSize);
	if(NULL == tBuffer)
	{
		LOG_ERROR("malloc fail");
		return "";
	}
	memset(tBuffer, 0, tBufferSize);

	// 获取服务信息
	tBoolRet = EnumServicesStatusExA(tSCHandle, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, 
		(LPBYTE)tBuffer, tBufferSize, &tBufferSizeNeed, &tNumberOfService, NULL, NULL);
	if(tBoolRet == FALSE)
	{
		LOG_ERROR("EnumServicesStatusEx fail! errno: %d", GetLastError());
		::CloseServiceHandle(tSCHandle);
		free(tBuffer);
		return "";
	}

	// 关闭打开的服务句柄
	tBoolRet = ::CloseServiceHandle(tSCHandle);
	if(tBoolRet == FALSE)
	{
		free(tBuffer);
		LOG_WARN("CloseServiceHandle fail! errno: %d", GetLastError());
	}

	//printf("Service Num:%d\n", tNumberOfService);


	//获取服务信息
	std::string tServiceName = "";
	tServiceInfo = (LPENUM_SERVICE_STATUS_PROCESSA)tBuffer;
	for(unsigned int i = 0; i < tNumberOfService; i++)
	{
		if(vPid == tServiceInfo[i].ServiceStatusProcess.dwProcessId)
		{
			tServiceName = tServiceInfo[i].lpServiceName;
			break;

			//打印服务信息
			printf("DisplayName \t\t : %s \n", tServiceInfo[i].lpDisplayName);												//显示名称
			printf("ServiceName \t\t : %s \n", tServiceInfo[i].lpServiceName);												//服务名称
			printf("ServiceType \t\t : %d \n", tServiceInfo[i].ServiceStatusProcess.dwServiceType);							//服务可执行文件的类型
			printf("CurrentState \t\t : %d \n", tServiceInfo[i].ServiceStatusProcess.dwCurrentState);						//服务状态
			printf("ControlsAccepted \t : %d \n", tServiceInfo[i].ServiceStatusProcess.dwControlsAccepted);					//服务接受什么样的控制通知
			printf("Win32ExitCode \t\t : %d \n", tServiceInfo[i].ServiceStatusProcess.dwWin32ExitCode);
			printf("ServiceSpecificExitCode  : %d \n", tServiceInfo[i].ServiceStatusProcess.dwServiceSpecificExitCode);
			printf("CheckPoint \t\t : %d \n", tServiceInfo[i].ServiceStatusProcess.dwCheckPoint);
			printf("WaitHint \t\t : %d \n", tServiceInfo[i].ServiceStatusProcess.dwWaitHint);
			printf("Process Id \t\t : %d \n", tServiceInfo[i].ServiceStatusProcess.dwProcessId);							//进程ID
			printf("ServiceFlags \t\t : %d \n", tServiceInfo[i].ServiceStatusProcess.dwServiceFlags);
		}
	}
	free(tBuffer);
	return tServiceName;
}
//---------------------------------------------------------------------------
/************************************************************************/
/*                                                                      */
/************************************************************************/
//---------------------------------------------------------------------------
