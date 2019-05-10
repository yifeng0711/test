#ifndef MC_RegHelper_H
#define MC_RegHelper_H
//---------------------------------------------------------------------------
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//---------------------------------------------------------------------------
#include <Windows.h>
#include <string>
using namespace std;
//---------------------------------------------------------------------------
class MC_RegHelper
{
	//访问HKLM/Software，会被WOW64(win32 on win64)重定向至HKLM/Software/Wow6432Node
	//KEY_WOW64_64KEY 使32位程序能够直接访问64位注册表，而不会重定向到WoW6432Node
	//Window2000 不支持KEY_WOW64_64KEY

public:
	static	HKEY			RegKeyRoot			(char* vKeyPath);

	static	bool			RegKeyExist			(char* vKeyPath);
	static	bool			RegKeyAdd			(char* vKeyPath, bool vVolatile);	//vVolatile true表示临时注册表，重启后消失
	static	bool			RegKeyDel			(char* vKeyPath);

	static	int				RegKeySubKeyCount	(char* vKeyPath);				//vKeyPath 下子项的数量
	static	std::string		RegKeySubKeyName	(char* vKeyPath, int vIndex);	//vKeyPath 下第n个子项的名称，从0开始

	static	int				RegValueCount		(char* vKeyPath);				//vKeyPath 下子键的数量
	static	std::string		RegValueName		(char* vKeyPath, int vIndex);	//vKeyPath 下第n个子键的名称，从0开始

	static	bool			RegValueExist		(char* vKeyPath, char* vValue);
	static	bool			RegValueSet			(char* vKeyPath, char* vValue, char* vData, int vType, int vSize);	//在vKeyPath 下设置名为vValue的子键，类型为vType
	static	bool			RegValueSet			(char* vKeyPath, char* vValue, DWORD vData);						//在vKeyPath 下设置名为vValue的子键，类型为DWORD
	static	std::string		RegValueGet			(char* vKeyPath, char* vValue);										//在vKeyPath 下获取名为vValue的子键的值
	static	bool			RegValueDel			(char* vKeyPath, char* vValue);										//在vKeyPath 下获取名为vValue的子键
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
/*
#define ROOR_REG_PATH	"HKEY_CURRENT_USER\\Software\\MyTest"
#define SUB_REG_PATH_1	"HKEY_CURRENT_USER\\Software\\MyTest\\test_1"
#define SUB_REG_PATH_2	"HKEY_CURRENT_USER\\Software\\MyTest\\test_2"
#define SUB_REG_PATH_3	"HKEY_CURRENT_USER\\Software\\MyTest\\test_3"

if(false == MC_RegHelper::RegKeyAdd(SUB_REG_PATH_1, false))
cout << "add path 1 fail" << endl;
if(false == MC_RegHelper::RegKeyAdd(SUB_REG_PATH_2, true))
cout << "add path 2 fail" << endl;
if(false == MC_RegHelper::RegKeyAdd(SUB_REG_PATH_3, false))
cout << "add path 3 fail" << endl;

if(false == MC_RegHelper::RegKeyDel(SUB_REG_PATH_3))	
cout << "del path 3 fail" << endl;

cout << MC_RegHelper::RegKeySubKeyCount(ROOR_REG_PATH) << endl;		//ROOR_REG_PATH 下子项的数量
cout << MC_RegHelper::RegKeySubKeyName(ROOR_REG_PATH, 0) << endl;	//ROOR_REG_PATH 下第一个子项的名称

cout << MC_RegHelper::RegValueCount(ROOR_REG_PATH) << endl;			//ROOR_REG_PATH 下子键的数量
cout << MC_RegHelper::RegValueName(ROOR_REG_PATH, 1) << endl;		//ROOR_REG_PATH 下第一个子键的值


if(false == MC_RegHelper::RegValueSet(SUB_REG_PATH_1, "sub.1.int", 123))
cout << "set sub.1.int fail" << endl;

if(false == MC_RegHelper::RegValueSet(SUB_REG_PATH_1, "sub.1.str", "asd", REG_SZ, strlen("asd")))
cout << "set sub.1.str fail" << endl;

cout << MC_RegHelper::RegValueGet(SUB_REG_PATH_1, "sub.1.str") << endl;

if(false == MC_RegHelper::RegValueDel(SUB_REG_PATH_1, "sub.1.int"))
cout << "del sub.1.int fail" << endl;

*/

/*
cout << MC_RegHelper::RegKeyAdd("HKEY_LOCAL_MACHINE\\Software\\MyTest", false) << endl;	//被重定向到	HKEY_LOCAL_MACHINE\\Software\\Wow6432Node\\MyTest
*/