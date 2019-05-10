//---------------------------------------------------------------------------
#ifndef MF_CommonFuncH
#define MF_CommonFuncH
//---------------------------------------------------------------------------
//#define QTDIR	D:\Qt\4.8.0
//---------------------------------------------------------------------------
#ifndef	_CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#define	SafeCloseHHandle(_X_)	{if(0 != _X_) {CloseHandle(_X_); _X_= 0;}}
//---------------------------------------------------------------------------
#include <Winsock2.h>	//避免冲突，Winsock2.h要在Windows.h前面
#pragma comment(lib, "Ws2_32.lib")

#include <tchar.h>

#include <Windows.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>

#include <WinCrypt.h>
#pragma comment(lib, "Crypt32.lib")

using namespace std;
//---------------------------------------------------------------------------
#define	KKS				std::string;
#define KKString(_X_)	(char*)(_X_.c_str())
//---------------------------------------------------------------------------
namespace MF_CommonFunc
{
	/*****************时间相关******************/
	/*
	* Method		_GetCurrentTime
	* @brief		获取当前系统时间，精确到秒
	* @param[in]	vTimePos   时分秒间隔符，默认":"
	* @return		当前时分秒
	*/
	std::string		_GetCurrentTime				(std::string vTimePos = ":");	//当前时间，默认格式：00:00:00
	std::string 	GetCurrentDate				(std::string vDatePos = "-");	//当前日期，默认格式：0000-00-00
	std::string 	GetCurrentDateTime			(std::string vDatePos = "-", std::string vDtPos = " ", std::string vTimePos = ":"); //当前日期+时间，默认格式：0000-00-00 00:00:00
	std::string 	GetCurrentTimeWithWeek		();

	__int64			GetTimestamp				(std::string vTimes="");							//获取时间戳
	std::string		FmtTimestamp				(__int64 vTimestamp);		//时间戳转格式化时间

	std::string		FmtDateTime					(const std::string& vDateTime);

	std::string		GetNowTime					(void);
	std::string		GetLastWeek					(void);


	/******************字符串操作******************/	
	std::string		STRReplace					(const std::string& vString, const std::string& vSubStrFrom, const std::string& vSubStrTo);
	std::string		ReplaceBackslashBySlash		(std::string vStr);	//用斜线(/)代替反斜杠(\\)
	std::string		Trim						(std::string& vStr);	//去掉头尾的空格

	void			SeparateString				(char* vStr, const char* vSeps, vector<std::string>& vRet);	//分离字符串
	void			SeparateWstring				(const std::wstring vWstr,const std::wstring WSeps,vector<std::wstring>& vRet);

	bool			SeparateFile				(const char* vPath, char* vDir, char* vFile);

	std::string		STRCombine					(char* vFormat,...);

	//C 字符串
	void			ChWConvertEndian			(void * ptr, size_t size);
	void			ChSeparateFile				(const char* vPath, char* vFile, char* vDir);	//将path的文件名和路径切割
	char*			ChStrReplace				(char* vStr, char* vOldStr, char* vNewStr);		//将字符串中的某个字符串换为另一个字符串
	/******************文件操作相关******************/
	std::string		GetPathSection				(std::string vPath);	//获取第二个路径间隔符前的字符串 E:\\123\\345\\1.txt -> E:\\123

	std::string		GetFileNameWithoutSuffix	(std::string vFileName);
	std::string		GetFileSuffix				(const std::string& vFileName);	//后缀名(不带点)	

	__int64			GetFileSize					(const std::string vFilePath);	//文件大小
	__int64			GetFolderSize				(const std::string vDir);		//目录大小

	std::string		GetFileConten				(const char* vFilePath);	//获取文件内容
	int				GetFileSizeFromFilePath		(const char* vFilePath);	
	int				GetFileSizeFromHandle		(FILE* vFileHandle);
	bool			LoadFile					(const char* vFilePath, char* vBuffer, int vBufLen);

	std::string		SearchFilePathByFileName	(const std::string vSearchDir, const std::string vFileName, const std::string vSuffixName);
	bool			TraversalFilesInDir			(const char* vDir, std::vector<std::string>&vFileList, const char* vSuffixName=NULL, bool vIsRecursive=true);	//遍历目录
	bool			IsValidFile					(const char* vFilePath);	//提取特征字段，判断是否为需要的文件

	bool			SHFileDelete				(const std::string& vPath, bool vDestory=true);	//删除文件或目录
	bool			WriteFileConten				(const char* vFileName, const char* vContent);

	//目录
	bool			MakeEmptyDir				(const char* vPath);	//创建空目录
	bool			MakeDir						(const char* vPath);	//创建目录
	bool			DeleteDir					(const char* vPath);	//删除目录
	bool			DirExist					(const char* vPath);	//目录是否存在
	bool			IsEmptyDir					(const char* vPath);	//目录是否为空

	//文件
	bool			IsFileExist					(std::string vFileName);
	bool			MyDeleteFile				(const char* vFileName);
	bool			GetFileInfo					(char* vFilePath, std::string& vOriginalName);

	std::string		GetSignNameOfFile			(char* vFilePath);	//获取文件的数字签名
	PCCERT_CONTEXT	GetFileCertificate			(char* vFilePath);	//获取证书信息



	/******************程序/进程相关******************/
	std::string 	RunFilePathName				(char* vAppend, HMODULE vHModule = 0);	//当前模块运行路径 + vAppend
	std::string		GetModuleName				();	//模块名test
	std::string		GetCurrentDirName			(HMODULE vHModule = 0);

