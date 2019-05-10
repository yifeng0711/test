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
	//STL�ַ���
	static	std::string		STRReplace					(const std::string vString, const std::string vSubStrFrom, const std::string vSubStrTo);	//��vString�е�����vSubStrFrom��ΪvSubStrTo
	static	std::string		ReplaceBackslashBySlash		(std::string vStr);							//��б��(/)���淴б��(\\)
	static	std::string		Trim						(std::string vStr, std::string vTrimStr);	//ȥ��vStr�е�ָ���ַ�vTrimStr

	static	void			SeparateString				(char* vStr,	const char* vSeps,		vector<std::string>& vRet);			//�����ַ���
	static	void			SeparateStringW				(wchar_t* vStr, const wchar_t* vSeps,	vector<std::wstring>& vRet);

	static	std::string		STRCombine					(char* vFormat,...);

	//C �ַ���
	static	char*			ChStrReplace				(char* vStr, char* vOldStr, char* vNewStr);		//���ַ���vStr�е�vOldStr��ΪvNewStr

	//����ת��
	static	std::string		IToString		(int i);
	static	std::string		I64ToString		(__int64 i);
	static	int				StringToI		(std::string s);

	//����ת��
	static	std::wstring	UTF_8ToUnicode	(const char* vBuf);
	static	std::string		UnicodeToUTF_8	(const wchar_t* vBuf);

	static	std::wstring	ACPToUnicode	(const char* vBuf);
	static	std::string		UnicodeToACP	(const wchar_t* vBuf);

	static	std::string 	ACPToUTF_8		(const char* vBuf);
	static	std::string 	UTF_8ToACP		(const char* vBuf);
	
	//����
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