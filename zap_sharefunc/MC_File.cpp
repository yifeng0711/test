#include "MC_File.h"
#include "MC_Log.h"

#include <fstream>
#include <sstream>

#include <io.h>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <shlobj.h>

#include <WinCrypt.h>
#pragma comment(lib, "Crypt32.lib")

#include <tchar.h>
//---------------------------------------------------------------------------
bool MC_File::TraversalFilesInDir(const char* vDirName, std::vector<std::string>&vFileList, const char* vSuffixName/* =NULL */, bool vIsRecursive/* =true */)
{
	if(0 == vDirName)		return false;
	if(0 == *vDirName)		return false;

	string tRootDir = STRReplace(vDirName, "\\", "/");
	if('/' != tRootDir[tRootDir.size()-1])
		tRootDir += "/";

	string tRootFile = tRootDir + "*.*";
	WIN32_FIND_DATAA tFindFileData;
	HANDLE tFindHandle = FindFirstFileA(tRootFile.c_str(), &tFindFileData);
	if(INVALID_HANDLE_VALUE == tFindHandle)
	{
		LOG_ERROR("INVALID_HANDLE_VALUE, tRootFile: %s, errno: %d", tRootFile.c_str(), GetLastError());
		return false;
	}

	do
	{
		if(0 == strcmp(tFindFileData.cFileName, ".") || 0 == strcmp(tFindFileData.cFileName, ".."))
		{
			continue;
		}

		if(tFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(false == vIsRecursive)
				continue;

			std::string tSubDir = tRootDir + tFindFileData.cFileName;
			TraversalFilesInDir(tSubDir.c_str(), vFileList, vSuffixName, vIsRecursive);
		}

		std::string tFileName = tRootDir + tFindFileData.cFileName;
		if(NULL != vSuffixName)
		{
			if(vSuffixName != tFileName.substr(tFileName.rfind(".")+1))		//只返回后缀为vSuffixName的文件，当然也可以改为过滤
				continue;
		}

		vFileList.push_back(tFileName);

	}while(FindNextFileA(tFindHandle, &tFindFileData));

	FindClose(tFindHandle);
	return true;
}
//---------------------------------------------------------------------------
bool MC_File::IsValidFile(const char* vFilePath)
{
	if(NULL == vFilePath)
		return false;

	FILE* tFp;
	if(fopen_s(&tFp, vFilePath, "rb") == 0)
	{
		char tBuffer[1024];
		fread(tBuffer, 1, 1023, tFp);
		fclose(tFp);

		const char* tp = strstr(tBuffer, "\r\n");	//取一行数据验证
		if(NULL != tp)
		{
			tBuffer[tp - tBuffer] = '\0';

			vector<string>	tTemp;
			SeparateString(tBuffer, "#", tTemp);
			if(tTemp.size() == 4)
				return true;
		}
	}

	return false;
}
//---------------------------------------------------------------------------
void MC_File::ChSeparateFile(const char* vPath, char* vDirName, char* vFile)
{
	//将所有的\\转换为/
	char tPath[MAX_PATH] = {0};
	strcpy_s(tPath, strlen(vPath)+1, vPath);
	ChStrReplace(tPath, "\\", "/");

	strcpy_s(vFile, MAX_PATH, tPath);	//先转为char*，必须
	char* tPos = strrchr(vFile, '/');
	if(0 == tPos)
		return;

	*tPos = 0;
	strcpy_s(vDirName,	MAX_PATH, vFile);
	strcpy_s(vFile, MAX_PATH, tPos+1);
}
//---------------------------------------------------------------------------
std::string MC_File::GetPathSection(const std::string vPath)
{
	std::size_t tPos	=	std::string::npos;
	std::size_t tPos1	=	vPath.find("\\");
	std::size_t tPos2	=	vPath.find("/");

	if(std::string::npos != tPos1 && std::string::npos != tPos2)
	{
		tPos = tPos1 < tPos2 ? tPos1 : tPos2;
	}
	else
	{
		tPos = std::string::npos != tPos1 ? tPos1 : tPos2;
	}

	if(std::string::npos != tPos)
	{
		tPos1	=	vPath.find("\\",tPos+1);
		tPos2	=	vPath.find("/",tPos+1);
		if(std::string::npos != tPos1 && std::string::npos != tPos2)
		{
			tPos = tPos1 < tPos2 ? tPos1 : tPos2;
		}
		else
		{
			tPos = std::string::npos != tPos1 ? tPos1 : tPos2;
		}

		if(std::string::npos != tPos)
		{
			return vPath.substr(0, tPos);
		}
	}
	return vPath;
}
//---------------------------------------------------------------------------
std::string MC_File::GetFilePrefix(const std::string vFileName)
{
	std::size_t tPos = vFileName.rfind(".");
	if(std::string::npos != tPos)
	{
		return vFileName.substr(0, tPos);
	}
	else
	{
		return vFileName;
	}
}
//---------------------------------------------------------------------------
std::string MC_File::GetFileSuffix(const std::string vFileName)
{
	std::size_t tPos = vFileName.rfind(".");
	if(std::string::npos != tPos)
	{
		return vFileName.substr(vFileName.rfind(".") + 1);
	}
	else
	{
		return "";
	}
}
//---------------------------------------------------------------------------
__int64 MC_File::GetFileSize(const std::string vFilePath)
{
	HANDLE tFileHandle = CreateFileA(vFilePath.c_str(), FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (INVALID_HANDLE_VALUE != tFileHandle)
	{
		LARGE_INTEGER tFileSize = {0};
		GetFileSizeEx(tFileHandle, &tFileSize);
		CloseHandle(tFileHandle);
		return tFileSize.QuadPart;
	}

	return 0;
}
//---------------------------------------------------------------------------
__int64 MC_File::GetFolderSize(const std::string vDirName)  
{  
	std::string	tRootPath	= STRReplace(vDirName, "\\", "/");
	if('/' != tRootPath[tRootPath.size()-1])
		tRootPath += '/';

	std::string	tRootFile	= tRootPath + "*.*"; 
	__int64		tSize		= 0;  

	WIN32_FIND_DATAA tFindData;  
	HANDLE tFindHandle = FindFirstFileA(tRootFile.c_str(), &tFindData);  
	if (INVALID_HANDLE_VALUE == tFindHandle)  
		return tSize; 

	do 
	{
		if (strcmp(tFindData.cFileName, ".") == 0 || strcmp(tFindData.cFileName, "..") == 0)  
			continue; 

		std::string tSubPath = tRootPath + tFindData.cFileName;  
		if (tFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	
		{
			if(tFindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)	//外挂目录不要
				tSize += GetFolderSize(tSubPath);  
			else
				continue;
		}
		else  
		{
			tSize += GetFileSize(tSubPath);
		}  
	} while(FindNextFileA(tFindHandle, &tFindData));

	FindClose(tFindHandle);  
	return tSize;  
} 
//---------------------------------------------------------------------------
std::string MC_File::GetFileConten(const char* vFilePath)
{
	if(0 == vFilePath)	return false;
	if(0 == *vFilePath)	return false;

	FILE* tFp = NULL;
	errno_t tError = fopen_s(&tFp, vFilePath, "rb");
	if(0 != tError)
	{
		LOG_ERROR("fopen_s fail! file: %s, errno: %d", vFilePath, GetLastError());
		return "";
	}

	int tFileSize = 0;
	int tCurrPos  = 0;
	tCurrPos = ftell(tFp);
	fseek(tFp, 0, SEEK_END);
	tFileSize = ftell(tFp);
	fseek(tFp, tCurrPos, SEEK_SET);
	if(0 == tFileSize)
	{
		LOG_ERROR("FileSize = 0, file: %s", vFilePath);
		fclose(tFp);
		return "";
	}

	char*	tBuffer		= new char[tFileSize + 1];
	memset(tBuffer, 0, tFileSize + 1);
	int		tReadSize	= 0;
	size_t	tReadNum	= 0;
	while(tReadSize < tFileSize)
	{
		tReadNum	=	fread(tBuffer, 1, tFileSize - tReadSize, tFp);
		tReadSize	+=	tReadNum;
	}
	fclose(tFp);

	std::string tFileConten = tBuffer;
	delete tBuffer;
	tBuffer = NULL;


	/*std::string tFileConten = "";
	std::ifstream tIn(vFilePath, std::ios::in | std::ios::binary);
	if (tIn) 
	{
	std::ostringstream tOss;
	tOss << tIn.rdbuf();
	tFileConten.assign(tOss.str());
	} */

	return tFileConten;
}
//---------------------------------------------------------------------------
bool MC_File::SetFileContent(const char* vFileName, const char* vContent)
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
bool MC_File::ReadFileContent(const char* vFileName, void** vContent, int* vContentSize)
{
	if(0 == vFileName)	return false;
	if(0 == *vFileName)	return false;

	FILE* tFp = NULL;
	errno_t tError = fopen_s(&tFp, vFileName, "rb");
	if(0 != tError)
	{
		LOG_ERROR("fopen_s fail! file: %s, errno: %d", vFileName, tError);
		return false;
	}

	int tFileSize = 0;
	int tCurrPos  = 0;
	tCurrPos = ftell(tFp);
	fseek(tFp, 0, SEEK_END);
	tFileSize = ftell(tFp);
	fseek(tFp, tCurrPos, SEEK_SET);
	if(0 == tFileSize)
	{
		LOG_ERROR("FileSize = 0, file: %s", vFileName);
		fclose(tFp);
		return "";
	}
	
	*vContent = new unsigned char[tFileSize + 1];
	memset(*vContent, 0, tFileSize+1);

	int		tReadSize	= 0;
	size_t	tReadNum	= 0;
	while(tReadSize < tFileSize)
	{
		tReadNum	=	fread(*vContent, 1, tFileSize - tReadSize, tFp);
		tReadSize	+=	tReadNum;
	}
	fclose(tFp);

	*vContentSize	= tFileSize;

	return true;
}
//---------------------------------------------------------------------------
bool MC_File::WriteFileContent(const char* vFileName, const void* vContent, int vContentSize)
{
	if(0 == vFileName)	return false;
	if(0 == *vFileName)	return false;
	if(0 == vContent)	return false;

	FILE* tFp = NULL;
	if(0 != fopen_s(&tFp, vFileName, "wb++"))
	{
		return false;
	}

	fwrite(vContent, 1, vContentSize, tFp);  
	fflush(tFp);
	fclose(tFp);

	return true;
}
//---------------------------------------------------------------------------
void MC_File::SafeFreeBuffer(void* vContent)
{
	if(0 == vContent)
		return;

	delete[] vContent;
	vContent = 0;		//好像不起效果
}
//---------------------------------------------------------------------------
bool MC_File::ContinueCopy(const char* vSrcFile, const char* vDestFile)
{
	if(0 == vSrcFile)	return false;
	if(0 == *vSrcFile)	return false;
	if(0 == vDestFile)	return false;
	if(0 == vDestFile)	return false;

	FILE*	tSrcFp	= 0;
	int	tSrcSize	= 0;
	int tSrcCurPos	= 0;

	if(0 != fopen_s(&tSrcFp, vSrcFile, "rb"))
	{
		return false;
	}

	tSrcCurPos = ftell(tSrcFp);
	fseek(tSrcFp, 0, SEEK_END);
	tSrcSize = ftell(tSrcFp);
	fseek(tSrcFp, tSrcCurPos, SEEK_SET);

	FILE*	tDestFp	= 0;
	int	tDestSize	= 0;
	int tDestCurPos	= 0;

	errno_t tss = fopen_s(&tDestFp, vDestFile, "wb");
	if(0 != fopen_s(&tDestFp, vDestFile, "wb"))	
	{
		fclose(tSrcFp);
		return false;
	}

	tDestCurPos = ftell(tDestFp);
	fseek(tDestFp, 0, SEEK_END);
	tDestSize = ftell(tDestFp);
	fseek(tDestFp, tDestCurPos, SEEK_SET);

	char	tBuffer[512]	= {0};
	size_t	tNumRead		= 0;
	size_t	tNumWrite		= 0;
	size_t	tNumCopy		= 0;

	//从中断的地方开始复制
	//对比中断部分的值是否一样，判断是否同个文件
	fseek(tSrcFp,  tDestSize, SEEK_SET);
	fseek(tDestFp, tDestSize, SEEK_SET);

	tNumRead = fread(tBuffer, 1, sizeof(tBuffer), tSrcFp);
	while(0 < tNumRead)
	{
		tNumWrite = fwrite(tBuffer, 1, tNumRead, tDestFp);
		if(tNumWrite != tNumRead)
		{
			fclose(tSrcFp);
			fclose(tDestFp);
			return false;
		}

		tNumCopy += tNumWrite;
		tNumRead = fread(tBuffer, 1, sizeof(tBuffer), tSrcFp);
	}

	

	tDestCurPos = ftell(tDestFp);
	fseek(tDestFp, 0, SEEK_END);
	tDestSize = ftell(tDestFp);
	fseek(tDestFp, tDestCurPos, SEEK_SET);

	fclose(tSrcFp);
	fclose(tDestFp);

	if(tSrcSize != tDestSize)
		return false;

	return true;
}
//---------------------------------------------------------------------------
bool MC_File::SHFileDelete(const std::string vPath, bool vDestory)
{
	if(0 == vPath.size())
		return false;

	//删除文件，pFrom参数必须以2个\0结尾
	std::string tFrom = vPath;
	tFrom.resize(tFrom.size()+1);
	tFrom[tFrom.size()-1] = 0;

	//删除文件到回收站
	SHFILEOPSTRUCTA tFileOp = {0};
	tFileOp.wFunc	= FO_DELETE;
	tFileOp.pFrom	= tFrom.c_str();
	tFileOp.fFlags	= FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;	//不显示进度，不进行确认，不报告错误信息

	//如果不是销毁，则放进回收站
	if(false == vDestory)
		tFileOp.fFlags |= FOF_ALLOWUNDO; 

	int tRet = SHFileOperationA(&tFileOp);
	if(ERROR_SUCCESS != tRet)
	{
		LOG_ERROR("SHFileOperationA fail! errno: %d", tRet);
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool MC_File::MyDeleteFile(const char* vFileName)
{
	DWORD tFileAttribute = GetFileAttributesA(vFileName);
	if(1 == (tFileAttribute & FILE_SHARE_READ))
	{
		SetFileAttributesA(vFileName, tFileAttribute & ~FILE_SHARE_READ);	//去掉只读属性
	}
	if(false ==DeleteFileA(vFileName))
	{
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
bool MC_File::IsFileExist(const char* vFileName)
{
	if(0 == vFileName)		return false;
	if(0 == *vFileName)		return false;

	return 0 == _access(vFileName, 0) ? true : false;
}
//---------------------------------------------------------------------------
bool MC_File::IsDirExist(const char* vDirName)
{
	if(0 == vDirName)		return false;
	if(0 == *vDirName)		return false;

	return FALSE != PathIsDirectoryA(vDirName);
}
//---------------------------------------------------------------------------
bool MC_File::MakeDir(const char* vDirName)
{
	if(0 == vDirName)		return false;
	if(0 == *vDirName)		return false;

	int tRet = SHCreateDirectoryExA(NULL, vDirName, NULL);
	if(ERROR_ALREADY_EXISTS == tRet || ERROR_SUCCESS == tRet)
		return true;

	return false;
}
//---------------------------------------------------------------------------
bool MC_File::RemakeEmptyDir(const char* vDirName)
{
	if(0 == vDirName)		return false;
	if(0 == *vDirName)		return false;

	if(false != IsDirExist(vDirName))	//若存在则先清空
	{
		if(false == SHFileDelete(vDirName))
		{
			LOG_ERROR("DeleteDir %s fail", vDirName);
			return false;
		}
	}

	if(false == MakeDir(vDirName))
	{
		LOG_ERROR("MakeDir: %s fail", vDirName);
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool MC_File::IsEmptyDir(const char* vDirName)
{
	if(0 == vDirName)		return false;
	if(0 == *vDirName)		return false;

	char tPath[MAX_PATH] = {0};
	sprintf_s(tPath, MAX_PATH, "%s\\*.*", vDirName);

	WIN32_FIND_DATAA tFindFileData;
	HANDLE tFindHandle = FindFirstFileA(tPath, &tFindFileData);
	if(INVALID_HANDLE_VALUE == tFindHandle)
	{
		LOG_ERROR("INVALID_HANDLE_VALUE, path: %s, errno: %d", tPath, GetLastError());
		return false;
	}
	do
	{
		if(0 == strcmp(tFindFileData.cFileName, ".") || 0 == strcmp(tFindFileData.cFileName, ".."))
			continue;

		FindClose(tFindHandle); 
		return false;

	}while(FindNextFileA(tFindHandle, &tFindFileData));

	FindClose(tFindHandle);  
	return true;
}
//---------------------------------------------------------------------------
bool MC_File::GetFileInfo(const char* vFileName, std::string& vOriginalName)
{
	if(0 == vFileName)		return false;
	if(0 == *vFileName)		return false;

	HMODULE tModule = GetModuleHandleA("version.dll");
	if(0 == tModule)
	{
		tModule = LoadLibraryA("version.dll");
		if(0 == tModule)
		{
			LOG_ERROR("%d", GetLastError());
			return false;
		}
	}

	typedef DWORD	(WINAPI* PGetFileVersionInfoSizeA)	(LPCSTR, LPDWORD);
	typedef BOOL	(WINAPI* PGetFileVersionInfoA)		(LPCSTR, DWORD, DWORD, LPVOID);
	typedef BOOL	(WINAPI* PVerQueryValueA)			(const LPVOID, LPCSTR, LPVOID*, PUINT);

	PGetFileVersionInfoSizeA	tMyGetFileVersionInfoSizeA	= (PGetFileVersionInfoSizeA)GetProcAddress(tModule, "GetFileVersionInfoSizeA");
	PGetFileVersionInfoA		tMyGetFileVersionInfoA		= (PGetFileVersionInfoA)	GetProcAddress(tModule, "GetFileVersionInfoA");
	PVerQueryValueA				tMyVerQueryValueA			= (PVerQueryValueA)			GetProcAddress(tModule, "VerQueryValueA");
	if(0 == tMyGetFileVersionInfoSizeA)	return false;
	if(0 == tMyGetFileVersionInfoA)		return false;
	if(0 == tMyVerQueryValueA)			return false;

	DWORD	tHandle		= 0;
	DWORD	tInfoSize	= tMyGetFileVersionInfoSizeA(vFileName, &tHandle);
	if(0 == tInfoSize)
		return false;

	BYTE* tData = new BYTE[tInfoSize];

	if(0 < tMyGetFileVersionInfoA(vFileName, 0, tInfoSize, tData))
	{
		struct LANGANDCODEPAGE
		{
			WORD wLanguage;
			WORD wCodePage;
		} *lpTranslate;

		UINT tTranslate = 0;
		if(0 == tMyVerQueryValueA((void*)tData, "\\VarFileInfo\\Translation", (void**)&lpTranslate, &tTranslate))
			return false;

		if((int)(tTranslate/sizeof(struct LANGANDCODEPAGE)) < 0)
			return false;

		char*	tRevBuff		= 0;
		UINT	tSizeBuf		= 0;
		char	tSubBlock[128]	= {0};

		/*
		* FileDescription	获取文件说明 
		* FileVersion		获取文件版本
		* InternalName		获取内部名称  
		* CompanyName		获取公司名称  
		* LegalCopyright	获取版权  
		* OriginalFilename	获取原始文件名
		* ProductName		获取产品名称
		* ProductVersion	获取产品版本  
		*/
		wsprintfA(tSubBlock, "\\StringFileInfo\\%04x%04x\\OriginalFilename", lpTranslate[0].wLanguage, lpTranslate[0].wCodePage);
		if(0 != tMyVerQueryValueA((void*)tData, tSubBlock, (void**)&tRevBuff, &tSizeBuf))
		{
			ZeroMemory((void*)tSubBlock, 128);
			strncpy(tSubBlock, tRevBuff, 127);
			vOriginalName = tSubBlock;
		}
	}

	return true;
}
//---------------------------------------------------------------------------
std::string MC_File::GetSignNameOfFile(const char* vFilePath)
{
	if(0 == vFilePath)		return "";
	if(0 == *vFilePath)		return "";

	if(false == IsFileExist(vFilePath))
		return "";

	PCCERT_CONTEXT tContext = GetFileCertificate(vFilePath);
	if(0 == tContext)
		return "";

	if(0 == tContext->pCertInfo)
	{
		CertFreeCertificateContext(tContext);
		return "";
	}

	DWORD tSize = CertGetNameString(tContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, 0, 0, 0);
	if(0 == tSize)
	{
		CertFreeCertificateContext(tContext);
		return "";
	}

	LPTSTR tName = (LPTSTR)LocalAlloc(LPTR, tSize*sizeof(TCHAR));
	if(0 == tName)
	{
		CertFreeCertificateContext(tContext);
		return "";
	}

	std::string tSignName = "";
	if(0 < CertGetNameString(tContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, 0, tName, tSize))
		tSignName = UnicodeToACP(tName);

	LocalFree(tName);
	CertFreeCertificateContext(tContext);

	return tSignName;
}
//---------------------------------------------------------------------------
PCCERT_CONTEXT MC_File::GetFileCertificate(const char* vFilePath)
{
	if(0 == vFilePath)	return 0;
	if(0 == *vFilePath)	return 0;

	WCHAR tSignedFile[MAX_PATH] = {0};
	mbstowcs(tSignedFile, vFilePath, MAX_PATH);

	DWORD 		tEncoding 		= 0;
	DWORD 		tContentType	= 0;
	DWORD 		tFormatType		= 0;
	HCERTSTORE 	tStore   		= 0;
	HCRYPTMSG 	tMsg			= 0;
	BOOL tResult = CryptQueryObject(CERT_QUERY_OBJECT_FILE, tSignedFile, CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED, CERT_QUERY_FORMAT_FLAG_BINARY, 0,
		&tEncoding, &tContentType, &tFormatType, &tStore, &tMsg, 0);

	if(FALSE == tResult)
	{
		printf("%d", GetLastError());
		return 0;
	}

	//获取数字签名签名名字信息的大小
	DWORD	tSignerInfoSize = 0;
	if(FALSE == CryptMsgGetParam(tMsg, CMSG_SIGNER_INFO_PARAM, 0, 0, &tSignerInfoSize))
	{
		CryptMsgClose(tMsg);
		CertCloseStore(tStore, CERT_CLOSE_STORE_CHECK_FLAG);
		return 0;
	}

	//分配数据签名信息内存
	PCMSG_SIGNER_INFO tSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, tSignerInfoSize);
	if(0 == tSignerInfo)
	{
		CryptMsgClose(tMsg);
		CertCloseStore(tStore, CERT_CLOSE_STORE_CHECK_FLAG);
		return 0;
	}

	//获取签名者的信息
	if(FALSE == CryptMsgGetParam(tMsg, CMSG_SIGNER_INFO_PARAM, 0, (PVOID)tSignerInfo, &tSignerInfoSize))
	{
		LocalFree(tSignerInfo);
		CryptMsgClose(tMsg);
		CertCloseStore(tStore, CERT_CLOSE_STORE_CHECK_FLAG);
		return 0;
	}


	CERT_INFO tCertInfo;
	tCertInfo.Issuer 		= tSignerInfo->Issuer;
	tCertInfo.SerialNumber 	= tSignerInfo->SerialNumber;

	PCCERT_CONTEXT tCert 	= CertFindCertificateInStore(tStore, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, 0, CERT_FIND_SUBJECT_CERT, (PVOID)&tCertInfo, 0);

	LocalFree(tSignerInfo);
	CryptMsgClose(tMsg);
	CertCloseStore(tStore, CERT_CLOSE_STORE_CHECK_FLAG);

	return tCert;
}
//---------------------------------------------------------------------------
BOOL MC_File::PrintCertificateInfo(PCCERT_CONTEXT vPCertContext)
{
	BOOL	tReturn	= FALSE;
	LPTSTR	tName	= NULL;
	DWORD	tData	= 0;

	__try
	{
		_tprintf(_T("Serial Number: "));
		tData = vPCertContext->pCertInfo->SerialNumber.cbData;
		for(DWORD i=0; i<tData; i++)
		{
			_tprintf(_T("%02x "), vPCertContext->pCertInfo->SerialNumber.pbData[tData - (i + 1)]);
		}
		_tprintf(_T("\n"));

		// Get Issuer name size.
		tData = CertGetNameString(vPCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, NULL, 0);
		if(0 == tData)
		{
			_tprintf(_T("CertGetNameString failed.\n"));
			__leave;
		}

		// Allocate memory for Issuer name.
		tName = (LPTSTR)LocalAlloc(LPTR, tData * sizeof(TCHAR));
		if(NULL == tName)
		{
			_tprintf(_T("Unable to allocate memory for issuer name.\n"));
			__leave;
		}

		// Get Issuer name.
		if(0 == CertGetNameString(vPCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, tName, tData))
		{
			_tprintf(_T("CertGetNameString failed.\n"));
			__leave;
		}

		// print Issuer name.
		_tprintf(_T("Issuer Name: %s\n"), tName);
		LocalFree(tName);
		tName = NULL;

		// Get Subject name size.
		if(0 == CertGetNameString(vPCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, NULL, 0))
		{
			_tprintf(_T("CertGetNameString failed.\n"));
			__leave;
		}

		// Allocate memory for subject name.
		tName = (LPTSTR)LocalAlloc(LPTR, tData * sizeof(TCHAR));
		if(NULL == tName)
		{
			_tprintf(_T("Unable to allocate memory for subject name.\n"));
			__leave;
		}

		// Get subject name.
		if(0 == CertGetNameString(vPCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, tName,tData))
		{
			_tprintf(_T("CertGetNameString failed.\n"));
			__leave;
		}

		// Print Subject Name.
		_tprintf(_T("Subject Name: %s\n"), tName);
		tReturn = TRUE;
	}

	__finally
	{
		if(NULL != tName)
			LocalFree(tName);
	}
		
	return tReturn;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//---------------------------------------------------------------------------
char* MC_File::ChStrReplace(char* vStr, char* vOldStr, char* vNewStr)
{
	char tStr[MAX_PATH];
	memset(tStr, 0, sizeof(tStr));

	for(int i=0; i<strlen(vStr); i++)
	{
		if(0 == strncmp(vStr+i, vOldStr, strlen(vOldStr)))	//查找目标字符串
		{
			strcat(tStr, vNewStr);
			i += strlen(vOldStr) -1;
		}
		else
		{
			strncat(tStr, vStr + i, 1);	//保存一字节进缓冲区
		}
	}

	strcpy_s(vStr, sizeof(tStr), tStr);
	return vStr;
}
//---------------------------------------------------------------------------
std::string MC_File::STRReplace(const std::string vString, const std::string vSubStrFrom, const std::string vSubStrTo)
{
	std::string				tString		= vString;
	std::string::size_type	tPos		= 0;
	std::string::size_type	tFormLen	= vSubStrFrom.size();
	std::string::size_type	tToLen		= vSubStrTo.size();

	while( (tPos=tString.find(vSubStrFrom, tPos)) != std::string::npos)
	{
		tString.replace( tPos, tFormLen, vSubStrTo);
		tPos += tToLen;
	}

	return tString;
}
//---------------------------------------------------------------------------
void MC_File::SeparateString(char* vStr, const char* vSeps, vector<std::string>& vRet)
{
	if(NULL == vStr)		return ;
	if(NULL == vSeps)		return ;

	char* tToken		=	NULL;
	char* tNextToken	=	NULL;
	tToken = strtok_s(vStr, vSeps, &tNextToken);	
	while(NULL != tToken)
	{
		vRet.push_back(tToken);	
		tToken = strtok_s(NULL, vSeps, &tNextToken);
	}
}
//---------------------------------------------------------------------------
std::string MC_File::UnicodeToACP(const wchar_t* vBuf)
{
	int tLen = WideCharToMultiByte(CP_ACP, 0, vBuf, -1, NULL, 0, NULL, NULL);
	std::vector<char> tUft8(tLen);
	WideCharToMultiByte(CP_ACP, 0, vBuf, -1, &tUft8[0], tLen, NULL, NULL);

	return std::string(&tUft8[0]);
}
//---------------------------------------------------------------------------