#ifndef MC_OsVerion_H
#define MC_OsVerion_H
//---------------------------------------------------------------------------
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//---------------------------------------------------------------------------
#include <Windows.h>
#include <string>
using namespace std;
//---------------------------------------------------------------------------
class MC_OsVerion
{
public:
	/**
	 * @Method		:	GetOsNameByReg
	 * @brief		:	ͨ��ע�����ȡϵͳ�汾��Ϣ
	 * @other[1]	:	�����Թ���ԱȨ�����У��������
	 * @other[2]	:	�����������Լ���ģʽ����ʱ�����ص��Ǽ���ģʽ��ϵͳ�汾�����ǵ�ǰϵͳ�汾
	 */
	static	std::string		GetOsNameByReg				(void);

	/**
	 * @Method		:	GetOsNameByApi
	 * @brief		:	ͨ��GetVersionEx����ȡϵͳ�汾��Ϣ
	 * @other[1]	:	GetVersionEx��Win8.1������ͱ�������������ȡ����ȷ�İ汾��Ϣ��
	 					��Ҫ����������->�嵥����->��������-�����嵥�ļ� ��Ӽ����ļ���compatibility.xml
	 					����Win8�����޷���ȡ����ȷ�汾��Ϣ
	 * @other[2]	:	�����������Լ���ģʽ����ʱ�����ص��Ǽ���ģʽ��ϵͳ�汾�����ǵ�ǰϵͳ�汾
	 */
	static	std::string		GetOsNameByApi				(void);


	static	bool			IsWin7OS					(void);	//���淶
	static	BOOL			Is64System					(void);
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
/*
 * ϵͳ�汾�ţ�https://docs.microsoft.com/zh-cn/windows/desktop/SysInfo/operating-system-version

 Operating system			Version number
 Windows 10					10.0*
 Windows Server 2019		10.0*
 Windows Server 2016		10.0*
 Windows 8.1				6.3*
 Windows Server 2012 R2		6.3*
 Windows 8					6.2
 Windows Server 2012		6.2
 Windows 7					6.1
 Windows Server 2008 R2		6.1
 Windows Server 2008		6.0
 Windows Vista				6.0
 Windows Server 2003 R2		5.2
 Windows Server 2003		5.2
 Windows XP 64-Bit Edition	5.2
 Windows XP					5.1
 Windows 2000				5.0
 */