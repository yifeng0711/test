#ifndef MC_Process_H
#define MC_Process_H
//---------------------------------------------------------------------------
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//---------------------------------------------------------------------------
#include <WinSock2.h>	//��ֹ��ͻ�����������ǰ��
#include <Windows.h>
#include <vector>
#include <string>
using namespace std;
//---------------------------------------------------------------------------
class MC_Process
{
public:
	static	std::string		GetModuleName				(HMODULE vHModule = 0);					//��ȡָ��ģ������ƣ�  vHModule = 0 ��ʾ��ȡ��ǰEXE������
	static	std::string		GetMoudleDir				(HMODULE vHModule = 0);					//��ȡָ��ģ������Ŀ¼��vHModule = 0 ��ʾ��ȡ��ǰEXE������Ŀ¼
	static	std::string 	RunFilePathName				(char* vAppend, HMODULE vHModule = 0);	//[ָ��ģ������·��]\\[vAppend]

	static	bool			GetCmdlineArgs				(std::vector<std::string>& vArgs);	//��ȡ�������в���

	static	bool            XLib_LoadAppAndWait     	(char* vCmdLine, int vShowMode, DWORD* vExitCode, DWORD vTimeOut = INFINITE);	//������ֱ�������������������������bat�ļ�����ִ��bat
	static	bool            RunCmdAndWait     			(char* vCmdLine, int vShowMode, DWORD* vExitCode, DWORD vTimeOut = INFINITE);	//����ֱ����������������

	/**
	 * @Method		: DeleteMyselfExe
	 * @brief		: �����˳�������ɾ��
	 * @param[in]	: ��
	 * @return		: true=�ɹ���false=ʧ��
	 * @other		: ���ô˽ӿں󣬲����ٵ���getchar()�Ȼ�ʹ������ͣ�ĺ���������ʧ�ܣ�errno=5����������õĵ���ʱ������main����return֮ǰ��
	 */
	static	bool			DeleteMyselfExe				(void);	//ɾ������EXE
	static	bool			DeleteMyselfDir				(void);	//ɾ����������Ŀ¼

	static	bool            KillProcessByProName       	(const char* vProcessName);
	static	bool			KillProcessByPid			(DWORD vPid);

	static	bool			GetAllPidByProName			(const char* vProName, std::vector<DWORD>& vPids);
	static	DWORD			GetPidByProName				(const char* vProName);		//һ�����������ܶ�Ӧ������̣�����ȡ��һ��
	static	DWORD			GetPidByTcpPort				(u_short vLocalPort);		//һ���˿�ֻ�ܶ�Ӧһ������

	static	std::string		GetProUserByPid				(DWORD vPid);				//��ȡ���������û�
	static	std::string		GetProPathByPid				(DWORD vPid);				//��ȡ��������·��

	/**
	 * @Method		: CreateFileShortcut
	 * @brief		: ����ָ������Ŀ�ݷ�ʽ
	 * @param[in]	: vSrcFile:		Ŀ���ļ�·����ΪNULL��ʾ��ǰ���̵�EXE�ļ���������һ������·��+�ļ���	  
	 * @param[in]	: vLinkFile:	��ݷ�ʽ�ļ�����x.lnk����NULL ��ʾʹ�õ�ǰ�ļ���
	 * @param[in]	: vLinkDir:		��ݷ�ʽ����λ�ã�����ΪNULL
	 * @param[in]	: vHotKey:		Ϊ0��ʾ�����ÿ�ݼ�
	 * @param[in]	: vDescription: ��ע
	 * @param[in]	: vShowCmd:		���з�ʽ��Ĭ��Ϊ���洰��
	 * @return		: 
	 */
	static	BOOL			CreateFileShortcut			(const char* vSrcFile, const char* vLinkFile, const char* vLinkDir, WORD vHotKey, const char* vDescription, int vShowCmd=SW_SHOWNORMAL);
	static	bool			CreatLinkToDesktop			();

	static	std::string 	GetDesktopPath				();		//�õ���ǰ����·��
	static	std::string 	GetIEQuickLaunchPath		();		//�õ�������������·��
	static	std::string 	GetProgramsPath				();		//�õ� ��ʼ->������ ��·��

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

//���󣬲������ɱ����ļ�
tRet = MC_Process::XLib_LoadAppAndWait(tCmd, SW_SHOW, &tExitCode);
if(false == tRet || 1 == tExitCode)
	LOG_ERROR("XLib_LoadAppAndWait fail");

//��ȷ���������ɱ����ļ�
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