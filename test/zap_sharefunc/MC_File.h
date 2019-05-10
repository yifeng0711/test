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
	 * @brief		: 遍历目录，返回文件列表
	 * @param[in]	: vIsRecursive	是否递归，true=递归，false=不递归
	 * @return		: true=成功，false=失败
	 */
	static	bool			TraversalFilesInDir			(const char* vDirName, std::vector<std::string>&vFileList, const char* vSuffixName=NULL, bool vIsRecursive=true);	
	static	bool			IsValidFile					(const char* vFilePath);	//提取特征字段，判断是否为需要的文件，根据实际情况修改

	static	void			ChSeparateFile				(const char* vPath, char* vDirName, char* vFile);	//将path的文件名和路径切割, vFile和vDirName需要自己释放
	static	std::string		GetPathSection				(const std::string vPath);	//获取第二个路径间隔符前的字符串 E:\\123\\345\\1.txt -> E:\\123

	static	std::string		GetFilePrefix				(const std::string vFileName);		//不带后缀的文件名
	static	std::string		GetFileSuffix				(const std::string vFileName);	//文件的后缀名(不带点)	

	static	__int64			GetFileSize					(const std::string vFilePath);	//文件大小
	static	__int64			GetFolderSize				(const std::string vDirName);		//目录大小

	//非二进制文件读写
	static	std::string		GetFileConten				(const char* vFilePath);	//无法读取二进制，比如图片、rar格式的文件
	static	bool			SetFileContent				(const char* vFileName, const char* vContent);

	//二进制文件读写
	static	bool			ReadFileContent				(const char* vFileName, void** vContent,      int* vContentSize);	//可以读写二进制
	static	bool			WriteFileContent			(const char* vFileName, const void* vContent, int  vContentSize);
	static	void			SafeFreeBuffer				(void* vContentBuffer);												//释放上面两个接口申请的指针

	//断点重续
	static	bool			ContinueCopy				(const char* vSrcFile, const char* vDestFile);	//若存在同名文件，则进行续传

	/**
	 * @Method		: SHFileDelete
	 * @brief		: 删除文件或目录
	 * @param[in]	: vDestory	是否完全删除，false=放进回收站
	 * @return		: true=成功，false=失败
	 */
	static	bool			SHFileDelete				(const std::string vPath, bool vDestory=true);	//删除文件或目录
	static	bool			MyDeleteFile				(const char* vFileName);

	static	bool			IsFileExist					(const char* vFileName);
	static	bool			IsDirExist					(const char* vDirName);	//目录是否存在

	static	bool			MakeDir						(const char* vDirName);	//创建目录
	static	bool			RemakeEmptyDir				(const char* vDirName);	//重建目录并清空，会删除目录内的文件!!!
	static	bool			IsEmptyDir					(const char* vDirName);	//目录是否为空

	/**
	 * @Method		: GetFileInfo
	 * @brief		: 获取文件属性 - 详细信息 中的值
	 * @param[out]	: vOriginalName	获取的属性值
	 * @return		: true=成功，false=失败
	 */
	static	bool			GetFileInfo					(const char* vFileName, std::string& vOriginalName);
	static	std::string		GetSignNameOfFile			(const char* vFilePath);	//获取文件的数字签名
	static	PCCERT_CONTEXT	GetFileCertificate			(const char* vFilePath);	//获取证书信息
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