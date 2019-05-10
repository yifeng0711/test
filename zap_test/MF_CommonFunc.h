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
#include <Winsock2.h>	//�����ͻ��Winsock2.hҪ��Windows.hǰ��
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
	/*****************ʱ�����******************/
	/*
	* Method		_GetCurrentTime
	* @brief		��ȡ��ǰϵͳʱ�䣬��ȷ����
	* @param[in]	vTimePos   ʱ����������Ĭ��":"
	* @return		��ǰʱ����
	*/
	std::string		_GetCurrentTime				(std::string vTimePos = ":");	//��ǰʱ�䣬Ĭ�ϸ�ʽ��00:00:00
	std::string 	GetCurrentDate				(std::string vDatePos = "-");	//��ǰ���ڣ�Ĭ�ϸ�ʽ��0000-00-00
	std::string 	GetCurrentDateTime			(std::string vDatePos = "-", std::string vDtPos = " ", std::string vTimePos = ":"); //��ǰ����+ʱ�䣬Ĭ�ϸ�ʽ��0000-00-00 00:00:00
	std::string 	GetCurrentTimeWithWeek		();

	__int64			GetTimestamp				(std::string vTimes="");							//��ȡʱ���
	std::string		FmtTimestamp				(__int64 vTimestamp);		//ʱ���ת��ʽ��ʱ��

	std::string		FmtDateTime					(const std::string& vDateTime);

	std::string		GetNowTime					(void);
	std::string		GetLastWeek					(void);


	/******************�ַ�������******************/	
	std::string		STRReplace					(const std::string& vString, const std::string& vSubStrFrom, const std::string& vSubStrTo);
	std::string		ReplaceBackslashBySlash		(std::string vStr);	//��б��(/)���淴б��(\\)
	std::string		Trim						(std::string& vStr);	//ȥ��ͷβ�Ŀո�

	void			SeparateString				(char* vStr, const char* vSeps, vector<std::string>& vRet);	//�����ַ���
	void			SeparateWstring				(const std::wstring vWstr,const std::wstring WSeps,vector<std::wstring>& vRet);

	bool			SeparateFile				(const char* vPath, char* vDir, char* vFile);

	std::string		STRCombine					(char* vFormat,...);

	//C �ַ���
	void			ChWConvertEndian			(void * ptr, size_t size);
	void			ChSeparateFile				(const char* vPath, char* vFile, char* vDir);	//��path���ļ�����·���и�
	char*			ChStrReplace				(char* vStr, char* vOldStr, char* vNewStr);		//���ַ����е�ĳ���ַ�����Ϊ��һ���ַ���
	/******************�ļ��������******************/
	std::string		GetPathSection				(std::string vPath);	//��ȡ�ڶ���·�������ǰ���ַ��� E:\\123\\345\\1.txt -> E:\\123

	std::string		GetFileNameWithoutSuffix	(std::string vFileName);
	std::string		GetFileSuffix				(const std::string& vFileName);	//��׺��(������)	

	__int64			GetFileSize					(const std::string vFilePath);	//�ļ���С
	__int64			GetFolderSize				(const std::string vDir);		//Ŀ¼��С

	std::string		GetFileConten				(const char* vFilePath);	//��ȡ�ļ�����
	int				GetFileSizeFromFilePath		(const char* vFilePath);	
	int				GetFileSizeFromHandle		(FILE* vFileHandle);
	bool			LoadFile					(const char* vFilePath, char* vBuffer, int vBufLen);

	std::string		SearchFilePathByFileName	(const std::string vSearchDir, const std::string vFileName, const std::string vSuffixName);
	bool			TraversalFilesInDir			(const char* vDir, std::vector<std::string>&vFileList, const char* vSuffixName=NULL, bool vIsRecursive=true);	//����Ŀ¼
	bool			IsValidFile					(const char* vFilePath);	//��ȡ�����ֶΣ��ж��Ƿ�Ϊ��Ҫ���ļ�

	bool			SHFileDelete				(const std::string& vPath, bool vDestory=true);	//ɾ���ļ���Ŀ¼
	bool			WriteFileConten				(const char* vFileName, const char* vContent);

	//Ŀ¼
	bool			MakeEmptyDir				(const char* vPath);	//������Ŀ¼
	bool			MakeDir						(const char* vPath);	//����Ŀ¼
	bool			DeleteDir					(const char* vPath);	//ɾ��Ŀ¼
	bool			DirExist					(const char* vPath);	//Ŀ¼�Ƿ����
	bool			IsEmptyDir					(const char* vPath);	//Ŀ¼�Ƿ�Ϊ��

	//�ļ�
	bool			IsFileExist					(std::string vFileName);
	bool			MyDeleteFile				(const char* vFileName);
	bool			GetFileInfo					(char* vFilePath, std::string& vOriginalName);

	std::string		GetSignNameOfFile			(char* vFilePath);	//��ȡ�ļ�������ǩ��
	PCCERT_CONTEXT	GetFileCertificate			(char* vFilePath);	//��ȡ֤����Ϣ



	/******************����/�������******************/
	std::string 	RunFilePathName				(char* vAppend, HMODULE vHModule = 0);	//��ǰģ������·�� + vAppend
	std::string		GetModuleName				();	//ģ����test
	std::string		GetCurrentDirName			(HMODULE vHModule = 0);

	bool			GetCmdlineArgs				(std::vector<std::string>* vArgs);	//��ȡ�������в���

	bool            XLib_LoadAppAndWait     	(char* vCmdLine, int vShowMode, DWORD vTimeOut, DWORD* vExitCode);
	bool            RunCmdAndWait     			(char* vCmdLine, int vShowMode, DWORD vTimeOut, DWORD* vExitCode);

	bool			DeleteMyselfExe				(void);	//ɾ������EXE
	bool			DeleteMyselfDir				(void);	//ɾ����������Ŀ¼

	DWORD           GetProcessIdByName      	(const char* vProcessName); //��ȡ����ID�������������ʾ��ӳ������
	DWORD			GetProcessIdByTcpPort		(u_short vLocalPort);

	bool            KillProcessByName       	(const char* vProcessName);
	std::string		GetProcUserNameByPid		(int vProcId);	//��ȡ�����û�kinkoo
	std::string		GetProcessPathByPid			(int vPid);


	/******************ע������******************/
	bool			GetRegValue					(const char* vPath, const char* vValue, DWORD *vRegType, char **vData, DWORD *vLen);
	HKEY			RegKeyRoot					(const std::string& vPath);
	std::string		GetRegValueS				(const char* vPath, const char* vValue);	//��ȡע��� ��ֵ
	void			FreeRegBuff					(const char* vData);


	/******************�������******************/
	bool			check_disk_wirteable		(int vIndex);	//�������Ƿ��д

	int				GetDiskInfo					(void); 
	bool			GetDiskSpace				(std::string vName, __int64& vAllKB, __int64& vUsedKB, __int64& vFreeKB );

	//���������ص����
	bool			GetMountPointsList			(std::vector<std::string>& vMountPointList, std::string vWildcard="");	//��ȡϵͳ���ص� vWildcard��ʾƥ��Ĺؼ���
	bool			ListMountPointOfVolume		(const char* vVolumeName, std::vector<std::string>& vMountPointList, std::string vWildcard="");	//���ص��ѯ

	bool			DeleteMountPoint			(const char* vMountPoint);
	bool			SetMountPoint				(const char* vOldMountPoint, const char* vNewMountPoint);

	//test func
	bool			GetDriverString				(const char* vVolumeName);	//�̷�


	/******************����ϵͳ���******************/
	bool			IsWin7OS					();	//��׼ȷ
	bool			CheckSystemVersion			();	//׼ȷ	
	std::string		GetSystemName				(); 

	BOOL			Is64System					(void);

	bool			pc_power_off				(bool vRestart);

	bool			GetScreenResolution			(int& vLateral, int& vVertical);

	/******************windows����******************/
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

	/******************��ݷ�ʽ******************/
	/*���ɿ�ݷ�ʽ
	*vSrcFile:		Ŀ���ļ�·����ΪNULL��ʾ��ǰ���̵�EXE�ļ���������һ������·��+�ļ���	 
	*vLinkFile:	��ݷ�ʽ�ļ�����NULL ��ʾʹ�õ�ǰ�ļ���
	*vLinkDir:		��ݷ�ʽ����λ�ã�����ΪNULL
	*vHotKey:		Ϊ0��ʾ�����ÿ�ݼ�
	*vDescription: ��ע
	*vShowCmd:		���з�ʽ��Ĭ��Ϊ���洰��
	*/
	BOOL			CreateFileShortcut			(const char* vSrcFile, const char* vLinkFile, const char* vLinkDir, WORD vHotKey, const char* vDescription, int vShowCmd=SW_SHOWNORMAL);
	BOOL			CreateFileShortcutA			(const char* vSrcFile, const char* vLinkFile, const char* vLinkDir, WORD vHotKey, const char* vDescription, int vShowCmd=SW_SHOWNORMAL);

	std::string 	GetDesktopPath				();		//�õ���ǰ����·��
	std::string 	GetIEQuickLaunchPath		();		//�õ�������������·��
	std::string 	GetProgramsPath				();		//�õ� ��ʼ->������ ��·��

	bool			CreatLinkToDesktop			();




	/******************ϵͳ��Ϣ******************/
	bool			GetSingleDiskInfo			(const char* vName, ULONGLONG& vAll, ULONGLONG& vUsed, ULONGLONG& vFree);
	int				GetCpu						(void);	//CPUʹ����
	int				GetRam						(void);	//�ڴ�ʹ����
	std::string		GetMac						(void);	


	/******************MD5******************/
	string			GetStrMD5					(const string vString);
	string			GetFileMD5					(const string vFileName);


	/******************Ȩ��******************/
	BOOL			EnablePriviledge				();				//Ȩ������,eg:EnablePriviledge(SE_DEBUG_NAME)
	BOOL			EnableFileAccountPrivilege		(char* vDestPath, char* vAccount);		//�ļ���Ȩ���޸ģ��ļ�·�� + �û���
	BOOL			GetProcessPriviledge			(HANDLE vProcess);


	/******************��С��******************/
	bool			IsBigEndian						(void);


	/******************������******************/
	std::string		TransErrorCode					(DWORD vErrorCode);

}
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------