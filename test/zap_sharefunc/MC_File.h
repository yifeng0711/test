#ifndef MC_File_H
#define MC_File_H
//---------------------------------------------------------------------------
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//---------------------------------------------------------------------------
#include <Windows.h>
#include <vector>
#include <string>
using namespace std;

#include <WinCrypt.h>
//---------------------------------------------------------------------------
class MC_File
{
public:
	/**
	 * @Method		: TraversalFilesInDir
	 * @brief		: ����Ŀ¼�������ļ��б�
	 * @param[in]	: vIsRecursive	�Ƿ�ݹ飬true=�ݹ飬false=���ݹ�
	 * @return		: true=�ɹ���false=ʧ��
	 */
	static	bool			TraversalFilesInDir			(const char* vDirName, std::vector<std::string>&vFileList, const char* vSuffixName=NULL, bool vIsRecursive=true);	
	static	bool			IsValidFile					(const char* vFilePath);	//��ȡ�����ֶΣ��ж��Ƿ�Ϊ��Ҫ���ļ�������ʵ������޸�

	static	void			ChSeparateFile				(const char* vPath, char* vDirName, char* vFile);	//��path���ļ�����·���и�, vFile��vDirName��Ҫ�Լ��ͷ�
	static	std::string		GetPathSection				(const std::string vPath);	//��ȡ�ڶ���·�������ǰ���ַ��� E:\\123\\345\\1.txt -> E:\\123

	static	std::string		GetFilePrefix				(const std::string vFileName);		//������׺���ļ���
	static	std::string		GetFileSuffix				(const std::string vFileName);	//�ļ��ĺ�׺��(������)	

	static	__int64			GetFileSize					(const std::string vFilePath);	//�ļ���С
	static	__int64			GetFolderSize				(const std::string vDirName);		//Ŀ¼��С

	//�Ƕ������ļ���д
	static	std::string		GetFileConten				(const char* vFilePath);	//�޷���ȡ�����ƣ�����ͼƬ��rar��ʽ���ļ�
	static	bool			SetFileContent				(const char* vFileName, const char* vContent);

	//�������ļ���д
	static	bool			ReadFileContent				(const char* vFileName, void** vContent,      int* vContentSize);	//���Զ�д������
	static	bool			WriteFileContent			(const char* vFileName, const void* vContent, int  vContentSize);
	static	void			SafeFreeBuffer				(void* vContentBuffer);												//�ͷ����������ӿ������ָ��

	//�ϵ�����
	static	bool			ContinueCopy				(const char* vSrcFile, const char* vDestFile);	//������ͬ���ļ������������

	/**
	 * @Method		: SHFileDelete
	 * @brief		: ɾ���ļ���Ŀ¼
	 * @param[in]	: vDestory	�Ƿ���ȫɾ����false=�Ž�����վ
	 * @return		: true=�ɹ���false=ʧ��
	 */
	static	bool			SHFileDelete				(const std::string vPath, bool vDestory=true);	//ɾ���ļ���Ŀ¼
	static	bool			MyDeleteFile				(const char* vFileName);

	static	bool			IsFileExist					(const char* vFileName);
	static	bool			IsDirExist					(const char* vDirName);	//Ŀ¼�Ƿ����

	static	bool			MakeDir						(const char* vDirName);	//����Ŀ¼
	static	bool			RemakeEmptyDir				(const char* vDirName);	//�ؽ�Ŀ¼����գ���ɾ��Ŀ¼�ڵ��ļ�!!!
	static	bool			IsEmptyDir					(const char* vDirName);	//Ŀ¼�Ƿ�Ϊ��

	/**
	 * @Method		: GetFileInfo
	 * @brief		: ��ȡ�ļ����� - ��ϸ��Ϣ �е�ֵ
	 * @param[out]	: vOriginalName	��ȡ������ֵ
	 * @return		: true=�ɹ���false=ʧ��
	 */
	static	bool			GetFileInfo					(const char* vFileName, std::string& vOriginalName);
	static	std::string		GetSignNameOfFile			(const char* vFilePath);	//��ȡ�ļ�������ǩ��
	static	PCCERT_CONTEXT	GetFileCertificate			(const char* vFilePath);	//��ȡ֤����Ϣ
	static	BOOL			PrintCertificateInfo		(PCCERT_CONTEXT vPCertContext);

private:
	static	char*			ChStrReplace				(char* vStr, char* vOldStr, char* vNewStr);		
	static	std::string		STRReplace					(const std::string vString, const std::string vSubStrFrom, const std::string vSubStrTo);
	static	void			SeparateString				(char* vStr, const char* vSeps,	vector<std::string>& vRet);	

	static	std::string		UnicodeToACP				(const wchar_t* vBuf);
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
/*
char tPath[]	= "C:\\music\\1.mp3";
char* tDir		= new char[MAX_PATH];
char* tFile		= new char[MAX_PATH];
memset(tDir,  0, MAX_PATH);
memset(tFile, 0, MAX_PATH);

MC_File::ChSeparateFile(tPath, tDir, tFile);
cout << "Dir: "  << tDir << endl;		//C:/music
cout << "File: " << tFile << endl;		//1.mp3
SafeDelArrayPoint(tDir);
SafeDelArrayPoint(tFile);
*/

/*
unsigned char* tCotent = NULL;
int tSize = 0;
MC_File::ReadFileContent("../3rdData/1.jpg", (void**)&tCotent, &tSize);
MC_File::WriteFileContent("../3rdData/2.jpg", tCotent, tSize);
MC_File::SafeFreeBuffer(tCotent);
*/

/*
std::string tInfo = "";
MC_File::GetFileInfo("../3rdData/QtCore4.dll", tInfo);
cout << tInfo << endl; 

cout << MC_File::GetSignNameOfFile("../3rdData/databaseaccess.dll") << endl;
PCCERT_CONTEXT tContext = MC_File::GetFileCertificate("../3rdData/databaseaccess.dll");

MC_File::PrintCertificateInfo(tContext);
*/