#ifndef MC_Process_H
#define MC_Process_H
//---------------------------------------------------------------------------
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//---------------------------------------------------------------------------
#include <WinSock2.h>	//防止冲突，这个必须在前面
#include <Windows.h>
#include <vector>
#include <string>
using namespace std;
//---------------------------------------------------------------------------
class MC_Process
{
public:
	static	std::string		GetModuleName				(HMODULE vHModule = 0);					//获取指定模块的名称，  vHModule = 0 表示获取当前EXE的名称
	static	std::string		GetMoudleDir				(HMODULE vHModule = 0);					//获取指定模块所在目录，vHModule = 0 表示获取当前EXE的所在目录
	static	std::string 	RunFilePathName				(char* vAppend, HMODULE vHModule = 0);	//[指定模块运行路径]\\[vAppend]

	static	bool			GetCmdlineArgs				(std::vector<std::string>& vArgs);	//获取程序运行参数

	static	bool            XLib_LoadAppAndWait     	(char* vCmdLine, int vShowMode, DWORD* vExitCode, DWORD vTimeOut = INFINITE);	//不可以直接运行批处理命令，必须先生成bat文件，在执行bat
	static	bool            RunCmdAndWait     			(char* vCmdLine, int vShowMode, DWORD* vExitCode, DWORD vTimeOut = INFINITE);	//可以直接运行批处理命令

	/**
	 * @Method		: DeleteMyselfExe
	 * @brief		: 程序退出后将自身删除
	 * @param[in]	: 无
	 * @return		: true=成功，false=失败
	 * @other		: 调用此接口后，不能再调用getchar()等会使进程暂停的函数，否则失败（errno=5）。所以最好的调用时机是在main函数return之前。
	 */
	static	bool			DeleteMyselfExe				(void);	//删除自身EXE
	static	bool			DeleteMyselfDir				(void);	//删除自身所在目录

	static	bool            KillProcessByProName       	(const char* vProcessName);
	static	bool			KillProcessByPid			(DWORD vPid);

	static	bool			GetAllPidByProName			(const char* vProName, std::vector<DWORD>& vPids);
	static	DWORD			GetPidByProName				(const char* vProName);		//一个进程名可能对应多个进程，这里取第一个
	static	DWORD			GetPidByTcpPort				(u_short vLocalPort);		//一个端口只能对应一个进程

	static	std::string		GetProUserByPid				(DWORD vPid);				//获取进程所属用户
	static	std::string		GetProPathByPid				(DWORD vPid);				//获取进程运行路径

	/**
	 * @Method		: CreateFileShortcut
	 * @brief		: 生成指定程序的快捷方式
	 * @param[in]	: vSrcFile:		目标文件路径，为NULL表示当前进程的EXE文件，必须是一个完整路径+文件名	  
	 * @param[in]	: vLinkFile:	快捷方式文件名（x.lnk），NULL 表示使用当前文件名
	 * @param[in]	: vLinkDir:		快捷方式生成位置，不能为NULL
	 * @param[in]	: vHotKey:		为0表示不设置快捷键
	 * @param[in]	: vDescription: 备注
	 * @param[in]	: vShowCmd:		运行方式，默认为常规窗口
	 * @return		: 
	 */
	static	BOOL			CreateFileShortcut			(const char* vSrcFile, const char* vLinkFile, const char* vLinkDir, WORD vHotKey, const char* vDescription, int vShowCmd=SW_SHOWNORMAL);
	static	bool			CreatLinkToDesktop			();

	static	std::string 	GetDesktopPath				();		//得到当前桌面路径
	static	std::string 	GetIEQuickLaunchPath		();		//得到快速启动栏的路径
	static	std::string 	GetProgramsPath				();		//得到 开始->程序组 的路径

private:
	static	std::string		UnicodeToACP				(const wchar_t* vBuf);
	static	std::wstring	ACPToUnicode				(const char* vBuf);
	static	bool			SetFileContent				(const char* vFileName, const char* vContent);

};
//---------------------------------------------------------------------------
#endif 
//---------------------------------------------------------------------------
/*
bool		tRet		= false;
DWORD		tExitCode	= 0;

std::string tMySqlTool	= MC_Process::RunFilePathName("..\\3rdData\\cmdline_test\\mysqldump.exe");
std::string tBackupFile	= MC_Process::RunFilePathName("..\\3rdData\\cmdline_test\\backup.sql");
std::string tTestModule = MC_Process::RunFilePathName("..\\3rdData\\cmdline_test\\test1.exe");

char tCmd[1024] = {0};
sprintf_s(tCmd, sizeof(tCmd), "%s -h127.0.0.1 -uroot -pgosuncn_goldmsg -P3306 --default-character-set=utf8 "
"--database --opt --extended-insert=false --triggers -R --hex-blob -x gmvcsws > %s", tMySqlTool.c_str(), tBackupFile.c_str());

//错误，不能生成备份文件
tRet = MC_Process::XLib_LoadAppAndWait(tCmd, SW_SHOW, &tExitCode);
if(false == tRet || 1 == tExitCode)
	LOG_ERROR("XLib_LoadAppAndWait fail");

//正确，可以生成备份文件
tRet = MC_Process::RunCmdAndWait(tCmd, SW_SHOW, &tExitCode);
if(false == tRet || 1 == tExitCode)
	LOG_ERROR("RunCmdAndWait fail");


tRet = MC_Process::XLib_LoadAppAndWait((char*)tTestModule.c_str(), SW_SHOW, &tExitCode);
if(false == tRet || 1 == tExitCode)
	LOG_ERROR("XLib_LoadAppAndWait fail");

tRet = MC_Process::RunCmdAndWait((char*)tTestModule.c_str(), SW_SHOW, &tExitCode);
if(false == tRet || 1 == tExitCode)
	LOG_ERROR("RunCmdAndWait fail");
*/

/*
std::string tPath = MC_Process::RunFilePathName("..\\Debug\\test1.exe");
MC_Process::CreateFileShortcut(tPath.c_str(), "xx.lnk", MC_Process::GetDesktopPath().c_str(), 0, "");
*/