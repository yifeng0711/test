#ifndef MC_Md5Test_H
#define MC_Md5Test_H
//---------------------------------------------------------------------------
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//---------------------------------------------------------------------------
#include <string>
using namespace std;
//---------------------------------------------------------------------------
class MC_Md5Test
{
public:
	static	string		GetStrMD5	(const string vString);
	static	string		GetFileMD5	(const string vFileName);
};
//---------------------------------------------------------------------------
#endif