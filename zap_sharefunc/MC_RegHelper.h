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
	//����HKLM/Software���ᱻWOW64(win32 on win64)�ض�����HKLM/Software/Wow6432Node
	//KEY_WOW64_64KEY ʹ32λ�����ܹ�ֱ�ӷ���64λע����������ض���WoW6432Node
	//Window2000 ��֧��KEY_WOW64_64KEY

public:
	static	HKEY			RegKeyRoot			(char* vKeyPath);

	static	bool			RegKeyExist			(char* vKeyPath);
	static	bool			RegKeyAdd			(char* vKeyPath, bool vVolatile);	//vVolatile true��ʾ��ʱע�����������ʧ
	static	bool			RegKeyDel			(char* vKeyPath);

	static	int				RegKeySubKeyCount	(char* vKeyPath);				//vKeyPath �����������
	static	std::string		RegKeySubKeyName	(char* vKeyPath, int vIndex);	//vKeyPath �µ�n����������ƣ���0��ʼ

	static	int				RegValueCount		(char* vKeyPath);				//vKeyPath ���Ӽ�������
	static	std::string		RegValueName		(char* vKeyPath, int vIndex);	//vKeyPath �µ�n���Ӽ������ƣ���0��ʼ

	static	bool			RegValueExist		(char* vKeyPath, char* vValue);
	static	bool			RegValueSet			(char* vKeyPath, char* vValue, char* vData, int vType, int vSize);	//��vKeyPath ��������ΪvValue���Ӽ�������ΪvType
	static	bool			RegValueSet			(char* vKeyPath, char* vValue, DWORD vData);						//��vKeyPath ��������ΪvValue���Ӽ�������ΪDWORD
	static	std::string		RegValueGet			(char* vKeyPath, char* vValue);										//��vKeyPath �»�ȡ��ΪvValue���Ӽ���ֵ
	static	bool			RegValueDel			(char* vKeyPath, char* vValue);										//��vKeyPath �»�ȡ��ΪvValue���Ӽ�
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

cout << MC_RegHelper::RegKeySubKeyCount(ROOR_REG_PATH) << endl;		//ROOR_REG_PATH �����������
cout << MC_RegHelper::RegKeySubKeyName(ROOR_REG_PATH, 0) << endl;	//ROOR_REG_PATH �µ�һ�����������

cout << MC_RegHelper::RegValueCount(ROOR_REG_PATH) << endl;			//ROOR_REG_PATH ���Ӽ�������
cout << MC_RegHelper::RegValueName(ROOR_REG_PATH, 1) << endl;		//ROOR_REG_PATH �µ�һ���Ӽ���ֵ


if(false == MC_RegHelper::RegValueSet(SUB_REG_PATH_1, "sub.1.int", 123))
cout << "set sub.1.int fail" << endl;

if(false == MC_RegHelper::RegValueSet(SUB_REG_PATH_1, "sub.1.str", "asd", REG_SZ, strlen("asd")))
cout << "set sub.1.str fail" << endl;

cout << MC_RegHelper::RegValueGet(SUB_REG_PATH_1, "sub.1.str") << endl;

if(false == MC_RegHelper::RegValueDel(SUB_REG_PATH_1, "sub.1.int"))
cout << "del sub.1.int fail" << endl;

*/

/*
cout << MC_RegHelper::RegKeyAdd("HKEY_LOCAL_MACHINE\\Software\\MyTest", false) << endl;	//���ض���	HKEY_LOCAL_MACHINE\\Software\\Wow6432Node\\MyTest
*/