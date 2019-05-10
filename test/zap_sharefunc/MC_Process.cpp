#include "MC_Process.h"
#include "MC_Log.h"

#include <Tlhelp32.h>

#include <Iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")

#pragma comment(lib, "Ws2_32.lib")		//htons

#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

#include <shobjidl.h>
#include <ShlGuid.h>
#include <shlobj.h>
#include <tchar.h>
//---------------------------------------------------------------------------
std::string MC_Process::GetModuleName(HMODULE vHModule /* = 0 */)
{
	char tPath[MAX_PATH] = {0};
	if(0 == GetModuleFileNameA(vHModule, tPath, MAX_PATH))
		return "";

	char* tPos = strrchr(tPath, '\\');	
	if(0 == tPos)
		return "";

	tPos++;	
	char* tPos1 = strrchr(tPos, '.');	
	if(0 == tPos1)
		return tPos;

	*tPos1 = 0;
	return tPos;
}
//---------------------------------------------------------------------------
std::string MC_Process::GetMoudleDir(HMODULE vHModule /* = 0 */)
{
	char tPath[MAX_PATH];
	if(0 == GetModuleFileNameA(vHModule, tPath, MAX_PATH))
		return "";

	char* tPos = strrchr(tPath, '\\');
	if(0 == tPos)
		return "";

	*tPos = 0;
	return tPath;
}
//---------------------------------------------------------------------------
std::string MC_Process::RunFilePathName(char* vAppend, HMODULE vHModule /* = 0 */)
{
	char tPath[MAX_PATH];
	if(0 == GetModuleFileNameA(vHModule, tPath, MAX_PATH))
		return "";

	char* tPos = strrchr(tPath, '\\');
	if(0 == tPos)
		return "";

	*tPos = 0;
	char tResult[MAX_PATH];
	sprintf_s(tResult, sizeof(tResult), "%s\\%s", tPath, vAppend);

	return tResult;
}
//---------------------------------------------------------------------------
bool MC_Process::GetCmdlineArgs(std::vector<std::string>& vArgs)
{
	LPWSTR*		tArgList;
	int			tArgCount;
	tArgList = CommandLineToArgvW(GetCommandLineW(), &tArgCount);
	if(0 == tArgList)
		return false;

	vArgs.clear();
	for(int i=0; i<tArgCount; i++)
	{
		std::string tArgv = UnicodeToACP(tArgList[i]);
		vArgs.push_back(tArgv);
	}
	LocalFree(tArgList);

	return true;
}
//---------------------------------------------------------------------------
bool MC_Process::XLib_LoadAppAndWait(char* vCmdLine, int vShowMode, DWORD* vExitCode, DWORD vTimeOut /* = INFINITE */)
{
	if(0 == vCmdLine)	return false;
	if(0 == *vCmdLine)	return false;

	STARTUPINFOA stStartUpInfo;
	memset(&stStartUpInfo, 0, sizeof(STARTUPINFOA));
	stStartUpInfo.cb			= sizeof(STARTUPINFO);
	stStartUpInfo.dwFlags		= STARTF_USESHOWWINDOW;
	stStartUpInfo.wShowWindow	= vShowMode;

	PROCESS_INFORMATION stProcessInfo;
	if(false == CreateProcessA(0, vCmdLine, 0, 0, 0, NORMAL_PRIORITY_CLASS, 0, 0, &stStartUpInfo, &stProcessInfo))
	{
		//LOG_ERROR("Create Process fail! cmd: %s, errno: %d", vCmdLine, GetLastError());
		return false;
	}
	WaitForSingleObject(stProcessInfo.hProcess,vTimeOut);

	if(0 != vExitCode)
		GetExitCodeProcess(stProcessInfo.hProcess, vExitCode);

	CloseHandle(stProcessInfo.hProcess);
	CloseHandle(stProcessInfo.hThread);

	return true;
}
//---------------------------------------------------------------------------
bool MC_Process::RunCmdAndWait(char* vCmdLine, int vShowMode, DWORD* vExitCode, DWORD vTimeOut /* = INFINITE */)
{
	if(0 == vCmdLine)	return false;
	if(0 == *vCmdLine)	return false;

	char tCmdPath[MAX_PATH] = {0};
	if(0 == GetEnvironmentVariableA("COMSPEC", tCmdPath, MAX_PATH))
	{
		//LOG_ERROR("get the path of cmd.exe fail! errno: %d", GetLastError());
		return false;
	}

	char tCmdLine[MAX_PATH*2] = {0};
	sprintf_s(tCmdLine, "%s /c  %s", tCmdPath, vCmdLine);

	STARTUPINFOA stStartUpInfo;
	memset(&stStartUpInfo, 0, sizeof(STARTUPINFOA));
	stStartUpInfo.cb			= sizeof(STARTUPINFO);
	stStartUpInfo.dwFlags		= STARTF_USESHOWWINDOW;
	stStartUpInfo.wShowWindow	= vShowMode;

	PROCESS_INFORMATION stProcessInfo;
	if(false == CreateProcessA(0, tCmdLine, 0, 0, 0, NORMAL_PRIORITY_CLASS, 0, 0, &stStartUpInfo, &stProcessInfo))
	{
		//LOG_ERROR("Create Process fail! cmd: %s, errno: %d", tCmdLine, GetLastError());
		return false;
	}
	WaitForSingleObject(stProcessInfo.hProcess,vTimeOut);

	if(0 != vExitCode)
		GetExitCodeProcess(stProcessInfo.hProcess, vExitCode);

	CloseHandle(stProcessInfo.hProcess);
	CloseHandle(stProcessInfo.hThread);

	return true;
}
//---------------------------------------------------------------------------
bool MC_Process::DeleteMyselfExe()
{
	char	tExePath[MAX_PATH]	= {0};
	if(0 == GetModuleFileNameA(NULL, tExePath, MAX_PATH))
	{
		LOG_ERROR("GetModuleFileNameA fail! errno: %d", GetLastError());
		return false;
	}

	char	tCmdPath[MAX_PATH]	= {0};
	if(0 == GetEnvironmentVariableA("COMSPEC", tCmdPath, MAX_PATH))
	{
		LOG_ERROR("GetEnvironmentVariableA fail! errno: %d", GetLastError());
		return false;
	}

	if(0 == GetShortPathNameA(tExePath, tExePath, MAX_PATH))
		return false;

	char	tParam[MAX_PATH*2]	= {0};
	sprintf_s(tParam, "%s /c del %s", tCmdPath, tExePath);

	// create a command process, set its priority, then start it.
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb          = sizeof(si);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	ZeroMemory( &pi, sizeof(pi) );

	if(false == CreateProcessA(NULL, tParam, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi))
	{
		LOG_ERROR("CreateProcessA fail! errno: %d", GetLastError());
		return false;
	}

	//将当前进程设为高优先级，这样程序将会先退出，再执行删除命令
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);	// heigthen priority of the current process
	SetFileAttributesA(tExePath, FILE_ATTRIBUTE_NORMAL);		// set file attribute to normal
	SetPriorityClass(pi.hProcess, IDLE_PRIORITY_CLASS);			// depress priority of command process, then start it
	ResumeThread(pi.hThread);
	
	return true;
}
//---------------------------------------------------------------------------
bool MC_Process::DeleteMyselfDir()
{
	//生成一个临时的bat，不用bat的方式会遗留一个空目录
	std::string tExePath	= GetMoudleDir();
	std::string tBatName	= GetMoudleDir() + "\\temp.bat";

	std::string tConten		= "";

#ifdef _WINDOWS_
	std::string tNewLine	= "\r\n"; 
#else
	std::string tNewLine	= "\n";
#endif

	tConten += "cd /" + tNewLine;						//解除源文件所在目录的占用
	tConten += "rd /q /s " + tExePath + tNewLine;		//删除源文件目录
	tConten += "del %0";								//删除bat文件

	if(false == SetFileContent(tBatName.c_str(), tConten.c_str()))
	{
		LOG_ERROR("create temp fail! errno: %d", GetLastError());
		return false;
	}

	// create a command process, set its priority, then start it.
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb          = sizeof(si);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	ZeroMemory( &pi, sizeof(pi) );

	if(false == CreateProcessA(NULL, (char*)tBatName.c_str(), NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi))
	{
		LOG_ERROR("CreateProcessA fail! errno: %d", GetLastError());
		return false;
	}

	// heigthen priority of the current process
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);   

	// set file attribute to normal
	SetFileAttributesA(tExePath.c_str(), FILE_ATTRIBUTE_NORMAL);

	// depress priority of command process, then start it
	SetPriorityClass(pi.hProcess, IDLE_PRIORITY_CLASS);
	ResumeThread(pi.hThread);
	return true;
}
//---------------------------------------------------------------------------
bool MC_Process::KillProcessByProName(const char* vProcessName)
{
	DWORD tProcessId = GetPidByProName(vProcessName);
	while(0 != tProcessId)
	{
		HANDLE tProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, tProcessId);
		if(NULL == tProcessHandle)
		{
			return false;
		}

		if(FALSE == TerminateProcess(tProcessHandle, 0))
		{
			return false;
		}

		Sleep(100);
		tProcessId = GetPidByProName(vProcessName);
	}

	return true;
}
//---------------------------------------------------------------------------
bool MC_Process::KillProcessByPid(DWORD vPid)
{
	if(0 > vPid)
		return false;

	HANDLE tProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, vPid);
	if(NULL == tProcessHandle)
	{
		return false;
	}

	if(FALSE == TerminateProcess(tProcessHandle, 0))
	{
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool MC_Process::GetAllPidByProName(const char* vProName, std::vector<DWORD>& vPids)
{
	if(0 == vProName)		return false;
	if(0 == *vProName)		return false;

	HANDLE tSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);	//获取快照
	if(INVALID_HANDLE_VALUE == tSnapshot)
		return false;

	PROCESSENTRY32 tPe = {sizeof(tPe)};
	BOOL tRet = Process32First(tSnapshot, &tPe);
	if(FALSE == tRet)
	{
		LOG_ERROR("Process32First fail! errno: %d", GetLastError());
		return false;
	}

	vPids.clear();
	do 
	{
		if(vProName != UnicodeToACP(tPe.szExeFile))
			continue;

		vPids.push_back(tPe.th32ProcessID);

	} while (Process32Next(tSnapshot, &tPe));

	CloseHandle(tSnapshot);
	return true;
}
//---------------------------------------------------------------------------
DWORD MC_Process::GetPidByProName(const char* vProName)
{
	if(0 == vProName)		return 0;
	if(0 == *vProName)		return 0;

	HANDLE tSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);	//获取快照
	if(INVALID_HANDLE_VALUE == tSnapshot)
		return false;

	PROCESSENTRY32 tPe = {sizeof(tPe)};
	BOOL tRet = Process32First(tSnapshot, &tPe);
	if(FALSE == tRet)
	{
		LOG_ERROR("Process32First fail! errno: %d", GetLastError());
		return false;
	}

	DWORD	tPId = 0;
	do 
	{
		if(vProName != UnicodeToACP(tPe.szExeFile))
			continue;

		tPId = tPe.th32ProcessID;
		break;

	} while (Process32Next(tSnapshot, &tPe));

	CloseHandle(tSnapshot);
	return tPId;
}
//---------------------------------------------------------------------------
DWORD MC_Process::GetPidByTcpPort(u_short vLocalPort)
{
	if(0 > vLocalPort || 65535 < vLocalPort)	
		return 0;

	PMIB_TCPTABLE_OWNER_PID tTcpTableInfo(NULL);
	DWORD tSize(0);
	if(ERROR_INSUFFICIENT_BUFFER == GetExtendedTcpTable(tTcpTableInfo, &tSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0))
	{
		tTcpTableInfo = (MIB_TCPTABLE_OWNER_PID *)new char[tSize];//重新分配缓冲区
	}

	if(NO_ERROR != GetExtendedTcpTable(tTcpTableInfo, &tSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) )
	{
		delete tTcpTableInfo;
		return false;
	}

	DWORD	tPId = 0;
	int		tNum = (int) tTcpTableInfo->dwNumEntries; //TCP连接的数目
	for(int i=0; i<tNum; i++)
	{
		if(vLocalPort == htons(tTcpTableInfo->table[i].dwLocalPort))	////本地端口
		{
			tPId =  tTcpTableInfo->table[i].dwOwningPid;
			break;
			//printf("本地地址：%s:%d  远程地址：%s:%d  状态：%d  进程ID:%d\n", 
			//	inet_ntoa(*(in_addr*)& tTcpTableInfo->table[i].dwLocalAddr), //本地IP 地址
			//	htons(tTcpTableInfo->table[i].dwLocalPort), //本地端口
			//	inet_ntoa(*(in_addr*)& tTcpTableInfo->table[i].dwRemoteAddr), //远程IP地址
			//	htons(tTcpTableInfo->table[i].dwRemotePort), //远程端口
			//	tTcpTableInfo->table[i].dwState, //状态
			//	tTcpTableInfo->table[i].dwOwningPid); //所属进程PID
		}

	}
	delete tTcpTableInfo;
	return tPId;
}
//---------------------------------------------------------------------------
std::string MC_Process::GetProUserByPid(DWORD vPid)
{
	HANDLE tProcHandle = OpenProcess(PROCESS_QUERY_INFORMATION, 0, vPid);
	if(0 == tProcHandle)
	{
		return "";
	}

	HANDLE tTokenHandle;
	if(0 == OpenProcessToken(tProcHandle, TOKEN_ALL_ACCESS, &tTokenHandle))
	{
		return "";
	}

	PTOKEN_USER tTokenUser = 0;
	DWORD		tInfoSize;
	GetTokenInformation(tTokenHandle, TokenUser, tTokenUser, 0, &tInfoSize);
	if(GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		return "";
	}

	tTokenUser = (PTOKEN_USER)malloc(tInfoSize);
	if(0 == GetTokenInformation(tTokenHandle,TokenUser,tTokenUser, tInfoSize, &tInfoSize))
	{
		return "";
	}

	CloseHandle(tTokenHandle);

	char			tUserName[256]	={0};
	char			tDomain[256]	={0};
	DWORD			tDomainSize		=256;
	DWORD			tNameSize		=256;
	SID_NAME_USE	tSNU;
	if(0 == LookupAccountSidA(0, tTokenUser->User.Sid, tUserName, &tNameSize, tDomain, &tDomainSize, &tSNU))
	{
		return "";
	}

	return tUserName;
}
//---------------------------------------------------------------------------
std::string MC_Process::GetProPathByPid(DWORD vPid)
{
	HANDLE	tProHandle	= OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, vPid);  
	if (NULL == tProHandle)
	{  
		LOG_ERROR("OpenProcess fail! errno: %d", GetLastError());
		return "";  
	}  

	static DWORD tVer=0;	//静态 调用一次即可,获取系统版本  
	if (tVer==0)  
	{  
		OSVERSIONINFO tOs = {sizeof(OSVERSIONINFO)};  
		GetVersionEx(&tOs);  
		tVer = tOs.dwMajorVersion;  
	}  


	TCHAR tProcessName[MAX_PATH] = {0};
	if (tVer<6)//版本小于 Windows Vista  
	{  
		if (0 == GetModuleFileNameEx(tProHandle, (HMODULE)0, tProcessName, MAX_PATH))   
		{  
			// GetModuleFileNameEx could fail when the address space  
			// is not completely initialized. This occurs when the job  
			// notification happens.  
			// Hopefully, GetProcessImageFileNameW still works even though  
			// the obtained path is more complication to decipher  
			//    /Device/HarddiskVolume1/Windows/System32/notepad.exe  
			if (0 == GetProcessImageFileName(tProHandle, tProcessName, MAX_PATH))   
			{  
				LOG_ERROR("GetProcessImageFileNameA fail");
				return "";
			}  
		}  
	}  
	else  
	{  
		// but it is easier to call this function instead that works fine  
		// in all situations.  

		typedef BOOL (WINAPI* func_QueryFullProcessImageName)(HANDLE tProHandle, DWORD dwFlags, LPTSTR lpExeName, PDWORD lptSize);  

		func_QueryFullProcessImageName tQFPIN=NULL;  
		tQFPIN = (func_QueryFullProcessImageName) GetProcAddress(  
			GetModuleHandle(TEXT("Kernel32.dll")),  
#ifdef UNICODE  
			"QueryFullProcessImageNameW"  
#else  
			"QueryFullProcessImageNameA"  
#endif  
			);  
		DWORD tSize = (DWORD) MAX_PATH;  
		if (FALSE == tQFPIN(tProHandle, 0, tProcessName, &tSize))//call QueryFullProcessImageName  
		{                    //最低版本 vista  
			LOG_ERROR("tQFPIN fail");
			return "";
		}  
	}  

	// Don't forget to cltOse the process handle  
	CloseHandle(tProHandle); 

