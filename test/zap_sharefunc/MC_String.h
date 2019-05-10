#ifndef MC_String_H
#define MC_String_H
//---------------------------------------------------------------------------
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//---------------------------------------------------------------------------
#include <string>
using namespace std;

#include <vector>
//---------------------------------------------------------------------------
#define STR_LEN	260
//---------------------------------------------------------------------------
class MC_String
{
public:
	//STL字符串
	static	std::string		STRReplace					(const std::string vString, const std::string vSubStrFrom, const std::string vSubStrTo);	//将vString中的所有vSubStrFrom换为vSubStrTo
	static	std::string		ReplaceBackslashBySlash		(std::string vStr);							//用斜线(/)代替反斜杠(\\)
	static	std::string		Trim						(std::string vStr, std::string vTrimStr);	//去掉vStr中的指定字符vTrimStr

	static	void			SeparateString				(char* vStr,	const char* vSeps,		vector<std::string>& vRet);			//分离字符串
	static	void			SeparateStringW				(wchar_t* vStr, const wchar_t* vSeps,	vector<std::wstring>& vRet);

	static	std::string		STRCombine					(char* vFormat,...);

	//C 字符串
	static	char*			ChStrReplace				(char* vStr, char* vOldStr, char* vNewStr);		//将字符串vStr中的vOldStr换为vNewStr

	//类型转换
	static	std::string		IToString		(int i);
	static	std::string		I64ToString		(__int64 i);
	static	int				StringToI		(std::string s);

	//编码转换
	static	std::wstring	UTF_8ToUnicode	(const char* vBuf);
	static	std::string		UnicodeToUTF_8	(const wchar_t* vBuf);

	static	std::wstring	ACPToUnicode	(const char* vBuf);
	static	std::string		UnicodeToACP	(const wchar_t* vBuf);

	static	std::string 	ACPToUTF_8		(const char* vBuf);
	static	std::string 	UTF_8ToACP		(const char* vBuf);
	
	//其他
	static	void			ChWConvertEndian			(void * ptr, size_t size);
	static	std::string		CreateGuid					(bool minus=true);

};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
/*
char tStrA[] = "C:\\dir\\file";
std::vector<std::string> tVecA;
MC_String::SeparateStringA(tStrA, "\\", tVecA);

wchar_t tStrW[] = L"C:\\dir\\file";
std::vector<std::wstring> tVecW;
MC_String::SeparateStringW(tStrW, L"\\", tVecW);

std::string tBuf = "12345678";
MC_String::ChWConvertEndian((char*)tBuf.c_str(), tBuf.length());
 */