	bool			GetCmdlineArgs				(std::vector<std::string>* vArgs);	//获取程序运行参数

	bool            XLib_LoadAppAndWait     	(char* vCmdLine, int vShowMode, DWORD vTimeOut, DWORD* vExitCode);
	bool            RunCmdAndWait     			(char* vCmdLine, int vShowMode, DWORD vTimeOut, DWORD* vExitCode);

	bool			DeleteMyselfExe				(void);	//删除自身EXE
	bool			DeleteMyselfDir				(void);	//删除自身所在目录

	DWORD           GetProcessIdByName      	(const char* vProcessName); //获取进程ID，任务管理器显示的映像名称
	DWORD			GetProcessIdByTcpPort		(u_short vLocalPort);

	bool            KillProcessByName       	(const char* vProcessName);
	std::string		GetProcUserNameByPid		(int vProcId);	//获取进程用户kinkoo
	std::string		GetProcessPathByPid			(int vPid);


	/******************注册表操作******************/
	bool			GetRegValue					(const char* vPath, const char* vValue, DWORD *vRegType, char **vData, DWORD *vLen);
	HKEY			RegKeyRoot					(const std::string& vPath);
	std::string		GetRegValueS				(const char* vPath, const char* vValue);	//获取注册表 键值
	void			FreeRegBuff					(const char* vData);


	/******************磁盘相关******************/
	bool			check_disk_wirteable		(int vIndex);	//检测磁盘是否可写

	int				GetDiskInfo					(void); 
	bool			GetDiskSpace				(std::string vName, __int64& vAllKB, __int64& vUsedKB, __int64& vFreeKB );

	//驱动器挂载点操作
	bool			GetMountPointsList			(std::vector<std::string>& vMountPointList, std::string vWildcard="");	//获取系统挂载点 vWildcard表示匹配的关键字
	bool			ListMountPointOfVolume		(const char* vVolumeName, std::vector<std::string>& vMountPointList, std::string vWildcard="");	//挂载点查询

	bool			DeleteMountPoint			(const char* vMountPoint);
	bool			SetMountPoint				(const char* vOldMountPoint, const char* vNewMountPoint);

	//test func
	bool			GetDriverString				(const char* vVolumeName);	//盘符


	/******************操作系统相关******************/
	bool			IsWin7OS					();	//不准确
	bool			CheckSystemVersion			();	//准确	
	std::string		GetSystemName				(); 

	BOOL			Is64System					(void);

	bool			pc_power_off				(bool vRestart);

	bool			GetScreenResolution			(int& vLateral, int& vVertical);

	/******************windows服务******************/
	bool            IsServiceInstalled  		(std::string vServiceName);
	bool            IsServiceRunning    		(std::string vServiceName);

	bool            InstallService      		(std::string vServiceName, std::string vDisplayName, std::string vServicePath);
	bool            UninstallService    		(std::string vServiceName);

	bool            MyStartService      		(std::string vServiceName);
	bool            MyStopService       		(std::string vServiceName);

	std::string		GetServicePath				(std::string vServiceName);
	std::string		GetServiceNameByProcessId	(DWORD vProcessId);


	/******************XML******************/
	bool			UpdateXmlNode				(std::string vConfPath, std::string vName, std::string vValue);
	bool			CreateXml					(std::string vConfPath);
	bool			LoadConfig					(std::string vConfPath);

	bool			CreateXmlTest				();

	/******************快捷方式******************/
	/*生成快捷方式
	*vSrcFile:		目标文件路径，为NULL表示当前进程的EXE文件，必须是一个完整路径+文件名	 
	*vLinkFile:	快捷方式文件名，NULL 表示使用当前文件名
	*vLinkDir:		快捷方式生成位置，不能为NULL
	*vHotKey:		为0表示不设置快捷键
	*vDescription: 备注
	*vShowCmd:		运行方式，默认为常规窗口
	*/
	BOOL			CreateFileShortcut			(const char* vSrcFile, const char* vLinkFile, const char* vLinkDir, WORD vHotKey, const char* vDescription, int vShowCmd=SW_SHOWNORMAL);
	BOOL			CreateFileShortcutA			(const char* vSrcFile, const char* vLinkFile, const char* vLinkDir, WORD vHotKey, const char* vDescription, int vShowCmd=SW_SHOWNORMAL);

	std::string 	GetDesktopPath				();		//得到当前桌面路径
	std::string 	GetIEQuickLaunchPath		();		//得到快速启动栏的路径
	std::string 	GetProgramsPath				();		//得到 开始->程序组 的路径

	bool			CreatLinkToDesktop			();




	/******************系统信息******************/
	bool			GetSingleDiskInfo			(const char* vName, ULONGLONG& vAll, ULONGLONG& vUsed, ULONGLONG& vFree);
	int				GetCpu						(void);	//CPU使用率
	int				GetRam						(void);	//内存使用率
	std::string		GetMac						(void);	


	/******************MD5******************/
	string			GetStrMD5					(const string vString);
	string			GetFileMD5					(const string vFileName);


	/******************权限******************/
	BOOL			EnablePriviledge				();				//权限提升,eg:EnablePriviledge(SE_DEBUG_NAME)
	BOOL			EnableFileAccountPrivilege		(char* vDestPath, char* vAccount);		//文件夹权限修改，文件路径 + 用户名
	BOOL			GetProcessPriviledge			(HANDLE vProcess);


	/******************大小端******************/
	bool			IsBigEndian						(void);


	/******************错误码******************/
	std::string		TransErrorCode					(DWORD vErrorCode);

}
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------