#ifdef UNICODE  
	return UnicodeToACP(tProcessName);
#else  
	return tProcessName;
#endif   
}
//---------------------------------------------------------------------------
BOOL MC_Process::CreateFileShortcut(const char* vSrcFile, const char* vLinkFile, const char* vLinkDir, WORD vHotKey, const char* vDescription, int vShowCmd/* =SW_SHOWNORMAL */)
{
	if(0 == vLinkDir)	return FALSE;
	if(0 == *vLinkDir)	return FALSE;

	CoInitialize(NULL);

	HRESULT			tHr;
	IShellLink*		tShellLink;	//IShellLink对象指针
	IPersistFile*	tPf;		//IPersisFil对象指针

	//创建IShellLink对象
	tHr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&tShellLink);
	if (FAILED(tHr))
	{
		LOG_ERROR("CoCreateInstance fail! errno: %#0x", tHr);
		CoUninitialize();
		return FALSE;
	}

	/*
	 * tPath可以用tpgmptr代替，但是main函数的版本不确定，所以还是选择GetModuleFileNameA
	 * main()	对应 tPath
	 * wmain()	对应 _wpgmptr,此时tPath = NULL
	 */
	char tPath[MAX_PATH];
	if(0 == GetModuleFileNameA(NULL, tPath, MAX_PATH))
		return false;

	//源
	if(NULL == vSrcFile)
		tShellLink->SetPath(ACPToUnicode(tPath).c_str());
	else
		tShellLink->SetPath(ACPToUnicode(vSrcFile).c_str());


	//快捷键
	if (vHotKey != 0)
		tShellLink->SetHotkey(vHotKey);

	//备注
	if (vDescription != NULL)
		tShellLink->SetDescription(ACPToUnicode(vDescription).c_str());

	//显示方式
	tShellLink->SetShowCmd(vShowCmd);


	//指定目录
	char tBuffer[MAX_PATH];
	if (vLinkFile != NULL) //指定了快捷方式的名称
		sprintf(tBuffer, "%s\\%s", vLinkDir, vLinkFile);
	else  
	{
		//没有指定名称，就从取指定文件的文件名作为快捷方式名称。
		char* tStr = "";
		if (vSrcFile != NULL)
			tStr = strrchr((char*)vSrcFile, '\\');
		else
			tStr = strrchr(tPath, '\\');

		if (tStr == NULL)
		{    
			LOG_ERROR("tStr == NULL");
			tShellLink->Release();
			CoUninitialize();
			return FALSE;
		}
		//注意后缀名要从.exe改为.lnk
		sprintf(tBuffer, "%s\\%s", vLinkDir, tStr);
		int tLen = strlen(tBuffer);
		tBuffer[tLen - 3] = 'l';
		tBuffer[tLen - 2] = 'n';
		tBuffer[tLen - 1] = 'k';
	}


	//从IShellLink对象中获取IPersistFile接口
	tHr = tShellLink->QueryInterface(IID_IPersistFile, (void**)&tPf);
	if (FAILED(tHr))
	{
		LOG_ERROR("QueryInterface fail! errno: %#0x", tHr);
		tShellLink->Release();
		CoUninitialize();
		return FALSE;
	}

	//保存快捷方式到指定目录下
	//WCHAR  tWideStr[MAX_PATH];  //定义Unicode字符串
	//MultiByteToWideChar(CP_ACP, 0, tBuffer, -1, tWideStr, MAX_PATH);
	tHr = tPf->Save(ACPToUnicode(tBuffer).c_str(), TRUE);

	tPf->Release();
	tShellLink->Release();
	CoUninitialize();

	return SUCCEEDED(tHr);
}
//---------------------------------------------------------------------------
bool MC_Process::CreatLinkToDesktop()
{
	HRESULT tHr = CoInitialize(NULL);
	if(FAILED(tHr))
	{
		LOG_ERROR("CoInitialize fail! errno: %#0x", tHr);
		return false;
	}

	IShellLink *tShellLink;
	tHr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&tShellLink);
	if(FAILED(tHr))
	{
		LOG_ERROR("CoCreateInstance fail! errno: %#0x", tHr);
		CoUninitialize();
		return false;
	}

	/*
	 * tPath可以用tpgmptr代替，但是main函数的版本不确定，所以还是选择GetModuleFileNameA
	 * main()	对应 _pgmptr
	 * wmain()	对应 _wpgmptr,此时_pgmptr = NULL
	 */
	char tPath[MAX_PATH];
	if(0 == GetModuleFileNameA(NULL, tPath, MAX_PATH))
		return false;

	IPersistFile* tIPF;
	//这里是我们要创建快捷方式的原始文件地址
	tShellLink->SetPath(ACPToUnicode(tPath).c_str());	//为什么不能直接使用_wpgmptr？？？
	tHr = tShellLink->QueryInterface(IID_IPersistFile, (void**)&tIPF);
	if(FAILED(tHr))
	{
		LOG_ERROR("QueryInterface fail! errno: %#0x", tHr);
		tShellLink->Release();
		CoUninitialize();
		return false;
	}

	//这里是我们要创建快捷方式的目标地址
	char tStartPath[MAX_PATH] = {0};
	SHGetSpecialFolderPathA(NULL, tStartPath, CSIDL_DESKTOP, 0);		//桌面路径
	sprintf(tStartPath, "%s\\%s", tStartPath, strrchr(tPath, '\\'));

	//注意后缀名要从.exe改为.lnk
	int tLen = strlen(tStartPath);
	tStartPath[tLen - 3] = 'l';
	tStartPath[tLen - 2] = 'n';
	tStartPath[tLen - 1] = 'k';

	//USES_CONVERSION;
	//LPCOLESTR lpOleStr = A2COLE(tStartPath);	//多字节
	tIPF->Save(ACPToUnicode(tStartPath).c_str(), FALSE);	//Unicode

	tIPF->Release();
	tShellLink->Release();
	CoUninitialize();

	return true;
}
//---------------------------------------------------------------------------
std::string MC_Process::GetDesktopPath()
{
	////第一种方法
	//HRESULT			tHr				= S_FALSE;
	//LPITEMIDLIST	tPidl			= NULL;
	//char			tPath[MAX_PATH] = {0};

	//tHr = SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &tPidl);
	//if(S_OK != tHr)
	//{
	//	LOG_ERROR("SHGetSpecialFolderLocation fail! errno: %d", tHr);
	//	return "";
	//}

	//BOOL tFlag =  SHGetPathFromIDListA(tPidl, tPath);
	//CoTaskMemFree(tPidl);
	//if(FALSE == tFlag)
	//	return "";

	//return tPath;

	//第二种方法
	std::string	tPath			= "";
	wchar_t*	tLocalAppData	= NULL;
	if (S_OK == SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &tLocalAppData))
	{
		tPath = UnicodeToACP(tLocalAppData);
		CoTaskMemFree(reinterpret_cast<void*>(tLocalAppData));
	}

	return tPath;
}
//---------------------------------------------------------------------------
std::string MC_Process::GetIEQuickLaunchPath()
{
	HRESULT			tHr				= S_FALSE;
	LPITEMIDLIST	tPidl			= NULL;
	char			tPath[MAX_PATH] = {0};

	tHr = SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &tPidl);
	if(S_OK != tHr)
	{
		LOG_ERROR("SHGetSpecialFolderLocation fail! errno: %d", tHr);
		return "";
	}

	BOOL tFlag =  SHGetPathFromIDListA(tPidl, tPath);
	CoTaskMemFree(tPidl);
	if(FALSE == tFlag)
		return "";

	strcat(tPath, "\\Microsoft\\Internet Explorer\\Quick Launch");
	return tPath;
}
//---------------------------------------------------------------------------
std::string MC_Process::GetProgramsPath()
{
	HRESULT			tHr				= S_FALSE;
	LPITEMIDLIST	tPidl			= NULL;
	char			tPath[MAX_PATH] = {0};

	tHr = SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &tPidl);
	if(S_OK != tHr)
	{
		LOG_ERROR("SHGetSpecialFolderLocation fail! errno: %d", tHr);
		return "";
	}

	BOOL tFlag =  SHGetPathFromIDListA(tPidl, tPath);
	CoTaskMemFree(tPidl);
	if(FALSE == tFlag)
		return "";

	return tPath;
}
//---------------------------------------------------------------------------
/************************************************************************/
/*                                                                      */
/************************************************************************/
//---------------------------------------------------------------------------
std::string MC_Process::UnicodeToACP(const wchar_t* vBuf)
{
	int tLen = WideCharToMultiByte(CP_ACP, 0, vBuf, -1, NULL, 0, NULL, NULL);
	std::vector<char> tUft8(tLen);
	WideCharToMultiByte(CP_ACP, 0, vBuf, -1, &tUft8[0], tLen, NULL, NULL);

	return std::string(&tUft8[0]);
}
//---------------------------------------------------------------------------
std::wstring MC_Process::ACPToUnicode(const char* vBuf)
{
	int tLen = MultiByteToWideChar(CP_ACP, 0, vBuf, -1, NULL, 0);
	std::vector<wchar_t> tUnicode(tLen);
	MultiByteToWideChar(CP_ACP, 0, vBuf, -1, &tUnicode[0], tLen);

	return std::wstring(&tUnicode[0]);
}
//---------------------------------------------------------------------------
bool MC_Process::SetFileContent(const char* vFileName, const char* vContent)
{
	if(0 == vFileName)	return false;
	if(0 == *vFileName)	return false;
	if(0 == vContent)	return false;
	if(0 == *vContent)	return false;

	FILE* tFp = NULL;
	if(0 != fopen_s(&tFp, vFileName, "wb+"))
	{
		return false;
	}

	int	tContenSize = strlen(vContent);
	int tPos		= 0;
	int tCount		= 0;
	while(0 < tContenSize)
	{
		if(1024 >= tContenSize)
			tCount = tContenSize;
		else
			tCount = 1024;

		size_t tWirteNum = fwrite(vContent+tPos, sizeof(char), tCount, tFp);
		if(0 > tWirteNum)
		{
			fclose(tFp);
			return false;
		}

		tPos		+= tWirteNum;
		tContenSize -= tWirteNum;
	}

	fflush(tFp);
	fclose(tFp);
	return true;
}
//---------------------------------------------------------------------------