#include "MC_String.h"

#include <Windows.h>
#include <objbase.h>
//---------------------------------------------------------------------------
std::string MC_String::STRReplace(const std::string vString, const std::string vSubStrFrom, const std::string vSubStrTo)
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
std::string MC_String::ReplaceBackslashBySlash(std::string vStr)
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
std::string MC_String::Trim(std::string vStr, std::string vTrimStr) 
{
	if (vStr.empty()) 
	{
		return vStr;
	}

	//去掉头和尾的空格
	vStr.erase(0, vStr.find_first_not_of(vTrimStr));
	vStr.erase(vStr.find_last_not_of(vTrimStr) + 1);

	////去掉所有的空格
	//std::string::size_type	tPos	= vStr.find_first_of(vTrimStr);
	//while(std::string::npos != tPos)
	//{
	//	vStr.erase(tPos, 1);
	//	tPos = vStr.find_first_of(" ");
	//}
	
	return vStr;
}
//---------------------------------------------------------------------------
void MC_String::SeparateString(char* vStr, const char* vSeps, vector<std::string>& vRet)
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
void MC_String::SeparateStringW(wchar_t* vStr, const wchar_t* vSeps, vector<std::wstring>& vRet)
{
	if(NULL == vStr)		return ;
	if(NULL == vSeps)		return ;

	wchar_t* tToken		= NULL;
	wchar_t* tNextToken	= NULL;
	tToken = wcstok_s(vStr, vSeps, &tNextToken);
	while(NULL != tToken)
	{
		vRet.push_back(tToken);
		tToken = wcstok_s(NULL, vSeps, &tNextToken);
	}
}
//---------------------------------------------------------------------------
std::string MC_String::STRCombine(char* vFormat,...)
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
/************************************************************************/
/*                                                                      */
/************************************************************************/
//---------------------------------------------------------------------------
char* MC_String::ChStrReplace(char* vStr, char* vOldStr, char* vNewStr)
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
std::string MC_String::IToString(int i)
{
	char c[128];
	memset(c, 0, sizeof(c));
	_itoa_s(i, c, sizeof(c),10);
	return std::string(c);
}
//---------------------------------------------------------------------------
std::string MC_String::I64ToString(__int64 i)
{
	char c[128];
	memset(c, 0, sizeof(c));
	_i64toa_s(i, c, sizeof(c),10);
	return std::string(c);
}
//---------------------------------------------------------------------------
int MC_String::StringToI(std::string s)
{
	return atoi(s.c_str());
}
//---------------------------------------------------------------------------
std::wstring MC_String::UTF_8ToUnicode(const char* vBuf)
{
	int tLen = MultiByteToWideChar(CP_UTF8, 0, vBuf, -1, NULL, 0);
	std::vector<wchar_t> tUnicode(tLen);
	MultiByteToWideChar(CP_UTF8, 0, vBuf, -1, &tUnicode[0], tLen);

	return std::wstring(&tUnicode[0]);
}
//---------------------------------------------------------------------------
std::string MC_String::UnicodeToUTF_8(const wchar_t* vBuf)
{
	int tLen = WideCharToMultiByte(CP_UTF8, 0, vBuf, -1, NULL, 0, NULL, NULL);
	std::vector<char> tUtf8(tLen);
	WideCharToMultiByte(CP_UTF8, 0, vBuf, -1, &tUtf8[0], tLen, NULL, NULL);

	return std::string(&tUtf8[0]);
}
//---------------------------------------------------------------------------
std::wstring MC_String::ACPToUnicode(const char* vBuf)
{
	int tLen = MultiByteToWideChar(CP_ACP, 0, vBuf, -1, NULL, 0);
	std::vector<wchar_t> tUnicode(tLen);
	MultiByteToWideChar(CP_ACP, 0, vBuf, -1, &tUnicode[0], tLen);

	return std::wstring(&tUnicode[0]);
}
//---------------------------------------------------------------------------
std::string MC_String::UnicodeToACP(const wchar_t* vBuf)
{
	int tLen = WideCharToMultiByte(CP_ACP, 0, vBuf, -1, NULL, 0, NULL, NULL);
	std::vector<char> tAcp(tLen);
	WideCharToMultiByte(CP_ACP, 0, vBuf, -1, &tAcp[0], tLen, NULL, NULL);

	return std::string(&tAcp[0]);
}
//---------------------------------------------------------------------------
std::string MC_String::ACPToUTF_8(const char* vBuf)
{
	std::wstring tUnicode = ACPToUnicode(vBuf);
	return UnicodeToUTF_8(tUnicode.c_str());
}
//---------------------------------------------------------------------------
std::string MC_String::UTF_8ToACP(const char* vBuf)
{
	std::wstring tUnicode = UTF_8ToUnicode(vBuf);
	return UnicodeToACP(tUnicode.c_str());
}
//---------------------------------------------------------------------------
void MC_String::ChWConvertEndian(void * ptr, size_t size)
{
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
std::string MC_String::CreateGuid(bool minus/* =true */)
{
	std::string tStr	= "";
	GUID		tGuid;
	char		tBuffer[128];
	memset(tBuffer, 0, sizeof(tBuffer));

	if(0 == CoCreateGuid(&tGuid))
	{
		sprintf_s(tBuffer, sizeof(tBuffer), minus ? "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X" : "%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X",
			tGuid.Data1, tGuid.Data2, tGuid.Data3, 
			tGuid.Data4[0], tGuid.Data4[1], tGuid.Data4[2], tGuid.Data4[3], tGuid.Data4[4], tGuid.Data4[5], tGuid.Data4[6], tGuid.Data4[7]);
		
		tStr = tBuffer;
	}

	return tStr;
}
//---------------------------------------------------------------------------