//---------------------------------------------------------------------------
#include "MF_CommonFunc.h"
//---------------------------------------------------------------------------
#include <map>
#include <time.h>
#include <vector>
#include <string>
#include <io.h>

#include "MC_Log.h"
#include "common/FUN_Convent.h"
#include "rapidxml/mc_rapidxml.h"
using namespace rapidxml;

#include "md5.hpp"

#include <shlobj.h>
#pragma comment(lib, "shell32.lib")

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


#include <IPHlpApi.h>
#pragma comment(lib, "Iphlpapi.lib")

#include "Psapi.h"
#pragma comment(lib, "Psapi.lib")

#include <AccCtrl.h>
#include <AclAPI.h>

#include <sstream>

#include <WtsApi32.h>	//WTSQueryUserToken
#pragma comment(lib, "WtsApi32.lib")


#include <Userenv.h>	//CreateEnvironmentBlock
#pragma comment(lib, "Userenv.lib")
//---------------------------------------------------------------------------
std::string MF_CommonFunc::_GetCurrentTime(std::string vTimePos)
{
	time_t tSystemTime	= time(NULL);
	struct tm tTime;
	char tTimeBuf[128];

	if(0 != localtime_s(&tTime, &tSystemTime))
	{
		return "";
	}
	sprintf_s(tTimeBuf, sizeof(tTimeBuf), "%02d%s%02d%s%02d", 
		tTime.tm_hour, vTimePos.c_str(), tTime.tm_min, vTimePos.c_str(), tTime.tm_sec);

	return std::string(tTimeBuf);
}
//---------------------------------------------------------------------------
std::string MF_CommonFunc::GetCurrentDate(std::string vDatePos)
{
	time_t tSystemTime	= time(NULL);
	struct tm tTime;
	char tTimeBuf[128];

	if(0 != localtime_s(&tTime, &tSystemTime))
	{
		return "";
	}
	sprintf_s(tTimeBuf, sizeof(tTimeBuf), "%04d%s%02d%s%02d", 
		tTime.tm_year+1900, vDatePos.c_str(), tTime.tm_mon+1, vDatePos.c_str(), tTime.tm_mday);

	return std::string(tTimeBuf);
}
//---------------------------------------------------------------------------
std::string MF_CommonFunc::GetCurrentDateTime(std::string vDatePos, std::string vDtPos, std::string vTimePos)
{
	time_t tSystemTime	= time(NULL);
	struct tm tTime;
	char tTimeBuf[128];

	if(0 != localtime_s(&tTime, &tSystemTime))
	{
		return "";
	}
	sprintf_s(tTimeBuf, sizeof(tTimeBuf), "%04d%s%02d%s%02d%s%02d%s%02d%s%02d", 
		tTime.tm_year+1900, vDatePos.c_str(), tTime.tm_mon+1, vDatePos.c_str(), tTime.tm_mday, 
		vDtPos.c_str(), tTime.tm_hour, vTimePos.c_str(), tTime.tm_min, vTimePos.c_str(), tTime.tm_sec);

	return std::string(tTimeBuf);
}
//---------------------------------------------------------------------------
std::string MF_CommonFunc::GetCurrentTimeWithWeek()
{
	std::map<int, std::string> tMap;
	tMap[0]	= "��";
	tMap[1] = "һ";
	tMap[2] = "��";
	tMap[3] = "��";
	tMap[4] = "��";
	tMap[5] = "��";
	tMap[6] = "��";

	SYSTEMTIME tTime;
	memset(&tTime, 0, sizeof(tTime));
	char tTimeBuf[128] = {0};

	GetLocalTime(&tTime);
	sprintf_s(tTimeBuf, sizeof(tTimeBuf), "%04u-%02u-%02u_%02u:%02u:%02u, ����%s", 
		tTime.wYear, tTime.wMonth, tTime.wDay, tTime.wHour, tTime.wMinute, tTime.wSecond, tMap[tTime.wDayOfWeek].c_str());

	return std::string(tTimeBuf);
}
//---------------------------------------------------------------------------
__int64 MF_CommonFunc::GetTimestamp(std::string vTimes)
{
	time_t tTimestamp;
	if("" == vTimes)
		time(&tTimestamp);
	else
	{
		char* tTimes = KKString(vTimes);
		tm tTm;
		int tYear	= 0;
		int tMonth	= 0;
		int tDay	= 0;
		int tHour	= 0;
		int tMinute = 0;
		int tSecond = 0;
		sscanf(tTimes, "%d-%d-%d %d:%d:%d", &tYear, &tMonth, &tDay, &tHour, &tMinute, &tSecond);

		tTm.tm_year	= tYear - 1900; // �꣬����tm�ṹ��洢���Ǵ�1900�꿪ʼ��ʱ�䣬����tm_yearΪint��ʱ������ȥ1900��
		tTm.tm_mon	= tMonth - 1;   // �£�����tm�ṹ����·ݴ洢��ΧΪ0-11������tm_monΪint��ʱ������ȥ1��
		tTm.tm_mday	= tDay;         // �ա�
		tTm.tm_hour	= tHour;        // ʱ��
		tTm.tm_min	= tMinute;      // �֡�
		tTm.tm_sec	= tSecond;      // �롣
		tTm.tm_isdst= 0;			// ������ʱ��
		tTimestamp = mktime(&tTm);   // ��tm�ṹ��ת����time_t��ʽ��
	}

	return tTimestamp;
}
//---------------------------------------------------------------------------
std::string MF_CommonFunc::FmtTimestamp(__int64 vTimestamp)
{
	__int64 tTimestamp = vTimestamp;
	if(-1 == vTimestamp)
		tTimestamp = GetTimestamp();

	tm *pTm = new tm;
	localtime_s(pTm, &tTimestamp);

	char tTime[32] = {0};
	strftime(tTime, sizeof(tTime), "%Y-%m-%d %H:%M:%S", pTm);

	return tTime;

}
//---------------------------------------------------------------------------
std::string MF_CommonFunc::FmtDateTime(const std::string& vDateTime)
{
	std::string tRetString = vDateTime;
	tRetString = STRReplace(tRetString, " ", "");
	tRetString = STRReplace(tRetString, ":", "");
	tRetString = STRReplace(tRetString, "-", "");
	tRetString = STRReplace(tRetString, "_", "");

	return tRetString;
}
//---------------------------------------------------------------------------
std::string MF_CommonFunc::GetNowTime()
{
	char tBuf[16] = "\0";
	time_t t = time(NULL);
	struct tm *m = localtime(&t);
	strftime(tBuf, 16, "%H:%M:%S", m);

	return std::string(tBuf);
}
//---------------------------------------------------------------------------
std::string MF_CommonFunc::GetLastWeek()
{
	char tBuf[16] = "\0";
	time_t t = time(NULL) - 24*3600*7;
	struct tm *m = localtime(&t);
	strftime(tBuf, 16, "%Y-%m-%d", m);
	
	return std::string(tBuf);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
std::string MF_CommonFunc::STRReplace(const std::string& vString, const std::string& vSubStrFrom, const std::string& vSubStrTo)
{
	std::string tString = vString;
	std::string::size_type tPos = 0;
	std::string::size_type tFormLen = vSubStrFrom.size();
	std::string::size_type tToLen = vSubStrTo.size();

	while( (tPos=tString.find(vSubStrFrom, tPos)) != std::string::npos)
	{
		tString.replace( tPos, tFormLen, vSubStrTo);
		tPos += tToLen;
	}

	return tString;
}
//---------------------------------------------------------------------------
std::string MF_CommonFunc::ReplaceBackslashBySlash(std::string vStr)
{
	std::string::size_type tPos=0;
	tPos = vStr.find("\\", tPos);
	while(std::string::npos != tPos)
	{
		vStr.replace(tPos, 1, "/");
		tPos = vStr.find("\\", tPos+1);
	}
	return vStr;
}
//---------------------------------------------------------------------------
std::string MF_CommonFunc::Trim(std::string& vStr) 
{
	if (vStr.empty()) 
	{
		return vStr;
	}

	//ȥ���ո�
	vStr.erase(0, vStr.find_first_not_of(" "));
	vStr.erase(vStr.find_last_not_of(" ") + 1);
	return vStr;
}
//---------------------------------------------------------------------------
void MF_CommonFunc::SeparateString(char* vStr, const char* vSeps, vector<std::string>& vRet)
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
void MF_CommonFunc::SeparateWstring(const std::wstring vWstr,const std::wstring WSeps,vector<std::wstring>& vRet)
{
	wchar_t* tWstr = new wchar_t[vWstr.length()+1];
	memcpy(tWstr, vWstr.c_str(), (vWstr.length()+1) * sizeof(wchar_t));

	wchar_t* tNextToken	=NULL;
	wchar_t* tToken		=NULL;

	tToken = wcstok_s(tWstr, WSeps.c_str(), &tNextToken);
	while(NULL != tToken)
	{
		vRet.push_back(std::wstring(tToken));
		tToken = wcstok_s(NULL, WSeps.c_str(), &tNextToken);
	}
	delete [] tWstr;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::SeparateFile(const char* vPath, char* vDir, char* vFile)
{
	strcpy_s(vFile, MAX_PATH, vPath);
	char* tPos = strrchr(vFile, '/');

	if(0 == tPos)
		return false;

	*tPos = 0;
	strcpy_s(vDir, MAX_PATH, vFile);
	strcpy_s(vFile, MAX_PATH, tPos+1);
	return true;
}
//---------------------------------------------------------------------------
std::string MF_CommonFunc::STRCombine(char* vFormat,...)
{
	if(0 == vFormat)			return "";
	if(0 == *vFormat)			return "";

	const int tMaxLen = 4096;
	char tStr[tMaxLen];
	memset(tStr, 0, tMaxLen);

	va_list tArg;
	va_start(tArg, vFormat);

	try
	{
		_vsnprintf(tStr, tMaxLen-1, vFormat, tArg);
	}
	catch(...)
	{
		return "";
	}

	return tStr;

}
//---------------------------------------------------------------------------
void MF_CommonFunc::ChWConvertEndian(void * ptr, size_t size)
{
	//std::string tBuf = "12345678";
	//ChWConvertEndian((char*)tBuf.c_str(), tBuf.length());

	char * head = (char *)ptr;
	char * tail = head + size - 1;
	char temp;

	for( ; tail > head; tail--, head++)
	{
		temp = *head;
		*head = *tail;
		*tail = temp;
	}
}
//---------------------------------------------------------------------------
void MF_CommonFunc::ChSeparateFile(const char* vPath, char* vFile, char* vDir)
{
	////�����е�\\ת��Ϊ/
	//char tPath[MAX_PATH] = {0};
	//strcpy_s(tPath, strlen(vPath)+1, vPath);
	//ChStrReplace(tPath, "\\", "/");

	strcpy_s(vFile, MAX_PATH, vPath);	//��תΪchar*������
	char* tPos = strrchr(vFile, '/');
	if(0 != tPos)
	{
		*tPos = 0;
		strcpy_s(vDir,	MAX_PATH, vFile);
		strcpy_s(vFile, MAX_PATH, tPos+1);
	}
}
//---------------------------------------------------------------------------
char* MF_CommonFunc::ChStrReplace(char* vStr, char* vOldStr, char* vNewStr)
{
	char tStr[MAX_PATH];
	memset(tStr, 0, sizeof(tStr));

	for(int i=0; i<strlen(vStr); i++)
	{
		if(0 == strncmp(vStr+i, vOldStr, strlen(vOldStr)))	//����Ŀ���ַ���
		{
			strcat(tStr, vNewStr);
			i += strlen(vOldStr) -1;
		}
		else
		{
			strncat(tStr, vStr + i, 1);	//����һ�ֽڽ�������
		}
	}

	strcpy_s(vStr, sizeof(tStr), tStr);
	return vStr;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
std::string MF_CommonFunc::GetPathSection(std::string vPath)
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
std::string MF_CommonFunc::GetFileSuffix(const std::string& vFileName)
{
	std::string tSuffix = vFileName.substr(vFileName.rfind(".") + 1);

	return tSuffix;
}
//---------------------------------------------------------------------------
std::string MF_CommonFunc::GetFileNameWithoutSuffix(std::string vFileName)
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
__int64 MF_CommonFunc::GetFileSize(const std::string vFilePath)
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
__int64 MF_CommonFunc::GetFolderSize(const std::string vDir)  
{  
	__int64 tSize = 0;  
	std::string	tRootPath	= vDir + "\\";  
	std::string	tRootFile	= tRootPath + "*.*"; 

	WIN32_FIND_DATAA tFindData;  
	HANDLE tFindHandle = FindFirstFileA(tRootFile.c_str(), &tFindData);  
	if (INVALID_HANDLE_VALUE == tFindHandle)  
		return tSize; 

	while (FindNextFileA(tFindHandle, &tFindData))  
	{  
		if (strcmp(tFindData.cFileName, ".") == 0 || strcmp(tFindData.cFileName, "..") == 0 || strcmp(tFindData.cFileName, "WSFileStorage") == 0)  
			continue; 

		std::string strPath = tRootPath + tFindData.cFileName;  
		if (tFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	//���Ŀ¼��Ҫ
		{
			if(tFindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				tSize += GetFolderSize(strPath);  
			else
				continue;
		}

		else  
		{  //FILE_ATTRIBUTE_NORMAL
			HANDLE tFile = CreateFileA(strPath.c_str(), GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);  
			if (INVALID_HANDLE_VALUE == tFile)  
			{
				printf("%s, errno: %d\n", strPath.c_str(), GetLastError());
				continue;  
			}
			LARGE_INTEGER tFileSize;  
			if (::GetFileSizeEx(tFile, &tFileSize))  
			{  
				tSize += tFileSize.QuadPart;  
			}  
			CloseHandle(tFile);  
		}  
	}  
	FindClose(tFindHandle);  
	return tSize;  
} 
//---------------------------------------------------------------------------
std::string MF_CommonFunc::GetFileConten(const char* vFilePath)
{
	if(0 == vFilePath)	return false;
	if(0 == *vFilePath)	return false;

	FILE* tFile;
	errno_t tError = fopen_s(&tFile, vFilePath, "rb");
	if(0 != tError)
	{
		LOG_ERROR("fopen_s fail, file: %s", vFilePath);
		return "";
	}

	int tFileSize = 0;
	int tCurrPos  = 0;
	tCurrPos = ftell(tFile);
	fseek(tFile, 0, SEEK_END);
	tFileSize = ftell(tFile);
	fseek(tFile, tCurrPos, SEEK_SET);
	if(0 == tFileSize)
	{
		LOG_ERROR("FileSize = 0, file: %s", vFilePath);
		fclose(tFile);
		return "";
	}

	char*		tBuffer			= new char[tFileSize + 1];
	memset(tBuffer, 0, tFileSize + 1);
	int			tReadSize		= 0;
	size_t		tReadNum		= 0;
	while(tReadSize < tFileSize)
	{
		tReadNum	=	fread(tBuffer, 1, tFileSize - tReadSize, tFile);
		tReadSize	+=	tReadNum;
	}
	fclose(tFile);

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
int MF_CommonFunc::GetFileSizeFromFilePath(const char* vFilePath)
{
	if(NULL == vFilePath)
		return 0;

	FILE* tFile;
	if(fopen_s(&tFile, vFilePath, "r") == 0)
	{
		int tFileSize = GetFileSizeFromHandle(tFile);
		fclose(tFile);
		return tFileSize;
	}

	return 0;
}
//---------------------------------------------------------------------------
int MF_CommonFunc::GetFileSizeFromHandle(FILE* vFileHandle)
{
	if (NULL == vFileHandle)
		return 0;

	int tFileSize = 0;
	int tCurrPos  = 0;

	tCurrPos = ftell(vFileHandle);
	fseek(vFileHandle, 0, SEEK_END);
	tFileSize = ftell(vFileHandle);
	fseek(vFileHandle, tCurrPos, SEEK_SET);

	return tFileSize;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::LoadFile(const char* vFilePath, char* vBuffer, int vBufLen)
{
	if(NULL == vFilePath)		return false;
	if(NULL == vBuffer)			return false;

	bool tRet = false;
	FILE* tFile;
	if(fopen_s(&tFile, vFilePath, "rb+") == 0)
	{
		unsigned long long tFileSize = GetFileSizeFromHandle(tFile);
		if(vBufLen < tFileSize)
		{
			fclose(tFile);
			return false;
		}

		size_t tRead = 0;
		unsigned long long tReadSize = 0;
		while(tReadSize < tFileSize)
		{
			tRead = fread(vBuffer, 1, tFileSize - tReadSize, tFile);
			tReadSize += tRead;
		}
		fclose(tFile);
	}

	return tRet;
}
//---------------------------------------------------------------------------
std::string MF_CommonFunc::SearchFilePathByFileName(const std::string vSearchDir, const std::string vFileName, const std::string vSuffixName)
{
	if(true == vSearchDir.empty())	return "";
	if(true == vFileName.empty())	return "";


	std::string tSearchDir	= ReplaceBackslashBySlash(vSearchDir);
	if('/' != tSearchDir[tSearchDir.size()-1])
		tSearchDir += "/";

	std::string tStr = tSearchDir;
	"" == vSuffixName ? (tStr += "*.*") : (tStr += "*" + vSuffixName);


	std::string tFilePath	= "";
	char tBuffer[MAX_PATH] = {0};
	WIN32_FIND_DATAA tFindFileData;
	HANDLE thFind = FindFirstFileA(tStr.c_str(), &tFindFileData);
	if(INVALID_HANDLE_VALUE == thFind)
	{
		LOG_ERROR("thFind is invalid, errno: %d", GetLastError());
		return "";
	}

	do
	{
		if(0 == strcmp(tFindFileData.cFileName, ".") || 0 == strcmp(tFindFileData.cFileName, ".."))
		{
			continue;
		}

		else if(tFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			std::string tSubDir = tSearchDir + tFindFileData.cFileName;
			tFilePath = SearchFilePathByFileName(tSubDir, vFileName, vSuffixName);
		}

		else if(vFileName == tFindFileData.cFileName)
		{
			tFilePath = tSearchDir + vFileName;
			break;
		}

	}while(FindNextFileA(thFind, &tFindFileData));

	FindClose(thFind);
	return tFilePath;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::TraversalFilesInDir(const char* vDir, std::vector<std::string>&vFileList, const char* vSuffixName, bool vIsRecursive)
{
	if(vDir == NULL)			return false;
	if(strcmp(vDir, "") == 0)	return false;

	string tDir = STRReplace(vDir, "\\", "/");
	if('/' != tDir[tDir.size()-1])
		tDir += "/";

	//string tStr = tDir;
	//NULL==vSuffixName?(tStr += "*.*"):(tStr += "*." + vSuffixName);	//vDir��һ��Ŀ¼û�ж�Ӧ��׺�ļ�ʱ�����ش��󣬴�ʱֻ����*.*
	string tStr = tDir + "*.*";
	WIN32_FIND_DATAA tFindFileData;
	HANDLE tFindHandle = FindFirstFileA(tStr.c_str(), &tFindFileData);
	if(INVALID_HANDLE_VALUE == tFindHandle)
	{
		LOG_ERROR("INVALID_HANDLE_VALUE, tStr: %s, errno: %d", tStr.c_str(), GetLastError());
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

			std::string tSubDir = tDir + tFindFileData.cFileName;
			TraversalFilesInDir(tSubDir.c_str(), vFileList, vSuffixName, vIsRecursive);
		}

		std::string tFileName = tDir + tFindFileData.cFileName;
		if(NULL != vSuffixName)
		{
			if(vSuffixName != tFileName.substr(tFileName.rfind(".")+1))
				continue;
		}
		vFileList.push_back(tFileName);
	}while(FindNextFileA(tFindHandle, &tFindFileData));

	FindClose(tFindHandle);
	return true;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::IsValidFile(const char* vFilePath)
{
	if(NULL == vFilePath)
		return false;

	FILE* tFile;
	if(fopen_s(&tFile, vFilePath, "rb") == 0)
	{
		char tBuffer[1024];
		fread(tBuffer, 1, 1023, tFile);
		fclose(tFile);

		const char* tp = strstr(tBuffer, "\r\n");	//ȡһ��������֤
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
bool MF_CommonFunc::SHFileDelete(const std::string& vPath, bool vDestory)
{
	if(0 == vPath.size())
		return false;

	//ɾ���ļ���pFrom����������2��\0��β
	std::string tFrom = vPath;
	tFrom.resize(tFrom.size()+1);
	tFrom[tFrom.size()-1] = 0;

	//ɾ���ļ�������վ
	SHFILEOPSTRUCTA tFileOp = {0};
	tFileOp.wFunc	= FO_DELETE;
	tFileOp.pFrom	= tFrom.c_str();
	tFileOp.fFlags	= FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;	//����ʾ���ȣ�������ȷ�ϣ������������Ϣ

	//����������٣���Ž�����վ
	if(false == vDestory)
		tFileOp.fFlags |= FOF_ALLOWUNDO; 

	int tRet = SHFileOperationA(&tFileOp);
	if(ERROR_SUCCESS != tRet)
	{
		LOG_ERROR("SHFileOperationA fail, errno: %d", tRet);
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::WriteFileConten(const char* vFileName, const char* vContent)
{
	if(0 == vFileName)	return false;
	if(0 == *vFileName)	return false;
	if(0 == vContent)	return false;
	if(0 == *vContent)	return false;

	FILE* tFp;
	//if(0 != fopen_s(&tFp, vFileName, "wb++"))
	if(0 != fopen_s(&tFp, vFileName, "a+"))
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
bool MF_CommonFunc::MakeEmptyDir(const char* vPath)
{
	if(0 == vPath)		return false;
	if('\0' == *vPath)	return false;

	if(false != DirExist(vPath))	//������������գ�ֻ�п�Ŀ¼������Ϊ���ص�
	{
		if(false == DeleteDir(vPath))
		{
			LOG_ERROR("DeleteDir %s fail", vPath);
			return false;
		}
	}

	if(false == MakeDir(vPath))
	{
		LOG_ERROR("MakeDir: %s fail", vPath);
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::MakeDir(const char* vPath)
{
	if(0 == vPath)		return false;
	if('\0' == *vPath)	return false;

	int tRet = SHCreateDirectoryExA(NULL, vPath, NULL);
	if(ERROR_ALREADY_EXISTS == tRet || ERROR_SUCCESS == tRet)
		return true;

	LOG_ERROR("Make Dir: %s fail, errno: %d", vPath, tRet);
	return false;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::DeleteDir(const char* vPath)
{
	if(0 == vPath)		return false;
	if('\0' == *vPath)	return false;


	if(false == DirExist(vPath))
		return true;

	char tBuffer[MAX_PATH] = {0};
	sprintf_s(tBuffer, sizeof(tBuffer), "%s", vPath);
	//memset(tBuffer+strlen(tBuffer), 0, MAX_PATH-strlen(tBuffer));
	memset(tBuffer+strlen(tBuffer), 0, 2);

	SHFILEOPSTRUCTA tFileOp;
	tFileOp.fFlags				= FOF_NOCONFIRMATION | FOF_SILENT;
	tFileOp.hNameMappings		= NULL;
	tFileOp.hwnd				= NULL;
	tFileOp.lpszProgressTitle	= NULL;
	tFileOp.pFrom				= tBuffer;
	tFileOp.pTo					= NULL;
	tFileOp.wFunc				= FO_DELETE;	//����Ŀ¼һ��ɾ��

	int tRet = SHFileOperationA(&tFileOp);
	if(0 != tRet)
	{
		LOG_ERROR("SHFileOperation of %s fail, errno: %d", vPath, tRet);
		printf("SHFileOperation of %s fail, errno: %d\n", vPath, tRet);
		return false;
	}


	return true;


	/*if(0 == vPath)		return false;
	if('\0' == *vPath)	return false;

	if(false == DirExist(vPath))
	return true;

	if(false == SHFileDelete(vPath))
	return false;

	return true;*/
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::DirExist(const char* vPath)
{
	if(0 == vPath)		return false;
	if('\0' == *vPath)	return false;

	return FALSE != PathIsDirectoryA(vPath);
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::IsEmptyDir(const char* vPath)
{
	if(0 == vPath)		return false;
	if('\0' == *vPath)	return false;

	char tPath[MAX_PATH] = {0};
	sprintf_s(tPath, MAX_PATH, "%s\\*.*", vPath);

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
bool MF_CommonFunc::IsFileExist(std::string vFileName)
{
	return 0 == _access(vFileName.c_str(),0) ? true : false;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::MyDeleteFile(const char* vFileName)
{
	DWORD tFileAttribute = GetFileAttributesA(vFileName);
	if(1 == (tFileAttribute & FILE_SHARE_READ))
	{//ȥ��ֻ������
		SetFileAttributesA(vFileName, tFileAttribute & ~FILE_SHARE_READ);
	}
	if(false ==DeleteFileA(vFileName))
	{
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::GetFileInfo(char* vFilePath, string& vOriginalName)
{
	if(0 == vFilePath)		return false;
	if(0 == *vFilePath)		return false;

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
	DWORD	tInfoSize	= tMyGetFileVersionInfoSizeA(vFilePath, &tHandle);
	if(0 == tInfoSize)
		return false;

	/*MC_KKMemSpace tSpace(tInfoSize);
	if(0 == tSpace.m_Space._dat)
	return false;*/
	BYTE* tData = new BYTE[tInfoSize];

	if(0 < tMyGetFileVersionInfoA(vFilePath, 0, tInfoSize, tData))
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
		* FileDescription	��ȡ�ļ�˵�� 
		* FileVersion		��ȡ�ļ��汾
		* InternalName		��ȡ�ڲ�����  
		* CompanyName		��ȡ��˾����  
		* LegalCopyright	��ȡ��Ȩ  
		* OriginalFilename	��ȡԭʼ�ļ���
		* ProductName		��ȡ��Ʒ����
		* ProductVersion	��ȡ��Ʒ�汾  
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
std::string MF_CommonFunc::GetSignNameOfFile(char* vFilePath)
{
	if(0 == vFilePath)
		return "";

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
PCCERT_CONTEXT MF_CommonFunc::GetFileCertificate(char* vFilePath)
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

	//��ȡ����ǩ��ǩ��������Ϣ�Ĵ�С
	DWORD	tSignerInfoSize = 0;
	if(FALSE == CryptMsgGetParam(tMsg, CMSG_SIGNER_INFO_PARAM, 0, 0, &tSignerInfoSize))
	{
		CryptMsgClose(tMsg);
		CertCloseStore(tStore, CERT_CLOSE_STORE_CHECK_FLAG);
		return 0;
	}

	//��������ǩ����Ϣ�ڴ�
	PCMSG_SIGNER_INFO tSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, tSignerInfoSize);
	if(0 == tSignerInfo)
	{
		CryptMsgClose(tMsg);
		CertCloseStore(tStore, CERT_CLOSE_STORE_CHECK_FLAG);
		return 0;
	}

	//��ȡǩ���ߵ���Ϣ
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
std::string MF_CommonFunc::RunFilePathName(char* vAppend, HMODULE vHModule)
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
std::string MF_CommonFunc::GetModuleName()
{
	char tRunPath[MAX_PATH] = {0};
	if(FALSE == GetModuleFileNameA(NULL, tRunPath, MAX_PATH))	//..../test.exe
		return "";

	char* tPos = strrchr(tRunPath, '\\');	
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
std::string MF_CommonFunc::GetCurrentDirName(HMODULE vHModule)
{
	char tPath[MAX_PATH];
	if(0 == GetModuleFileNameA(vHModule, tPath, MAX_PATH))
		return "";

	char* tPos = strrchr(tPath, '\\');
	if(0 == tPos)
		return "";

	*tPos = 0;
	char tResult[MAX_PATH];
	sprintf_s(tResult, sizeof(tResult), "%s", tPath);

	return tResult;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::GetCmdlineArgs(std::vector<std::string>* vArgs)
{
	if(0 == vArgs)
		return false;

	LPWSTR*		tArgList;
	int			tArgCount;
	std::vector<std::string> tCmdLine;
	tArgList = CommandLineToArgvW(GetCommandLineW(), &tArgCount);
	if(0 == tArgList)
		return false;

	vArgs->clear();
	for(int i = 0; i < tArgCount; i++)
	{
		std::string	tArgv = UnicodeToACP(tArgList[i]);
		vArgs->push_back(tArgv);
	}
	LocalFree(tArgList);

	return true;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::XLib_LoadAppAndWait(char* vCmdLine, int vShowMode, DWORD vTimeOut, DWORD* vExitCode)
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
		printf("%d\n", GetLastError());
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
bool MF_CommonFunc::RunCmdAndWait(char* vCmdLine, int vShowMode, DWORD vTimeOut, DWORD* vExitCode)
{
	if(0 == vCmdLine)	return false;
	if(0 == *vCmdLine)	return false;

	char tCmdPath[MAX_PATH] = {0};
	if(0 == GetEnvironmentVariableA("COMSPEC", tCmdPath, MAX_PATH))
	{
		LOG_ERROR("get the path of cmd.exe fail, errno: %d", GetLastError());
		return false;
	}

	char tCmdLine[MAX_PATH] = {0};
	sprintf_s(tCmdLine, "%s /c  %s", tCmdPath, vCmdLine);

	STARTUPINFOA stStartUpInfo;
	memset(&stStartUpInfo, 0, sizeof(STARTUPINFOA));
	stStartUpInfo.cb			= sizeof(STARTUPINFO);
	stStartUpInfo.dwFlags		= STARTF_USESHOWWINDOW;
	stStartUpInfo.wShowWindow	= vShowMode;

	PROCESS_INFORMATION stProcessInfo;
	if(false == CreateProcessA(0, tCmdLine, 0, 0, 0, NORMAL_PRIORITY_CLASS, 0, 0, &stStartUpInfo, &stProcessInfo))
	{
		printf("%d", GetLastError());
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
bool MF_CommonFunc::DeleteMyselfExe()
{
	char	tExePath[MAX_PATH]	= {0};
	if(0 == GetModuleFileNameA(NULL, tExePath, MAX_PATH))
	{
		LOG_ERROR("GetModuleFileNameA fail, errno: %d", GetLastError());
		return false;
	}

	char	tCmdPath[MAX_PATH]	= {0};
	if(0 == GetEnvironmentVariableA("COMSPEC", tCmdPath, MAX_PATH))
	{
		LOG_ERROR("GetEnvironmentVariableA fail, errno: %d", GetLastError());
		return false;
	}

	//if(0 == GetShortPathNameA(tExePath, tExePath, MAX_PATH))
	//	return false;

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
		LOG_ERROR("CreateProcessA fail, errno: %d", GetLastError());
		return false;
	}

	// heigthen priority of the current process
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);   

	// set file attribute to normal
	SetFileAttributesA(tExePath, FILE_ATTRIBUTE_NORMAL);

	// depress priority of command process, then start it
	SetPriorityClass(pi.hProcess, IDLE_PRIORITY_CLASS);
	ResumeThread(pi.hThread);
	return true;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::DeleteMyselfDir()
{
	//����һ����ʱ��bat������bat�ķ�ʽ������һ����Ŀ¼
	std::string tBatName	= "C:\\temp.bat";
	std::string tExePath	= GetCurrentDirName();

	std::string tNewLine	= "\r\n"; 
	std::string tConten		= "";

	tConten += "cd /" + tNewLine;						//�Ӵ�Դ�ļ�����Ŀ¼��ռ��
	tConten += "rd /q /s " + tExePath + tNewLine;		//ɾ��Դ�ļ�Ŀ¼
	tConten += "del %0";								//ɾ��bat�ļ�

	if(false == WriteFileConten(tBatName.c_str(), tConten.c_str()))
	{
		LOG_ERROR("create temp fail");
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
		LOG_ERROR("CreateProcessA fail, errno: %d", GetLastError());
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
DWORD MF_CommonFunc::GetProcessIdByName(const char* vProcessName)
{
	HANDLE tSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);	//��ȡ����
	if(INVALID_HANDLE_VALUE == tSnapshot)
		return 0;

	//PROCESSENTRY32 tPe = {0};	//
	PROCESSENTRY32 tPe = { sizeof(tPe) };
	BOOL tRet = Process32First(tSnapshot, &tPe);
	if(FALSE == tRet)
	{
		LOG_ERROR("Process32First fail, errno: %d", GetLastError());
		return 0;
	}

	do 
	{
		if(0 != strcmp(UnicodeToACP(tPe.szExeFile).c_str(), vProcessName))
			continue;

		return tPe.th32ProcessID;
		
	} while (Process32Next(tSnapshot, &tPe));

	CloseHandle(tSnapshot);
	return 0;
}
//---------------------------------------------------------------------------
DWORD MF_CommonFunc::GetProcessIdByTcpPort(u_short vLocalPort)
{
	PMIB_TCPTABLE_OWNER_PID tTcpTableInfo(NULL);
	DWORD tSize(0);
	if(ERROR_INSUFFICIENT_BUFFER == GetExtendedTcpTable(tTcpTableInfo, &tSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0))
	{
		tTcpTableInfo = (MIB_TCPTABLE_OWNER_PID *)new char[tSize];//���·��仺����
	}

	if(NO_ERROR != GetExtendedTcpTable(tTcpTableInfo, &tSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) )
	{
		delete tTcpTableInfo;
		return 0;
	}

	int tNum = (int) tTcpTableInfo->dwNumEntries; //TCP���ӵ���Ŀ
	DWORD tPId = 0;
	for(int i=0; i<tNum; i++)
	{
		if(vLocalPort == htons(tTcpTableInfo->table[i].dwLocalPort))	////���ض˿�
		{
			tPId =  tTcpTableInfo->table[i].dwOwningPid;
			break;

			//printf("���ص�ַ��%s:%d  Զ�̵�ַ��%s:%d  ״̬��%d  ����ID:%d\n", 
			//	inet_ntoa(*(in_addr*)& tTcpTableInfo->table[i].dwLocalAddr), //����IP ��ַ
			//	htons(tTcpTableInfo->table[i].dwLocalPort), //���ض˿�
			//	inet_ntoa(*(in_addr*)& tTcpTableInfo->table[i].dwRemoteAddr), //Զ��IP��ַ
			//	htons(tTcpTableInfo->table[i].dwRemotePort), //Զ�̶˿�
			//	tTcpTableInfo->table[i].dwState, //״̬
			//	tTcpTableInfo->table[i].dwOwningPid); //��������PID
		}

	}
	delete tTcpTableInfo;
	return tPId;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::KillProcessByName(const char* vProcessName)
{
	DWORD tProcessId = GetProcessIdByName(vProcessName);
	while(0 != tProcessId)
	{
		HANDLE tProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, tProcessId);
		if(NULL == tProcessHandle)
		{
			return false;
		}

		if(0 == TerminateProcess(tProcessHandle, 0))
		{
			return false;
		}

		Sleep(100);
		tProcessId = GetProcessIdByName(vProcessName);
	}

	return true;
}
//---------------------------------------------------------------------------
std::string MF_CommonFunc::GetProcUserNameByPid(int vProcId)
{
	HANDLE tProcHandle = OpenProcess(PROCESS_QUERY_INFORMATION, 0, vProcId);
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

	//return _UCToACP(tUserName);
	//std::string tTemp = tUserName;
	return tUserName;
}
//---------------------------------------------------------------------------
std::string MF_CommonFunc::GetProcessPathByPid(int vPid)
{
	HANDLE	tProHandle	= OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, vPid);  
	if (NULL == tProHandle)
	{  
		LOG_ERROR("OpenProcess fail, errno: %d", GetLastError());
		return "";  
	}  

	static DWORD tVer=0;	//��̬ ����һ�μ���,��ȡϵͳ�汾  
	if (tVer==0)  
	{  
		OSVERSIONINFO tOs = {sizeof(OSVERSIONINFO)};  
		GetVersionEx(&tOs);  
		tVer = tOs.dwMajorVersion;  
	}  


	TCHAR tProcessName[MAX_PATH] = {0};
	if (tVer<6)//�汾С�� Windows Vista  
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
		{                    //��Ͱ汾 vista  
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/* @param[vPath]:	ע�����
* @param[vValue]:	ע������
* GetRegValue("HKEY_LOCAL_MACHINE\\Software\\GOLDMSG\\WSClient", "register", 0, &tBuff, &tSize))
*/
bool MF_CommonFunc::GetRegValue(const char* vPath, const char* vValue, DWORD *vRegType, char **vData, DWORD *vLen)
{
	std::string tPath = vPath;
	HKEY tKey = RegKeyRoot(tPath);	//eg: HKEY_LOCAL_MACHINE
	if(0 == tKey)
		return false;

	char* tPos = strstr((char*)tPath.c_str(), "\\");	// "\\Software\\GOLDMSG\\WSClient"
	if(0 == tPos)
		return false;

	tPos++;
	HKEY	tSubKey = 0;
	LSTATUS tRet	= 0;
	tRet = RegOpenKeyExA(tKey, tPos, 0, KEY_READ, &tSubKey);
	if(ERROR_SUCCESS != tRet)
	{
		LOG_ERROR("RegOpenKeyExA fail code = %d", tRet);
		return false;
	}

	bool	tResult = true;
	char*	tBuff	= 0;
	DWORD	tType;
	DWORD	tSize	= 0;

	if(ERROR_SUCCESS != RegQueryValueExA(tSubKey, vValue, 0, &tType, 0, &tSize))
	{
		tResult = false;
		goto WORK_END;
	}
	LOG_INFO("tSize = %d", tSize);
	if(0 == vData)
		goto WORK_END;

	tBuff = new char[tSize+1];
	memset(tBuff, 0, tSize+1);
	tRet = RegQueryValueExA(tSubKey, vValue, 0, &tType, (BYTE*)tBuff, &tSize);
	if(ERROR_SUCCESS != tRet)
	{
		LOG_ERROR("RegQueryValueExA() fail code = %d", tRet);
		tResult = false;
		goto WORK_END;
	}

WORK_END:
	if(true == tResult)
	{
		if(0 != vRegType)	*vRegType	= tType;
		if(0 != vLen)		*vLen		= tSize;
		if(0 != vData)		*vData		= tBuff;
	}

	RegCloseKey(tSubKey);
	return tResult;
}
//---------------------------------------------------------------------------
HKEY MF_CommonFunc::RegKeyRoot(const std::string& vPath)
{
	std::string tPath = vPath;
	char* tPos = strstr((char*)tPath.c_str(), "\\");
	if(0 != tPos)
		*tPos = 0;

#define SWITCH_HKEY_FORNAME(_HKEY_) if(0 == stricmp(""#_HKEY_, tPath.c_str())) return _HKEY_;
	SWITCH_HKEY_FORNAME(HKEY_CLASSES_ROOT);
	SWITCH_HKEY_FORNAME(HKEY_CURRENT_CONFIG);
	SWITCH_HKEY_FORNAME(HKEY_CURRENT_USER);
	SWITCH_HKEY_FORNAME(HKEY_LOCAL_MACHINE);
	SWITCH_HKEY_FORNAME(HKEY_USERS);

	return 0;

}
//---------------------------------------------------------------------------
std::string MF_CommonFunc::GetRegValueS(const char* vPath, const char* vValue)
{
	char *tBuff = 0;
	DWORD tSize;
	if(false == GetRegValue(vPath, vValue, 0, &tBuff, &tSize))
		return "";

	std::string tResult = tBuff;

	FreeRegBuff(tBuff);
	return tResult;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void MF_CommonFunc::FreeRegBuff(const char* vData)
{
	if(0!=vData)
	{
		delete[] vData;
		vData = 0;
	}
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::check_disk_wirteable(int vIndex)
{
	if(0 > vIndex || 25 < vIndex)	return false;

	char tPath[512]		= {0};
	char temp[512]		= {0};
	char tDevice[512]	= {0};

	sprintf_s(tDevice, _countof(tDevice), "\\\\.\\%c:", 'A'+vIndex);
	HANDLE tDevHandle = CreateFileA(tDevice, 0, 0, 0, CREATE_ALWAYS, FILE_FLAG_DELETE_ON_CLOSE,0);
	DWORD tLastError = GetLastError();
	if(INVALID_HANDLE_VALUE == tDevHandle && 87 == tLastError /*|| 21 == tLastError)*/)
	{
		LOG_ERROR("��������û�д��̣�����������%c�в������", 'A' + vIndex);
		return false;
	}
	CloseHandle(tDevHandle);

	//OPEN_EXISTING
	sprintf(tPath, "%c:\\goldmsg.txt", 'A' + vIndex);
	HANDLE tHandle = CreateFileA(tPath, 0, 0, 0, CREATE_ALWAYS, FILE_FLAG_DELETE_ON_CLOSE|FILE_ATTRIBUTE_HIDDEN, 0);
	if(tHandle == 0 || INVALID_HANDLE_VALUE == tHandle)
	{
		LOG_ERROR("����%c���ɶ�д", 'A' + vIndex);
		return false;
	}
	CloseHandle(tHandle);

	return true;
}
//---------------------------------------------------------------------------
int MF_CommonFunc::GetDiskInfo()  
{  
	DWORD tDiskCount = 0; 

	//����GetLogicalDrives()�������Ի�ȡϵͳ���߼����������������������ص���һ��32λ�޷����������ݡ�  
	DWORD tDiskInfo = GetLogicalDrives();  

	//ͨ��ѭ�������鿴ÿһλ�����Ƿ�Ϊ1�����Ϊ1�����Ϊ��,���Ϊ0����̲����ڡ�  
	while(tDiskInfo)  
	{  
		//ͨ��λ������߼���������ж��Ƿ�Ϊ1  
		Sleep(10);  
		if(tDiskInfo&1)  
		{  
			tDiskCount++;  
		}  
		tDiskInfo = tDiskInfo >> 1;//ͨ��λ��������Ʋ�����֤ÿѭ��һ��������λ�������ƶ�һλ��
	}  

	printf("disk count: %d\n", tDiskCount);
	//vCount = tDiskCount;
	//----------------------------------------------------------------------------//  
	//ͨ��GetLogicalDriveStrings()������ȡ�����������ַ�����Ϣ����  
	int tDSLength = GetLogicalDriveStrings(0, NULL);  

	char* tDStr = new char[tDSLength];  
	memset(tDStr, 0, tDSLength);  

	//ͨ��GetLogicalDriveStrings���ַ�����Ϣ���Ƶ�����������,���б�������������������Ϣ��  
	GetLogicalDriveStringsA(tDSLength, tDStr);  

	int tType;  
	int si=0;  
	/*BOOL tResult;  
	unsigned _int64 tI64FreeBytesToCaller;  
	unsigned _int64 tI64TotalBytes;  
	unsigned _int64 tI64FreeBytes;  */

	//��ȡ����������Ϣ������tDStr�ڲ����ݸ�ʽ��A:\NULLB:\NULLC:\NULL������tDSLength/4���Ի�þ����ѭ����Χ  
	for(int i=0; i<tDSLength/4; ++i)  
	{  
		Sleep(10);  
		char* tStrDriver = tDStr+i*4;  
		tType = GetDriveTypeA(tStrDriver);//GetDriveType���������Ի�ȡ���������ͣ�����Ϊ�������ĸ�Ŀ¼  
		switch (tType)  
		{  
		case DRIVE_FIXED:  
			{  
				printf("���ش���%s\n", tStrDriver);  
			}  
			break;  
		case DRIVE_CDROM:  
			{  
				printf("DVD������%s\n", tStrDriver);  
			}  
			break;  
		case DRIVE_REMOVABLE:  
			{  
				printf("���ƶ�����%s\n", tStrDriver);  
			}  
			break;  
		case DRIVE_REMOTE:  
			{  
				printf("�������%s\n", tStrDriver);  
			}  
			break;  
		case DRIVE_RAMDISK:  
			{  
				printf("����RAM����%s\n", tStrDriver);  
			}  
		case DRIVE_NO_ROOT_DIR:
			{
				printf("�޸�����\n");
			}
			break;  
		case DRIVE_UNKNOWN:  
			{  
				printf("δ֪�豸\n");  
			}  
			break;  
		default:  
			printf("δ֪�豸\n");  
			break;  
		}  

		////GetDiskFreeSpaceEx���������Ի�ȡ���������̵Ŀռ�״̬,�������ص��Ǹ�BOOL��������  
		//tResult = GetDiskFreeSpaceEx (tStrDriver, (PULARGE_INTEGER)&tI64FreeBytesToCaller, (PULARGE_INTEGER)&tI64TotalBytes, (PULARGE_INTEGER)&tI64FreeBytes);  
		//if(tResult)  
		//{  
		//	printf("����������: %s\n",		GetSizeFormat(tI64TotalBytes).c_str());  
		//	printf("����ʣ��ռ�: %s\n",	GetSizeFormat(tI64FreeBytes).c_str());

		//	//���ʣ������
		//	if(tI64FreeBytes < vFreeBytes)
		//		printf("warnning: %s ʣ����������!!!\n", tStrDriver);
		//}  
		//else  
		//{  
		//	printf("GetDiskFreeSpaceEx() failed, ������: %d\n", GetLastError()); 
		//}  
		//si+=4;  

		printf("\n");
	}  

	return tDiskCount;
} 
//---------------------------------------------------------------------------
bool MF_CommonFunc::GetDiskSpace(std::string vName, __int64& vAllKB, __int64& vUsedKB, __int64& vFreeKB )
{
	LPSTR tDriverName = (LPSTR)vName.c_str();
	ULARGE_INTEGER tFreeBytesAvailable, tTotalBytes, tTotalFreeBytes;
	if( !GetDiskFreeSpaceExA( tDriverName, &tFreeBytesAvailable, &tTotalBytes, &tTotalFreeBytes))
	{
		return false;
	}
	vAllKB    = tTotalBytes.QuadPart / 1024;
	vFreeKB   = tTotalFreeBytes.QuadPart / 1024;
	vUsedKB   = vAllKB - vFreeKB;

	return true;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::GetMountPointsList(std::vector<std::string>& vMountPointList, std::string vWildcard)
{
	vMountPointList.clear();

	char	tVolumeName[MAX_PATH]		= {0};	//������ ����
	HANDLE	tVolNameHandle	= FindFirstVolumeA(tVolumeName, MAX_PATH);	//���������ϵ�һ����������������������
	if(INVALID_HANDLE_VALUE == tVolNameHandle)
	{
		LOG_ERROR("tVolNameHandle invalid, errno: %d.", GetLastError());
		return false;
	}

	do 
	{
		if(false == ListMountPointOfVolume(tVolumeName, vMountPointList, vWildcard))
		{
			LOG_ERROR("ListMountPointOfVolume fail.");
			FindVolumeClose(tVolNameHandle);
			return false;
		}

	} while (FindNextVolumeA(tVolNameHandle, tVolumeName, MAX_PATH));

	FindVolumeClose(tVolNameHandle);
	return true;

}
//---------------------------------------------------------------------------
bool MF_CommonFunc::ListMountPointOfVolume(const char* vVolumeName, std::vector<std::string>& vMountPointList, std::string vWildcard)
{
	if(0	== vVolumeName)		return false;
	if('\0'	== *vVolumeName)	return false;

	char	tDriverString[MAX_PATH]	= {0};	//������ �̷���(C:\\)
	DWORD	tReturnLen				= 0;
	if(FALSE == GetVolumePathNamesForVolumeNameA(vVolumeName, tDriverString, MAX_PATH, &tReturnLen))
	{
		LOG_ERROR("GetVolumePathNamesForVolumeNameA fail, errno: %d", GetLastError());
		return false;
	}

	char	tVolumeMountPoint[MAX_PATH] = {0};	//������ ���ص�
	HANDLE	tVolMountPointHandle	= FindFirstVolumeMountPointA(vVolumeName, tVolumeMountPoint, MAX_PATH);	//��ȡ��������һ�����ص�
	if(INVALID_HANDLE_VALUE == tVolMountPointHandle)
	{
		//LOG_WARN("tVolMountPointHandle invalid, errno: %d", GetLastError());
		return true;
	}

	do 
	{
		std::string tMountPoint = std::string(tDriverString) + std::string(tVolumeMountPoint);
		if(std::string::npos == tMountPoint.find("disk") || std::string::npos == tMountPoint.find(vWildcard))
		{
			continue;
		}

		vMountPointList.push_back(tMountPoint);

	} while (FindNextVolumeMountPointA(tVolMountPointHandle, tVolumeMountPoint, MAX_PATH));


	FindVolumeMountPointClose(tVolMountPointHandle);
	return true;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::DeleteMountPoint(const char* vMountPoint)
{
	if(0	== vMountPoint)		return false;
	if('\0' == *vMountPoint)	return false;

	if(0 == DeleteVolumeMountPointA(vMountPoint))
	{
		LOG_ERROR("DeleteVolumeMountPointA %s fail, errno: %d", vMountPoint, GetLastError());
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::SetMountPoint(const char* vOldMountPoint, const char* vNewMountPoint)
{
	if(0	== vOldMountPoint)		return false;
	if('\0' == *vOldMountPoint)		return false;
	if(0	== vNewMountPoint)		return false;
	if('\0' == *vNewMountPoint)		return false;

	char tVolumeName[MAX_PATH]= {0};
	if(0 == GetVolumeNameForVolumeMountPointA(vOldMountPoint, tVolumeName, MAX_PATH))	//��ȡ����������
	{
		LOG_ERROR("Get Volume Name For Volume Mount Point: %s fail, errno: %d", vOldMountPoint, GetLastError());
		return false;
	}

	if(0 == SetVolumeMountPointA(vNewMountPoint, tVolumeName))
	{
		LOG_ERROR("SetVolumeMountPoint: %s fail, errno: %d", vNewMountPoint, GetLastError());
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool MF_CommonFunc::GetDriverString(const char* vVolumeName)
{
	if(0	== vVolumeName)		return false;
	if('\0'	== *vVolumeName)	return false;

	char	tDriverString[MAX_PATH]	= {0};	//������ �̷�
	DWORD	tReturnLen				= 0;

	if(FALSE == GetVolumePathNamesForVolumeNameA(vVolumeName, tDriverString, MAX_PATH, &tReturnLen))
	{
		LOG_ERROR("GetVolumePathNamesForVolumeNameA fail, errno: %d", GetLastError());
		return false;
	}

	printf("tDriverString: %s\n", tDriverString);

	return true;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::IsWin7OS()
{
	OSVERSIONINFOEX  tVersion;
	tVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX );
	if(0 == GetVersionEx((OSVERSIONINFO *)&tVersion))
	{
		LOG_ERROR("GetVersionEx error :  %d", GetLastError());
		return false;
	}

	LOG_INFO("dwMajorVersion: %d, dwMinorVersion: %d", tVersion.dwMajorVersion, tVersion.dwMinorVersion);
	if(tVersion.dwMajorVersion == 6 && tVersion.dwMinorVersion == 1)
	{
		if(tVersion.wProductType == 1)
		{
			LOG_INFO("the os is Win7");
			return true;
		}

		else
		{
			LOG_INFO("the os is server 2008");
			return false;
		}
	}

	return false;
}
//---------------------------------------------------------------------------
//�鿴ע����֪��HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion	sCurrentVersion  
//XPϵͳ  5.1ΪXP��6.0Ϊvista��2008, 6.1Ϊwin7, 6.2 Win8, 6.3 Win10  
// �Ƿ�Win10ϵͳ  
bool MF_CommonFunc::CheckSystemVersion()  
{  
	// GetVersion() �����ж�Win10ϵͳ��̫���� 

	//��ע���  
	HKEY	tKey;  
	LSTATUS	tRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0,  KEY_ALL_ACCESS,  &tKey);  
	if(tRet != ERROR_SUCCESS)  
	{
		LOG_ERROR("RegOpenKeyEx fail, error = %d", GetLastError());
		return false;  
	}

	//��ȡCurrentVersion
	char tCurrentVersion[100] = {0};  
	DWORD tSize = 100;  
	DWORD tType;  

	tRet = RegQueryValueExA(tKey, "CurrentVersion", NULL, &tType, (BYTE*)tCurrentVersion, &tSize);  
	if(tRet != ERROR_SUCCESS)  
	{
		LOG_ERROR("RegQueryValueEx fail, error = %d", GetLastError());
		return false;  
	}

	if		(0 == strncmp(tCurrentVersion, "5.1", 3))		printf("The OS is XP\n");
	else if	(0 == strncmp(tCurrentVersion, "6.0", 3))		printf("The OS is vista and 2008\n");
	else if	(0 == strncmp(tCurrentVersion, "6.1", 3))		printf("The OS is Win7\n");
	else if	(0 == strncmp(tCurrentVersion, "6.2", 3))		printf("The OS is Win8\n");
	else if	(0 == strncmp(tCurrentVersion, "6.3", 3))		printf("The OS is Win10\n");
	else													printf("Unknow OS\n");

	return true;
}  
//---------------------------------------------------------------------------
/*
<?xml version="1.0" encoding="UTF-8"?>   
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">   
<compatibility xmlns="urn:schemas-microsoft-com:compatibility.v1">   
<application>   
<!-- Windows 10 -->   
<supportedOS Id="{8e0f7a12-bfb3-4fe8-b9a5-48fd50a15a9a}"/>  
<!-- Windows 8.1 -->  
<supportedOS Id="{1f676c76-80e1-4239-95bb-83d0f6d0da78}"/>  
<!-- Windows Vista -->  
<supportedOS Id="{e2011457-1546-43c5-a5fe-008deee3d3f0}"/>   
<!-- Windows 7 -->  
<supportedOS Id="{35138b9a-5d96-4fbd-8e2d-a2440225f93a}"/>  
<!-- Windows 8 -->  
<supportedOS Id="{4a2f28e3-53b9-4441-ba9c-d69d4a4a6e38}"/>  
</application>   
</compatibility>  
</assembly>   
*/
//��������->�嵥����->��������-�����嵥�ļ� ��дcompatibility.xml
std::string MF_CommonFunc::GetSystemName()  
{  
	SYSTEM_INFO info;        //��SYSTEM_INFO�ṹ�ж�64λAMD������   
	GetSystemInfo(&info);    //����GetSystemInfo�������ṹ   
	OSVERSIONINFOEX os;   
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);     

	string osname = "unknown OperatingSystem.";  

	if(GetVersionEx((OSVERSIONINFO *)&os))  
	{   
		//������ݰ汾��Ϣ�жϲ���ϵͳ����   
		switch(os.dwMajorVersion)//�ж����汾��  
		{  
		case 4:  
			switch(os.dwMinorVersion)//�жϴΰ汾��   
			{   
			case 0:  
				if(os.dwPlatformId==VER_PLATFORM_WIN32_NT)  
					osname = "Microsoft Windows NT 4.0"; //1996��7�·���   
				else if(os.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)  
					osname = "Microsoft Windows 95";  
				break;  
			case 10:  
				osname = "Microsoft Windows 98";  
				break;  
			case 90:  
				osname = "Microsoft Windows Me";  
				break;  
			}  
			break;  

		case 5:  
			switch(os.dwMinorVersion)   //�ٱȽ�dwMinorVersion��ֵ  
			{   
			case 0:  
				osname = "Microsoft Windows 2000";//1999��12�·���  
				break;  

			case 1:  
				osname = "Microsoft Windows XP";//2001��8�·���  
				break;  

			case 2:  
				if(os.wProductType==VER_NT_WORKSTATION   
					&& info.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)  
				{  
					osname = "Microsoft Windows XP Professional x64 Edition";  
				}  
				else if(GetSystemMetrics(SM_SERVERR2)==0)  
					osname = "Microsoft Windows Server 2003";//2003��3�·���   
				else if(GetSystemMetrics(SM_SERVERR2)!=0)  
					osname = "Microsoft Windows Server 2003 R2";  
				break;  
			}  
			break;  

		case 6:  
			switch(os.dwMinorVersion)  
			{  
			case 0:  
				if(os.wProductType == VER_NT_WORKSTATION)  
					osname = "Microsoft Windows Vista";  
				else  
					osname = "Microsoft Windows Server 2008";//�������汾   
				break;  
			case 1:  
				if(os.wProductType == VER_NT_WORKSTATION)  
					osname = "Microsoft Windows 7";  
				else  
					osname = "Microsoft Windows Server 2008 R2";  
				break;  
			case 2:  
				if(os.wProductType == VER_NT_WORKSTATION)  
					osname = "Microsoft Windows 8";  
				else  
					osname = "Microsoft Windows Server 2012";  
				break;  
			case 3:  
				if(os.wProductType == VER_NT_WORKSTATION)  
					osname = "Microsoft Windows 8.1";  
				else  
					osname = "Microsoft Windows Server 2012 R2";  
				break;  
			}  
			break;  

		case 10:  
			switch(os.dwMinorVersion)  
			{  
			case 0:  
				if(os.wProductType == VER_NT_WORKSTATION)  
					osname = "Microsoft Windows 10";  
				else  
					osname = "Microsoft Windows Server 2016 Technical Preview";//�������汾   
				break;  
			}  
			break;  
		}  
	}//if(GetVersionEx((OSVERSIONINFO *)&os))  
	//https://msdn.microsoft.com/en-us/library/ms724832.aspx  
	return osname;  
}
//---------------------------------------------------------------------------
BOOL MF_CommonFunc::Is64System(void)
{
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;
	BOOL bIsWow64 = FALSE;
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress( GetModuleHandleA("kernel32"),"IsWow64Process");
	if (NULL != fnIsWow64Process)
	{
		fnIsWow64Process(GetCurrentProcess(),&bIsWow64);
	}
	return bIsWow64;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::pc_power_off(bool vRestart)
{
	HANDLE	tToken;
	LUID  	tLuid;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &tToken);
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tLuid);

	TOKEN_PRIVILEGES tPri;
	tPri.PrivilegeCount           = 1;
	tPri.Privileges[0].Luid       = tLuid;
	tPri.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(tToken, false, &tPri, sizeof(TOKEN_PRIVILEGES), 0, 0);

	if(true == vRestart)
	{
		ExitWindowsEx(EWX_REBOOT|EWX_FORCE, 0);
	}
	else
	{
		ExitWindowsEx(EWX_SHUTDOWN|EWX_FORCE, 0);
	}
	return true;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::GetScreenResolution(int& vLateral, int& vVertical)
{
	vLateral	= GetSystemMetrics(SM_CXSCREEN); 
	vVertical	= GetSystemMetrics(SM_CYSCREEN); 
	if(0 == vLateral || 0 == vVertical)
		return false;

	////������������������
	//vLateral	= GetSystemMetrics(SM_CXFULLSCREEN);
	//vVertical	= GetSystemMetrics(SM_CYFULLSCREEN);
	//if(0 == vLateral || vVertical == vVertical)
	//	return false;

	////�ڶ���
	//DEVMODE tDevMode = {0};
	//if(FALSE == EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &tDevMode))
	//	return false;

	//vLateral	= tDevMode.dmPelsWidth;
	//vVertical	= tDevMode.dmPelsHeight;

	return true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool MF_CommonFunc::IsServiceInstalled(std::string vServiceName)
{
	if(true == vServiceName.empty())
		return false;

	SC_HANDLE tSCHandle = ::OpenSCManager(NULL,  NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == tSCHandle)
	{
		LOG_ERROR("connect to service manager fail! GetLastError: %d", GetLastError());
		return false;
	}

	SC_HANDLE tSvcHandle = ::OpenServiceA(tSCHandle, vServiceName.c_str(), SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if(NULL == tSvcHandle)
	{
		//gLog.Log("open service fail. GetLastError: %d", GetLastError());
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	::CloseServiceHandle(tSvcHandle);
	::CloseServiceHandle(tSCHandle);
	return true;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::IsServiceRunning(std::string vServiceName)
{
	if(true == vServiceName.empty())
		return false;

	SC_HANDLE tSCHandle = ::OpenSCManager(NULL,  NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == tSCHandle)
	{
		LOG_ERROR("connect to service manager fail! GetLastError: %d", GetLastError());
		return false;
	}

	SC_HANDLE tSvcHandle = ::OpenServiceA(tSCHandle, vServiceName.c_str(), SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if(NULL == tSvcHandle)
	{
		//LOG_ERROR("open service fail!! GetlastError: %d", GetLastError());
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	SERVICE_STATUS tStatus;
	if(FALSE == ::QueryServiceStatus(tSvcHandle, &tStatus))
	{
		LOG_ERROR("query service status fail(1)! GetLastError: %d", GetLastError());
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
bool MF_CommonFunc::InstallService(std::string vServiceName, std::string vDisplayName, std::string vServicePath)
{
	SC_HANDLE tSCHandle = ::OpenSCManager(NULL,  NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == tSCHandle)
	{
		LOG_ERROR("connect to service manager fail! GetLastError: %d", GetLastError());
		return false;
	}

	//SERVICE_ERROR_IGNORE SERVICE_ERROR_NORMAL
	SC_HANDLE tSvcHandle = CreateServiceA(tSCHandle, vServiceName.c_str(), vDisplayName.c_str(), SERVICE_ALL_ACCESS ,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START,SERVICE_ERROR_NORMAL,
		vServicePath.c_str(),
		NULL,NULL,NULL,NULL,NULL);

	if(NULL == tSvcHandle)
	{
		LOG_ERROR("install service fail. name: %s, path: %s, GtLastError: ", vServiceName.c_str(), vServicePath.c_str(), GetLastError());
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	::CloseServiceHandle(tSvcHandle);
	::CloseServiceHandle(tSCHandle);

	return true;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::UninstallService(std::string vServiceName)
{
	if(true == vServiceName.empty())
		return false;

	SC_HANDLE tSCHandle = ::OpenSCManager(NULL,  NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == tSCHandle)
	{
		LOG_ERROR("connect to service manager fail! GetLastError: %d", GetLastError());
		return false;
	}

	SC_HANDLE tSvcHandle = ::OpenServiceA(tSCHandle, vServiceName.c_str(), SERVICE_ALL_ACCESS);
	if(NULL == tSvcHandle)
	{
		LOG_ERROR("open service fail. GetlastError: %d", GetLastError());
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	SERVICE_STATUS tStatus;
	if(FALSE == ::QueryServiceStatus(tSvcHandle, &tStatus))
	{
		LOG_ERROR("query service status fail(1)! GetLastError: %d", GetLastError());
		::CloseServiceHandle(tSvcHandle);
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	if(tStatus.dwCurrentState != SERVICE_STOPPED)
	{
		if(FALSE == ::ControlService(tSvcHandle, SERVICE_CONTROL_STOP, &tStatus))
		{
			LOG_ERROR("contorl service with stop fail. GetLastError: %d", GetLastError());
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
		LOG_ERROR("delete service fail, getlasterror: %d", GetLastError());
		::CloseServiceHandle(tSvcHandle);
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	::CloseServiceHandle(tSvcHandle);
	::CloseServiceHandle(tSCHandle);

	return true;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::MyStartService(std::string vServiceName)
{
	if(true == vServiceName.empty())
		return false;

	SC_HANDLE tSCHandle = ::OpenSCManager(NULL,  NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == tSCHandle)
	{
		LOG_ERROR("connect to service manager fail(1)! GetLastError: %d", GetLastError());
		return false;
	}

	SC_HANDLE tSvcHandle = ::OpenServiceA(tSCHandle, vServiceName.c_str(), SERVICE_ALL_ACCESS|DELETE);
	if(NULL == tSvcHandle)
	{
		LOG_ERROR("open service fail. GetLastError: %d", GetLastError());
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	SERVICE_STATUS tStatus;
	if(FALSE == ::QueryServiceStatus(tSvcHandle, &tStatus))
	{
		LOG_ERROR("query service status fail(1)! GetLastError: %d", GetLastError());
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
		LOG_ERROR("start service fail!! GetLastError: %d", GetLastError());
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
bool MF_CommonFunc::MyStopService(std::string vServiceName)
{
	if(true == vServiceName.empty())
	{
		LOG_ERROR("stop service, service name is empty");
		return false;
	}

	SC_HANDLE tSCHandle = ::OpenSCManager(NULL,  NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == tSCHandle)
	{
		LOG_ERROR("connect to service manager fail(2)! GetLastError: %d", GetLastError());
		return false;
	}

	SC_HANDLE tSvcHandle = ::OpenServiceA(tSCHandle, vServiceName.c_str(), SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if(NULL == tSvcHandle)
	{
		LOG_ERROR("open service fail(2)!! GetlastError: %d", GetLastError());
		::CloseServiceHandle(tSCHandle);
		return false;
	}

	SERVICE_STATUS tStatus;
	if(FALSE == ::QueryServiceStatus(tSvcHandle, &tStatus))
	{
		LOG_ERROR("query service status fail(2)! GetLastError: %d", GetLastError());
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
		LOG_ERROR("contorl service with stop fail. GetLastError: %d", GetLastError());
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
std::string MF_CommonFunc::GetServicePath(std::string vServiceName)
{
	if(true == vServiceName.empty())
	{
		LOG_ERROR("stop service, service name is empty");
		return false;
	}

	SC_HANDLE tSCHandle = OpenSCManager(NULL,  NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == tSCHandle)
	{
		LOG_ERROR("connect to service manager fail! GetLastError: %d", GetLastError());
		return false;
	}

	SC_HANDLE tSvcHandle = OpenServiceA(tSCHandle, vServiceName.c_str(), SERVICE_ALL_ACCESS);
	if(NULL == tSvcHandle)
	{
		LOG_ERROR("OpenServiceA fail! GetLastError: %d", GetLastError());
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
std::string MF_CommonFunc::GetServiceNameByProcessId(DWORD vProcessId)
{
	LONG							tLongRet			= 0;
	BOOL							tBoolRet			= FALSE;
	SC_HANDLE						tSCHandle			= NULL;     // �������ݿ���
	char*							tBuffer				= NULL;     // ������ָ��
	DWORD							tBufferSize			= 0;        // ����Ļ��峤��
	DWORD 							tBufferSizeNeed		= 0;        // ��Ҫ�Ļ��峤��
	DWORD 							tNumberOfService	= 0;        // ���صķ������
	ENUM_SERVICE_STATUS_PROCESS*	tServiceInfo		= NULL;		// ������Ϣ


	// ������һ����������ƹ����������ӣ�����ָ�������ݿ�
	tSCHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CONNECT);
	if(NULL == tSCHandle)
	{
		LOG_ERROR("OpenSCManager fail! errno: %d", GetLastError());
		return "";
	}

	// ��ȡ��Ҫ�Ļ�������С
	EnumServicesStatusEx(tSCHandle, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, 
		NULL, tBufferSize, &tBufferSizeNeed, &tNumberOfService, NULL, NULL);

	// �����ô��1��������Ϣ�ĳ���
	tBufferSize = tBufferSizeNeed + sizeof(ENUM_SERVICE_STATUS_PROCESS);
	tBuffer = (char *)malloc(tBufferSize);
	if(NULL == tBuffer)
	{
		LOG_ERROR("malloc fail");
		return "";
	}
	memset(tBuffer, 0, tBufferSize);

	// ��ȡ������Ϣ
	tBoolRet = EnumServicesStatusEx(tSCHandle, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, 
		(LPBYTE)tBuffer, tBufferSize, &tBufferSizeNeed, &tNumberOfService, NULL, NULL);
	if(tBoolRet == FALSE)
	{
		LOG_ERROR("EnumServicesStatusEx fail! errno: %d", GetLastError());
		::CloseServiceHandle(tSCHandle);
		free(tBuffer);
		return "";
	}

	// �رմ򿪵ķ�����
	tBoolRet = ::CloseServiceHandle(tSCHandle);
	if(tBoolRet == FALSE)
	{
		free(tBuffer);
		LOG_WARN("CloseServiceHandle fail! errno: %d", GetLastError());
	}

	//printf("Service Num:%d\n", tNumberOfService);


	//��ȡ������Ϣ
	std::string tServiceName = "";
	tServiceInfo = (LPENUM_SERVICE_STATUS_PROCESS)tBuffer;
	for(unsigned int i = 0; i < tNumberOfService; i++)
	{
		if(vProcessId == tServiceInfo[i].ServiceStatusProcess.dwProcessId)
		{
			tServiceName = UnicodeToACP(tServiceInfo[i].lpServiceName);
			break;

			////��ӡ������Ϣ
			//printf("DisplayName \t\t : %s \n", UnicodeToACP(tServiceInfo[i].lpDisplayName).c_str());					//��ʾ����
			//printf("ServiceName \t\t : %s \n", UnicodeToACP(tServiceInfo[i].lpServiceName).c_str());					//��������
			//printf("ServiceType \t\t : %d \n", tServiceInfo[i].ServiceStatusProcess.dwServiceType);						//�����ִ���ļ�������
			//printf("CurrentState \t\t : %d \n", tServiceInfo[i].ServiceStatusProcess.dwCurrentState);						//����״̬
			//printf("ControlsAccepted \t : %d \n", tServiceInfo[i].ServiceStatusProcess.dwControlsAccepted);				//�������ʲô���Ŀ���֪ͨ
			//printf("Win32ExitCode \t\t : %d \n", tServiceInfo[i].ServiceStatusProcess.dwWin32ExitCode);
			//printf("ServiceSpecificExitCode  : %d \n", tServiceInfo[i].ServiceStatusProcess.dwServiceSpecificExitCode);
			//printf("CheckPoint \t\t : %d \n", tServiceInfo[i].ServiceStatusProcess.dwCheckPoint);
			//printf("WaitHint \t\t : %d \n", tServiceInfo[i].ServiceStatusProcess.dwWaitHint);
			//printf("Process Id \t\t : %d \n", tServiceInfo[i].ServiceStatusProcess.dwProcessId);							//����ID
			//printf("ServiceFlags \t\t : %d \n", tServiceInfo[i].ServiceStatusProcess.dwServiceFlags);
		}
	}
	free(tBuffer);
	return tServiceName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool MF_CommonFunc::UpdateXmlNode(string vConfPath, string vName, string vValue)
{
	if(true == vConfPath.empty())  return false;

	MC_XMLDocument<> tConfXml;
	bool tRet = tConfXml.LoadFromFile(vConfPath.c_str());
	if(false == tRet)
	{
		LOG_ERROR("LoadFromFile %s fali", vConfPath.c_str());
		return false;
	}
	rapidxml::xml_node<>* tRootNode = tConfXml.first_node();
	if(0 == tRootNode)
		return false;

	rapidxml::xml_node<>* tPropertyNode = tRootNode->first_node("property");
	while(tPropertyNode)
	{
		rapidxml::xml_node<>* tNameNode     = tPropertyNode->first_node("name");
		rapidxml::xml_node<>* tValueNode    = tPropertyNode->first_node("value");
		if(vName == tNameNode->value())
		{
			tValueNode->remove_all_nodes();
			char* tValue = tConfXml.allocate_string(ACPToUTF_8(vValue.c_str()).c_str());
			tConfXml.SetValue(tValueNode, tValue);
			break;
		}

		tPropertyNode = tPropertyNode->next_sibling();
	}

	tConfXml.SaveToFile(vConfPath.c_str(), MC_XMLDocument<>::UTF8);
	return true;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::CreateXml(std::string vConfPath)
{
	xml_document<> tDoc;
	xml_node<>* tRoot = tDoc.allocate_node(rapidxml::node_pi,tDoc.allocate_string("xml version='1.0' encoding='gb2312'"));
	tDoc.append_node(tRoot);

	xml_node<>* tNode = tDoc.allocate_node(node_element,"KKXmlInf", NULL);
	tNode->append_attribute(tDoc.allocate_attribute("version", "1.5"));
	tDoc.append_node(tNode);

	xml_node<>* tSubNode = tDoc.allocate_node(node_element, "ExFastInf", NULL);
	tSubNode->append_attribute(tDoc.allocate_attribute("S0", "DELETE FROM _rule"));
	tSubNode->append_attribute(tDoc.allocate_attribute("S1", "INSERT INTO _rule(Application, Online) VALUES('XKKNST_Client_Setup(NoKey).exe', '0')"));
	tNode->append_node(tSubNode);

	std::ofstream tOut(vConfPath.c_str());
	tOut<<tDoc;

	return true;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::LoadConfig(std::string vConfPath)
{
	if(true == vConfPath.empty())
		return false;

	MC_XMLDocument<> tConfXml;
	bool tRet = tConfXml.LoadFromFile(vConfPath.c_str());
	if(false == tRet)
	{
		LOG_ERROR("load config file fail. path: %s", vConfPath.c_str());
		return false;
	}

	rapidxml::xml_node<>* tRootNode = tConfXml.first_node();
	if(0 == tRootNode)
	{
		LOG_ERROR("config xml node root is null");
		return false;
	}

	rapidxml::xml_node<>* tPropertyNode = tRootNode->first_node("property");
	while(tPropertyNode)
	{
		rapidxml::xml_node<>* tNameNode     = tPropertyNode->first_node("name");
		rapidxml::xml_node<>* tValueNode    = tPropertyNode->first_node("value");

		if(std::string("workstation.site.xml") == tNameNode->value())
		{
			std::string m_WsConfPath = (NULL == tValueNode->value()) ? "" : tValueNode->value();
		}
		else
		{
			OutputDebugStringA(tNameNode->value());
			OutputDebugStringA(tValueNode->value());
		}
		tPropertyNode = tPropertyNode->next_sibling();
	}
	return true;
}
//---------------------------------------------------------------------------
/*
<?xml version="1.0" encoding="gb2312"?>
<KKXmlInf Version="1.5">
<ExFastInf EnableLog="0" EnableSys="1" Mode="1" NetflowKB="0" Time="">
<R0 Allow="1" IPFrom="192.168.0.0" IPTo="192.168.255.255" IsAllowAll="0" Memo="" NetflowLimit="0" PortFrom="0" PortTo="65535"></R0>
<R1 Allow="1" IPFrom="10.0.0.0" IPTo="10.255.255.255" IsAllowAll="0" Memo="" NetflowLimit="0" PortFrom="0" PortTo="65535"></R1>
<R2 Allow="0" IPFrom="0.0.0.0" IPTo="255.255.255.255" IsAllowAll="1" Memo="Allow all" NetflowLimit="0" PortFrom="0" PortTo="65535"></R2>
</ExFastInf>
</KKXmlInf>
*/
std::string CreateSubXmlTest()
{
	xml_document<> tDoc;
	xml_node<>* tRoot = tDoc.allocate_node(rapidxml::node_pi,tDoc.allocate_string("xml version=\"1.0\" encoding=\"gb2312\""));
	tDoc.append_node(tRoot);

	xml_node<>* tKKXmlInfNode = tDoc.allocate_node(node_element,"KKXmlInf", NULL);
	tKKXmlInfNode->append_attribute(tDoc.allocate_attribute("version", "1.5"));
	tDoc.append_node(tKKXmlInfNode);

	xml_node<>* tExFastInfNode = tDoc.allocate_node(node_element, "ExFastInf", NULL);
	tExFastInfNode->append_attribute(tDoc.allocate_attribute("EnableLog", "0"));
	tExFastInfNode->append_attribute(tDoc.allocate_attribute("EnableSys", "1"));
	tExFastInfNode->append_attribute(tDoc.allocate_attribute("Mode", "1"));
	tExFastInfNode->append_attribute(tDoc.allocate_attribute("NetflowKB", "0"));
	tExFastInfNode->append_attribute(tDoc.allocate_attribute("Time", ""));
	tKKXmlInfNode->append_node(tExFastInfNode);

	xml_node<>* tR0Node = tDoc.allocate_node(node_element, "R0", NULL);
	tR0Node->append_attribute(tDoc.allocate_attribute("Allow", "1"));
	tR0Node->append_attribute(tDoc.allocate_attribute("IPFrom", "192.168.0.0"));
	tR0Node->append_attribute(tDoc.allocate_attribute("IPTo", "192.168.255.255"));
	tR0Node->append_attribute(tDoc.allocate_attribute("IsAllowAll", "0"));
	tR0Node->append_attribute(tDoc.allocate_attribute("Memo", ""));
	tR0Node->append_attribute(tDoc.allocate_attribute("NetflowLimit", "0"));
	tR0Node->append_attribute(tDoc.allocate_attribute("PortFrom", "0"));
	tR0Node->append_attribute(tDoc.allocate_attribute("PortTo", "65535"));
	tExFastInfNode->append_node(tR0Node);

	xml_node<>* tR1Node = tDoc.allocate_node(node_element, "R1", NULL);
	tR1Node->append_attribute(tDoc.allocate_attribute("Allow", "0"));
	tR1Node->append_attribute(tDoc.allocate_attribute("IPFrom", "0.0.0.0"));
	tR1Node->append_attribute(tDoc.allocate_attribute("IPTo", "255.255.255.255"));
	tR1Node->append_attribute(tDoc.allocate_attribute("IsAllowAll", "1"));
	tR1Node->append_attribute(tDoc.allocate_attribute("Memo", "Allow all"));
	tR1Node->append_attribute(tDoc.allocate_attribute("NetflowLimit", "0"));
	tR1Node->append_attribute(tDoc.allocate_attribute("PortFrom", "0"));
	tR1Node->append_attribute(tDoc.allocate_attribute("PortTo", "65535"));
	tExFastInfNode->append_node(tR1Node);

	std::string tText;    
	rapidxml::print(std::back_inserter(tText), tDoc, 0);   

	return tText;
}
//---------------------------------------------------------------------------
//#define RX_Attribute_10	"&lt;?xml version=&quot;1.0&quot; encoding=&quot;gb2312&quot;?&gt;&lt;KKXmlInf Version=&quot;1.5&quot;&gt;&lt;ExFastInf EnableLog=&quot;0&quot; EnableSys=&quot;1&quot; Mode=&quot;0&quot; NetflowKB=&quot;0&quot; Time=&quot;&quot;&gt;&lt;R0 Allow=&quot;1&quot; IPFrom=&quot;192.168.0.0&quot; IPTo=&quot;192.168.255.255&quot; IsAllowAll=&quot;0&quot; Memo=&quot;&quot; NetflowLimit=&quot;0&quot; PortFrom=&quot;0&quot; PortTo=&quot;65535&quot;&gt;&lt;/R0&gt;&lt;R1 Allow=&quot;1&quot; IPFrom=&quot;10.0.0.0&quot; IPTo=&quot;10.255.255.255&quot; IsAllowAll=&quot;0&quot; Memo=&quot;&quot; NetflowLimit=&quot;0&quot; PortFrom=&quot;0&quot; PortTo=&quot;65535&quot;&gt;&lt;/R1&gt;&lt;R2 Allow=&quot;0&quot; IPFrom=&quot;0.0.0.0&quot; IPTo=&quot;255.255.255.255&quot; IsAllowAll=&quot;1&quot; Memo=&quot;Allow all&quot; NetflowLimit=&quot;0&quot; PortFrom=&quot;0&quot; PortTo=&quot;65535&quot;&gt;&lt;/R2&gt;&lt;/ExFastInf&gt;&lt;/KKXmlInf&gt;"

//#define RX_Attribute_11	"&lt;?xml version=&quot;1.0&quot; encoding=&quot;gb2312&quot;?&gt;&lt;KKXmlInf Version=&quot;1.5&quot;&gt;&lt;ExFastInf EnableLog=&quot;0&quot; EnableSys=&quot;1&quot; Mode=&quot;0&quot; NetflowKB=&quot;0&quot; Time=&quot;&quot;&gt;&lt;R0 Allow=&quot;1&quot; IPFrom=&quot;192.168.0.0&quot; IPTo=&quot;192.168.255.255&quot; IsAllowAll=&quot;0&quot; Memo=&quot;&quot; NetflowLimit=&quot;0&quot; PortFrom=&quot;0&quot; PortTo=&quot;65535&quot;&gt;&lt;/R0&gt;&lt;R1 Allow=&quot;1&quot; IPFrom=&quot;11.0.0.0&quot; IPTo=&quot;11.255.255.255&quot; IsAllowAll=&quot;0&quot; Memo=&quot;&quot; NetflowLimit=&quot;0&quot; PortFrom=&quot;0&quot; PortTo=&quot;65535&quot;&gt;&lt;/R1&gt;&lt;R2 Allow=&quot;0&quot; IPFrom=&quot;0.0.0.0&quot; IPTo=&quot;255.255.255.255&quot; IsAllowAll=&quot;1&quot; Memo=&quot;Allow all&quot; NetflowLimit=&quot;0&quot; PortFrom=&quot;0&quot; PortTo=&quot;65535&quot;&gt;&lt;/R2&gt;&lt;/ExFastInf&gt;&lt;/KKXmlInf&gt;"

#define RX_Attribute_11	"<?xml version=\"1.0\" encoding=\"gb2312\"?><KKXmlInf Version=\"1.5\"><ExFastInf EnableLog=\"0\" EnableSys=\"1\" Mode=\"0\" NetflowKB=\"0\" Time=\"\"><R0 Allow=\"1\" IPFrom=\"192.168.0.0\" IPTo=\"192.168.255.255\" IsAllowAll=\"0\" Memo=\"\" NetflowLimit=\"0\" PortFrom=\"0\" PortTo=\"65535\"></R0><R1 Allow=\"1\" IPFrom=\"11.0.0.0\" IPTo=\"11.255.255.255\" IsAllowAll=\"0\" Memo=\"\" NetflowLimit=\"0\" PortFrom=\"0\" PortTo=\"65535\"></R1><R2 Allow=\"0\" IPFrom=\"0.0.0.0\" IPTo=\"255.255.255.255\" IsAllowAll=\"1\" Memo=\"Allow all\" NetflowLimit=\"0\" PortFrom=\"0\" PortTo=\"65535\"></R2></ExFastInf></KKXmlInf>"
bool MF_CommonFunc::CreateXmlTest()
{
	xml_document<> tDoc;
	xml_node<>* tRoot = tDoc.allocate_node(rapidxml::node_pi,tDoc.allocate_string("xml version=\"1.0\" encoding=\"gb2312\""));
	tDoc.append_node(tRoot);

	xml_node<>* tKKXmlInfNode = tDoc.allocate_node(node_element,"KKXmlInf", NULL);
	tKKXmlInfNode->append_attribute(tDoc.allocate_attribute("version", "1.5"));
	tDoc.append_node(tKKXmlInfNode);

	xml_node<>* tExFastInfNode = tDoc.allocate_node(node_element, "ExFastInf", NULL);
	tExFastInfNode->append_attribute(tDoc.allocate_attribute("OfflineRule", "R1"));
	tExFastInfNode->append_attribute(tDoc.allocate_attribute("OnlineRule", "R0"));
	tKKXmlInfNode->append_node(tExFastInfNode);

	//std::string tSub = CreateSubXmlTest();
	//std::string tSub = "&lt;?xml version=&quot;1.0&quot; encoding=&quot;gb2312&quot;?&gt;&lt;KKXmlInf Version=&quot;1.5&quot;&gt;&lt;ExFastInf EnableLog=&quot;0&quot; EnableSys=&quot;1&quot; Mode=&quot;1&quot; NetflowKB=&quot;0&quot; Time=&quot;&quot;&gt;&lt;R0 Allow=&quot;1&quot; IPFrom=&quot;192.168.0.0&quot; IPTo=&quot;192.168.255.255&quot; IsAllowAll=&quot;0&quot; Memo=&quot;&quot; NetflowLimit=&quot;0&quot; PortFrom=&quot;0&quot; PortTo=&quot;65535&quot;&gt;&lt;/R0&gt;&lt;R1 Allow=&quot;0&quot; IPFrom=&quot;0.0.0.0&quot; IPTo=&quot;255.255.255.255&quot; IsAllowAll=&quot;1&quot; Memo=&quot;Allow all&quot; NetflowLimit=&quot;0&quot; PortFrom=&quot;0&quot; PortTo=&quot;65535&quot;&gt;&lt;/R1&gt;&lt;/ExFastInf&gt;&lt;/KKXmlInf&gt;";
	std::string tRule = "<?xml version=\"1.0\" encoding=\"gb2312\"?><KKXmlInf Version=\"1.5\"><ExFastInf EnableLog=\"0\" EnableSys=\"1\" Mode=\"1\" NetflowKB=\"0\" Time=\"\"><R0 Allow=\"1\" IPFrom=\"192.168.0.0\" IPTo=\"192.168.255.255\" IsAllowAll=\"0\" Memo=\"\" NetflowLimit=\"0\" PortFrom=\"0\" PortTo=\"65535\"></R0><R1 Allow=\"0\" IPFrom=\"0.0.0.0\" IPTo=\"255.255.255.255\" IsAllowAll=\"1\" Memo=\"Allow all\" NetflowLimit=\"0\" PortFrom=\"0\" PortTo=\"65535\"></R1></ExFastInf></KKXmlInf>";



	xml_node<>* tR0Node = tDoc.allocate_node(node_element, "R0", NULL);
	tR0Node->append_attribute(tDoc.allocate_attribute("V1", tRule.c_str()));
	tExFastInfNode->append_node(tR0Node);

	xml_node<>* tR1Node = tDoc.allocate_node(node_element, "R1", NULL);
	tR1Node->append_attribute(tDoc.allocate_attribute("V1", tRule.c_str()));
	tExFastInfNode->append_node(tR1Node);

	std::string tConfPath = "C:\\Users\\kinkoo\\Desktop\\test.xml";
	std::ofstream tOut(tConfPath.c_str());
	tOut<<tDoc;

	std::string tText;    
	rapidxml::print(std::back_inserter(tText), tDoc, 0);

	return true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
BOOL MF_CommonFunc::CreateFileShortcut(const char* vSrcFile, const char* vLinkFile, const char* vLinkDir, WORD vHotKey, const char* vDescription, int vShowCmd)
{
	if(0 == vLinkDir)	return FALSE;
	if(0 == *vLinkDir)	return FALSE;

	CoInitialize(NULL);

	HRESULT			tHr;
	IShellLink*		tShellLink;  //IShellLink����ָ��
	IPersistFile*	tPf; //IPersisFil����ָ��

	//����IShellLink����
	tHr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&tShellLink);
	if (FAILED(tHr))
	{
		LOG_ERROR("CoCreateInstance fail, errno: %#0x", tHr);
		CoUninitialize();
		return FALSE;
	}

	//Դ
	if(NULL == vSrcFile)
		tShellLink->SetPath(ACPToUnicode(_pgmptr).c_str());
	else
		tShellLink->SetPath(ACPToUnicode(vSrcFile).c_str());


	//��ݼ�
	if (vHotKey != 0)
		tShellLink->SetHotkey(vHotKey);

	//��ע
	if (vDescription != NULL)
		tShellLink->SetDescription(ACPToUnicode(vDescription).c_str());

	//��ʾ��ʽ
	tShellLink->SetShowCmd(vShowCmd);


	//ָ��Ŀ¼
	char tBuffer[MAX_PATH];
	if (vLinkFile != NULL) //ָ���˿�ݷ�ʽ������
		sprintf(tBuffer, "%s\\%s", vLinkDir, vLinkFile);
	else  
	{
		//û��ָ�����ƣ��ʹ�ȡָ���ļ����ļ�����Ϊ��ݷ�ʽ���ơ�
		char* tStr = "";
		if (vSrcFile != NULL)
			tStr = strrchr((char*)vSrcFile, '\\');
		else
			tStr = strrchr(_pgmptr, '\\');

		if (tStr == NULL)
		{    
			LOG_ERROR("tStr == NULL");
			tShellLink->Release();
			CoUninitialize();
			return FALSE;
		}
		//ע���׺��Ҫ��.exe��Ϊ.lnk
		sprintf(tBuffer, "%s\\%s", vLinkDir, tStr);
		int tLen = strlen(tBuffer);
		tBuffer[tLen - 3] = 'l';
		tBuffer[tLen - 2] = 'n';
		tBuffer[tLen - 1] = 'k';
	}


	//��IShellLink�����л�ȡIPersistFile�ӿ�
	tHr = tShellLink->QueryInterface(IID_IPersistFile, (void**)&tPf);
	if (FAILED(tHr))
	{
		LOG_ERROR("QueryInterface fail, errno: %#0x", tHr);
		tShellLink->Release();
		CoUninitialize();
		return FALSE;
	}

	//�����ݷ�ʽ��ָ��Ŀ¼��
	//WCHAR  tWideStr[MAX_PATH];  //����Unicode�ַ���
	//MultiByteToWideChar(CP_ACP, 0, tBuffer, -1, tWideStr, MAX_PATH);
	tHr = tPf->Save(ACPToUnicode(tBuffer).c_str(), TRUE);

	tPf->Release();
	tShellLink->Release();
	CoUninitialize();

	return SUCCEEDED(tHr);
}
//---------------------------------------------------------------------------
//�õ���ǰ����·��
std::string MF_CommonFunc::GetDesktopPath()
{
	/*HRESULT			tHr				= S_FALSE;
	LPITEMIDLIST	tPidl			= NULL;
	char			tPath[MAX_PATH] = {0};

	tHr = SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &tPidl);
	if(S_OK != tHr)
	{
		LOG_ERROR("SHGetSpecialFolderLocation fail, errno: %d", tHr);
		return "";
	}

	BOOL tFlag =  SHGetPathFromIDListA(tPidl, tPath);
	CoTaskMemFree(tPidl);
	if(FALSE == tFlag)
		return "";*/

	std::string tPath = "";
	wchar_t* localAppData = NULL;
	if (SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &localAppData) == S_OK)
	{
		tPath = UnicodeToACP(localAppData);
		CoTaskMemFree(reinterpret_cast<void*>(localAppData));
	}

	return tPath;
}    
//---------------------------------------------------------------------------
//�õ�������������·��
std::string MF_CommonFunc::GetIEQuickLaunchPath()
{    
	HRESULT			tHr				= S_FALSE;
	LPITEMIDLIST	tPidl			= NULL;
	char			tPath[MAX_PATH] = {0};

	tHr = SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &tPidl);
	if(S_OK != tHr)
	{
		LOG_ERROR("SHGetSpecialFolderLocation fail, errno: %d", tHr);
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
//�õ� ��ʼ->������ ��·��
std::string MF_CommonFunc::GetProgramsPath()
{
	HRESULT			tHr				= S_FALSE;
	LPITEMIDLIST	tPidl			= NULL;
	char			tPath[MAX_PATH] = {0};

	tHr = SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &tPidl);
	if(S_OK != tHr)
	{
		LOG_ERROR("SHGetSpecialFolderLocation fail, errno: %d", tHr);
		return "";
	}

	BOOL tFlag =  SHGetPathFromIDListA(tPidl, tPath);
	CoTaskMemFree(tPidl);
	if(FALSE == tFlag)
		return "";

	return tPath;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::CreatLinkToDesktop()
{
	HRESULT tHr = CoInitialize(NULL);
	if(FAILED(tHr))
	{
		LOG_ERROR("CoInitialize fail, errno: %#0x", tHr);
		return false;
	}

	IShellLink *tShellLink;
	tHr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&tShellLink);
	if(FAILED(tHr))
	{
		LOG_ERROR("CoCreateInstance fail, errno: %#0x", tHr);
		CoUninitialize();
		return false;
	}

	IPersistFile* tIPF;
	//����������Ҫ������ݷ�ʽ��ԭʼ�ļ���ַ
	tShellLink->SetPath(ACPToUnicode(_pgmptr).c_str());	//Ϊʲô����ֱ��ʹ��_wpgmptr������
	tHr = tShellLink->QueryInterface(IID_IPersistFile, (void**)&tIPF);
	if(FAILED(tHr))
	{
		LOG_ERROR("QueryInterface fail, errno: %#0x", tHr);
		tShellLink->Release();
		CoUninitialize();
		return false;
	}

	//����������Ҫ������ݷ�ʽ��Ŀ���ַ
	char tStartPath[MAX_PATH] = {0};
	SHGetSpecialFolderPathA(NULL,tStartPath, CSIDL_DESKTOP, 0);		//����·��
	sprintf(tStartPath, "%s\\%s", tStartPath, strrchr(_pgmptr, '\\'));

	//ע���׺��Ҫ��.exe��Ϊ.lnk
	int tLen = strlen(tStartPath);
	tStartPath[tLen - 3] = 'l';
	tStartPath[tLen - 2] = 'n';
	tStartPath[tLen - 1] = 'k';

	//USES_CONVERSION;
	//LPCOLESTR lpOleStr = A2COLE(tStartPath);	//���ֽ�
	tIPF->Save(ACPToUnicode(tStartPath).c_str(), FALSE);	//Unicode

	tIPF->Release();
	tShellLink->Release();
	CoUninitialize();

	return true;
}
//---------------------------------------------------------------------------
BOOL MF_CommonFunc::CreateFileShortcutA(const char* vSrcFile, const char* vLinkFile, const char* vLinkDir, WORD vHotKey, const char* vDescription, int vShowCmd)
{
	if(0 == vLinkDir)	return FALSE;
	if(0 == *vLinkDir)	return FALSE;

	CoInitialize(NULL);

	HRESULT			tHr;
	IShellLink*		tShellLink;  //IShellLink����ָ��
	IPersistFile*	tPf; //IPersisFil����ָ��

	//����IShellLink����
	tHr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&tShellLink);
	if (FAILED(tHr))
	{
		LOG_ERROR("CoCreateInstance fail, errno: %#0x", tHr);
		CoUninitialize();
		return FALSE;
	}

	//Դ
	if(NULL == vSrcFile)
		tShellLink->SetPath(ACPToUnicode(_pgmptr).c_str());
	else
		tShellLink->SetPath(ACPToUnicode(vSrcFile).c_str());


	//��ݼ�
	if (vHotKey != 0)
		tShellLink->SetHotkey(vHotKey);

	//��ע
	if (vDescription != NULL)
		tShellLink->SetDescription(ACPToUnicode(vDescription).c_str());

	//��ʾ��ʽ
	tShellLink->SetShowCmd(vShowCmd);


	//ָ��Ŀ¼
	char tBuffer[MAX_PATH];
	if (vLinkFile != NULL) //ָ���˿�ݷ�ʽ������
		sprintf(tBuffer, "%s\\%s", vLinkDir, vLinkFile);
	else  
	{
		//û��ָ�����ƣ��ʹ�ȡָ���ļ����ļ�����Ϊ��ݷ�ʽ���ơ�
		char* tStr = "";
		if (vSrcFile != NULL)
			tStr = strrchr((char*)vSrcFile, '\\');
		else
			tStr = strrchr(_pgmptr, '\\');

		if (tStr == NULL)
		{    
			LOG_ERROR("tStr == NULL");
			tShellLink->Release();
			CoUninitialize();
			return FALSE;
		}
		//ע���׺��Ҫ��.exe��Ϊ.lnk
		sprintf(tBuffer, "%s\\%s", vLinkDir, tStr);
		int tLen = strlen(tBuffer);
		tBuffer[tLen - 3] = 'l';
		tBuffer[tLen - 2] = 'n';
		tBuffer[tLen - 1] = 'k';
	}


	//��IShellLink�����л�ȡIPersistFile�ӿ�
	tHr = tShellLink->QueryInterface(IID_IPersistFile, (void**)&tPf);
	if (FAILED(tHr))
	{
		LOG_ERROR("QueryInterface fail, errno: %#0x", tHr);
		tShellLink->Release();
		CoUninitialize();
		return FALSE;
	}

	//�����ݷ�ʽ��ָ��Ŀ¼��
	//WCHAR  tWideStr[MAX_PATH];  //����Unicode�ַ���
	//MultiByteToWideChar(CP_ACP, 0, tBuffer, -1, tWideStr, MAX_PATH);
	tHr = tPf->Save(ACPToUnicode(tBuffer).c_str(), TRUE);

	tPf->Release();
	tShellLink->Release();
	CoUninitialize();

	return SUCCEEDED(tHr);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool MF_CommonFunc::GetSingleDiskInfo(const char* vName, ULONGLONG& vAll, ULONGLONG& vUsed, ULONGLONG& vFree )
{
	ULARGE_INTEGER tFreeBytesAvailable;
	ULARGE_INTEGER tTotalNumberOfBytes;
	ULARGE_INTEGER tTotalNumberOfFreeBytes;


	if(FALSE == GetDiskFreeSpaceExA(vName,&tFreeBytesAvailable,&tTotalNumberOfBytes,&tTotalNumberOfFreeBytes))
	{
		LOG_ERROR("MC_SysHelper::GetSingleDiskInfo->Call to GetDiskFreeSpaceEx() failed. path: [%s], errCode: [%d].",vName,GetLastError());
		return false;
	}
	vAll    = (int) ((double)(tTotalNumberOfBytes.QuadPart)/(1024*1024));
	vFree   = (int) ((double)(tTotalNumberOfFreeBytes.QuadPart)/(1024*1024));
	vUsed   = vAll - vFree;
	return true;
}
//---------------------------------------------------------------------------
int MF_CommonFunc::GetCpu(void)
{
	FILETIME tTempIdleTime		= {0};
	FILETIME tTempKernelTime	= {0};
	FILETIME tTempUserTime		= {0};

	ULARGE_INTEGER tPreIdleTime		= {0};
	ULARGE_INTEGER tPreKernelTime	= {0};
	ULARGE_INTEGER tPreUserTime		= {0};
	if(FALSE == GetSystemTimes(&tTempIdleTime,&tTempKernelTime,&tTempUserTime))
		return -1;
	tPreIdleTime.LowPart	= tTempIdleTime.dwLowDateTime;
	tPreIdleTime.HighPart	= tTempIdleTime.dwHighDateTime;
	tPreKernelTime.LowPart	= tTempKernelTime.dwLowDateTime;
	tPreKernelTime.HighPart	= tTempKernelTime.dwHighDateTime;
	tPreUserTime.LowPart	= tTempUserTime.dwLowDateTime;
	tPreUserTime.HighPart	= tTempUserTime.dwHighDateTime;

	Sleep(1000);

	ULARGE_INTEGER tCurIdleTime		= {0};
	ULARGE_INTEGER tCurKernelTime	= {0};
	ULARGE_INTEGER tCurUserTime		= {0};
	if(FALSE == GetSystemTimes(&tTempIdleTime,&tTempKernelTime,&tTempUserTime))
		return -1;
	tCurIdleTime.LowPart	= tTempIdleTime.dwLowDateTime;
	tCurIdleTime.HighPart	= tTempIdleTime.dwHighDateTime;
	tCurKernelTime.LowPart	= tTempKernelTime.dwLowDateTime;
	tCurKernelTime.HighPart	= tTempKernelTime.dwHighDateTime;
	tCurUserTime.LowPart	= tTempUserTime.dwLowDateTime;
	tCurUserTime.HighPart	= tTempUserTime.dwHighDateTime;

	//����
	ULARGE_INTEGER tIdleTime	= {0};
	ULARGE_INTEGER tKernelTime	= {0};
	ULARGE_INTEGER tUserTime	= {0};
	tIdleTime.QuadPart		= tCurIdleTime.QuadPart - tPreIdleTime.QuadPart;
	tKernelTime.QuadPart	= tCurKernelTime.QuadPart - tPreKernelTime.QuadPart;
	tUserTime.QuadPart		= tCurUserTime.QuadPart - tCurUserTime.QuadPart;

	if(0 != (tKernelTime.QuadPart + tUserTime.QuadPart))
		return  (int)((tKernelTime.QuadPart + tUserTime.QuadPart - tIdleTime.QuadPart)*100 / (tKernelTime.QuadPart + tUserTime.QuadPart));
	return -1;
}
//---------------------------------------------------------------------------
int MF_CommonFunc::GetRam(void)
{
	MEMORYSTATUSEX tMemoryInfo = {sizeof(MEMORYSTATUSEX)};
	GlobalMemoryStatusEx(&tMemoryInfo);
	//return (int)((double)tMemoryInfo.ullAvailPhys / (1024*1024));

	printf("%d\n", (int)((double)tMemoryInfo.ullTotalPhys / (1024*1024*1024)));

	return tMemoryInfo.dwMemoryLoad;
}
//---------------------------------------------------------------------------
std::string MF_CommonFunc::GetMac(void)
{
	std::string tMacStr;
	ULONG		tOutBufLen			= sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO pAdapterInfo	= (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	if(pAdapterInfo == NULL)
		return false;

	//��ȡ��ַ��С
	if(GetAdaptersInfo(pAdapterInfo, &tOutBufLen) == ERROR_BUFFER_OVERFLOW) 
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(tOutBufLen);
		if(pAdapterInfo == NULL) 
			return false;
	}

	if(GetAdaptersInfo(pAdapterInfo, &tOutBufLen) == NO_ERROR)
	{
		for(PIP_ADAPTER_INFO pAdapter = pAdapterInfo; pAdapter != NULL; pAdapter = pAdapter->Next)
		{
			// ȷ������̫��
			if(pAdapter->Type != MIB_IF_TYPE_ETHERNET)
				continue;

			// ȷ��MAC��ַ�ĳ���Ϊ 00-00-00-00-00-00
			if(pAdapter->AddressLength != 6)
				continue;

			char tMac[32];
			sprintf(tMac, "%02X:%02X:%02X:%02X:%02X:%02X",
				int (pAdapter->Address[0]),
				int (pAdapter->Address[1]),
				int (pAdapter->Address[2]),
				int (pAdapter->Address[3]),
				int (pAdapter->Address[4]),
				int (pAdapter->Address[5]));
			tMacStr = std::string(tMac);
			break;
		}
	}

	free(pAdapterInfo);
	return tMacStr;
}
//--------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
string MF_CommonFunc::GetStrMD5(const string vString)
{
	MD5 tMD5;
	tMD5.update(vString);
	return tMD5.toString();
}
//---------------------------------------------------------------------------
string MF_CommonFunc::GetFileMD5(const string vFileName)
{
	ifstream tIn(vFileName.c_str(), ios::binary);
	if(!tIn)
		return "";

	MD5 tMD5;
	std::streamsize tLength;
	char tBuffer[1024] = {0};

	while(!tIn.eof())
	{
		tIn.read(tBuffer, sizeof(tBuffer));
		tLength = tIn.gcount();
		if(tLength > 0)
			tMD5.update(tBuffer, tLength);
	}

	tIn.close();
	return tMD5.toString();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
BOOL MF_CommonFunc::EnablePriviledge()
{
	BOOL				tRet		= FALSE;	
	HANDLE				tToken		= NULL;
	LUID				tLuid;
	TOKEN_PRIVILEGES	tTokenPriv;
	ZeroMemory(&tTokenPriv, sizeof(TOKEN_PRIVILEGES));

	tRet = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &tToken);
	if (FALSE == tRet) 
	{
		if (GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)	//ϵͳ��֧�ָù���	
		{
			SetLastError(ERROR_SUCCESS);
			SafeCloseHHandle(tToken);
			return TRUE;
		}
		
		LOG_ERROR("OpenProcessToken fail, errno: %d", GetLastError());
		SafeCloseHHandle(tToken);
		return FALSE;
	}

	tRet = LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tLuid);
	if (FALSE == tRet)
	{
		LOG_ERROR("LookupPrivilegeValue fail, errno: %d", GetLastError());
		SafeCloseHHandle(tToken);
		return FALSE;
	}
	tTokenPriv.PrivilegeCount				= 1;
	tTokenPriv.Privileges[0].Luid			= tLuid;
	tTokenPriv.Privileges[0].Attributes		= SE_PRIVILEGE_ENABLED;

	//1300: ����û�и�Ȩ��
	tRet = AdjustTokenPrivileges(tToken, FALSE, &tTokenPriv, 0, NULL, 0);
	if(FALSE == tRet)
	{
		LOG_ERROR("AdjustTokenPrivileges fail, errno: %d", GetLastError());
		SafeCloseHHandle(tToken);
		return FALSE;
	}

	SafeCloseHHandle(tToken);
	return TRUE;
}
//---------------------------------------------------------------------------
BOOL MF_CommonFunc::EnableFileAccountPrivilege (char* vDestPath, char* vAccount)
{
	BOOL				tSuccess	= TRUE;
	PACL				tOldDacl	= NULL;
	PACL				tNewDacl	= NULL;
	EXPLICIT_ACCESSA	tEa;

	do
	{
		// ��ȡ�ļ�(��)��ȫ�����DACL�б�(���ɷ��ʿ����б�)
		if(ERROR_SUCCESS != GetNamedSecurityInfoA((LPSTR)vDestPath, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &tOldDacl, NULL, NULL))
		{
			tSuccess =FALSE;
			break;
		}

		// �˴�����ֱ����AddAccessAllowedAce����,��Ϊ���е�DACL�����ǹ̶�,�������´���һ��DACL����
		// ����ָ���û��ʻ��ķ��ʿ�����Ϣ(����ָ������ȫ���ķ���Ȩ��)
		BuildExplicitAccessWithNameA(&tEa, vAccount, GENERIC_ALL, GRANT_ACCESS, SUB_CONTAINERS_AND_OBJECTS_INHERIT);

		// �����µ�ACL����(�ϲ����е�ACL����͸����ɵ��û��ʻ����ʿ�����Ϣ)
		if(ERROR_SUCCESS != SetEntriesInAclA(1, &tEa, tOldDacl, &tNewDacl))
		{
			tSuccess =FALSE;
			break;
		}

		// �����ļ�(��)��ȫ�����DACL�б�
		if (ERROR_SUCCESS != SetNamedSecurityInfoA(vDestPath, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, tNewDacl, NULL))
		{
			tSuccess = FALSE;
			break;
		}
	}while(FALSE);

	if (NULL != tNewDacl)
	{
		LocalFree(tNewDacl);
	}

	return tSuccess;
}
//---------------------------------------------------------------------------
BOOL MF_CommonFunc::GetProcessPriviledge(HANDLE hProcess)
{
	const int tMaxName = 256;

	DWORD i, dwSize = 0, dwResult = 0;
	HANDLE hToken;
	PTOKEN_GROUPS pGroupInfo;
	PSID pSID = NULL;
	SID_IDENTIFIER_AUTHORITY SIDAuth = SECURITY_NT_AUTHORITY;
	char lpName[tMaxName];
	char lpDomain[tMaxName];
	SID_NAME_USE SidType;

	// Open a handle to the access token for the calling process.
	//TOKEN_QUERY:Required to query an access token.
	//GetCurrentProcess()���ؽ��̾��
	//[out]hToken��access token�ľ��
	if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
	{
		printf("OpenProcessToken Error %u\n", GetLastError());
		return FALSE;
	}

	//ǰ�����ε���GetTokenInformation��Ŀ�Ĳ�ͬ
	// Call GetTokenInformation to get the buffer size.
	//The TOKEN_GROUPS structure contains information about the group security identifiers (SIDs) in an access token.
	if (!GetTokenInformation(hToken, TokenGroups, NULL, dwSize, &dwSize))
	{
		dwResult = GetLastError();
		if (dwResult != ERROR_INSUFFICIENT_BUFFER) {
			printf("GetTokenInformation Error %u\n", dwResult);
			return FALSE;
		}
	}

	// Allocate the buffer.
	pGroupInfo = (PTOKEN_GROUPS)GlobalAlloc(GPTR, dwSize);

	// Call GetTokenInformation again to get the group information.

	if (!GetTokenInformation(hToken, TokenGroups, pGroupInfo,
		dwSize, &dwSize))
	{
		printf("GetTokenInformation Error %u\n", GetLastError());
		return FALSE;
	}

	// Create a SID for the BUILTIN\Administrators group.

	if (!AllocateAndInitializeSid(&SIDAuth, 2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&pSID))
	{
		printf("AllocateAndInitializeSid Error %u\n", GetLastError());
		return FALSE;
	}
	// Loop through the group SIDs looking for the administrator SID.
	//
	for (i = 0; i < pGroupInfo->GroupCount; i++)
	{
		if (EqualSid(pSID, pGroupInfo->Groups[i].Sid))
		{

			// Lookup the account name and print it.

			dwSize = tMaxName;
			if (!LookupAccountSidA(NULL, pGroupInfo->Groups[i].Sid,
				lpName, &dwSize, lpDomain,
				&dwSize, &SidType))
			{
				dwResult = GetLastError();
				if (dwResult == ERROR_NONE_MAPPED)
					strcpy_s(lpName, dwSize, "NONE_MAPPED");
				else
				{
					printf("LookupAccountSid Error %u\n", GetLastError());
					return FALSE;
				}
			}

			printf("Current user is a member of the %s\\%s group\n",
				lpDomain, lpName);

			// Find out whether the SID is enabled in the token.
			if (pGroupInfo->Groups[i].Attributes & SE_GROUP_ENABLED)
				printf("The group SID is enabled.\n");
			else if (pGroupInfo->Groups[i].Attributes &
				SE_GROUP_USE_FOR_DENY_ONLY)
				printf("The group SID is a deny-only SID.\n");
			else
				printf("The group SID is not enabled.\n");
		}
	}
	if (pSID)
		FreeSid(pSID);
	if (pGroupInfo)
		GlobalFree(pGroupInfo);

	return TRUE;
}
//---------------------------------------------------------------------------
bool MF_CommonFunc::IsBigEndian()
{
	//int		a = 0x1234;
	//char	b = *(char*)&a;	//ͨ����intǿ������ת����char���ֽڣ�ͨ���ж���ʼ�洢λ�á������� ȡb����a�ĵ͵�ַ����  
	//if(0x12 == b)
	//	return true;

	//return false;

	
	//������union�Ĵ��˳�������г�Ա���ӵ͵�ַ��ʼ��ţ����ø����Կ������ɵػ����CPU���ڴ����Little-endian����Big-endianģʽ��д
	union NUM  
	{  
		int a;  
		char b;	//bֻ��һ���ֽڣ������൱�ھ���a�ĸ��ֽڵĵ͵�ַ�����Կ����Ǹ��ֽڻ��ǵ��ֽھ����ж���  
	}num;  

	num.a = 0x1234;  
	if( num.b == 0x12 )  
		return true;

	return false;
	 
};
//---------------------------------------------------------------------------
std::string MF_CommonFunc::TransErrorCode(DWORD vErrorCode)
{
	HANDLE	tLocaleHandle	= NULL;
	setlocale(LC_ALL, "Chinese-simplified");  //������ʾ
	DWORD tLocaleLangId = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

	BOOL tRet = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, vErrorCode, tLocaleLangId, (char*)&tLocaleHandle, 0 , NULL);
	if(FALSE == tRet)
	{
		// �������������
		HMODULE tDll = LoadLibraryExA("netmsg.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
		if (NULL != tDll)
		{
			tRet = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, tDll, vErrorCode, tLocaleLangId, (char*)&tLocaleHandle, 0 , NULL);
			FreeLibrary(tDll);
		}
	}

	std::string tMsg = "";
	if(FALSE != tRet && NULL != tLocaleHandle)
	{
		tMsg = (char*)LocalLock(tLocaleHandle);

		LocalFree(tLocaleHandle);
		tLocaleHandle = NULL;
	}

	return tMsg;
}
//---------------------------------------------------------------------------