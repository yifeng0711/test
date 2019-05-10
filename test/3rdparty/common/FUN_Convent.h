#ifndef FUN_ConventH
#define FUN_ConventH
//-----------------------------------------------------------------------------------------
#include <windows.h>
#include <string>
#include <vector>
//-----------------------------------------------------------------------------------------
inline std::wstring UTF_8ToUnicode(const char* vBuf)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, vBuf, -1, NULL, 0);
    std::vector<wchar_t> unicode(len);
	MultiByteToWideChar(CP_UTF8, 0, vBuf, -1, &unicode[0], len);

	return std::wstring(&unicode[0]);
}
//----------------------------------------------------------------------------------------
inline std::wstring ACPToUnicode(const char* vBuf)
{
	int len = MultiByteToWideChar(CP_ACP, 0, vBuf, -1, NULL, 0);
	std::vector<wchar_t> unicode(len);
	MultiByteToWideChar(CP_ACP, 0, vBuf, -1, &unicode[0], len);

	return std::wstring(&unicode[0]);
}
//----------------------------------------------------------------------------------------
inline std::string UnicodeToUTF_8(const wchar_t* vBuf)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, vBuf, -1, NULL, 0, NULL, NULL);
	std::vector<char> utf8(len);
	WideCharToMultiByte(CP_UTF8, 0, vBuf, -1, &utf8[0], len, NULL, NULL);

	return std::string(&utf8[0]);
}
//----------------------------------------------------------------------------------------
inline std::string UnicodeToACP(const wchar_t* vBuf)
{
	int len = WideCharToMultiByte(CP_ACP, 0, vBuf, -1, NULL, 0, NULL, NULL);
    std::vector<char> utf8(len);
	WideCharToMultiByte(CP_ACP, 0, vBuf, -1, &utf8[0], len, NULL, NULL);

	return std::string(&utf8[0]);
}
//----------------------------------------------------------------------------------------
inline std::string UTF_8ToACP(const char* vBuf)
{
	std::wstring tUnicode = UTF_8ToUnicode(vBuf);
	return UnicodeToACP(tUnicode.c_str());
}
//----------------------------------------------------------------------------------------
inline std::string ACPToUTF_8(const char* vBuf)
{
	std::wstring tUnicode = ACPToUnicode(vBuf);
	return UnicodeToUTF_8(tUnicode.c_str());
}
//-----------------------------------------------------------------------------------------
#endif