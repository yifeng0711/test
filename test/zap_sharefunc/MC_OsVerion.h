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
	 * @brief		:	通过注册表，获取系统版本信息
	 * @other[1]	:	必须以管理员权限运行，否则出错
	 * @other[2]	:	当程序设置以兼容模式运行时，返回的是兼容模式的系统版本，不是当前系统版本
	 */
	static	std::string		GetOsNameByReg				(void);

	/**
	 * @Method		:	GetOsNameByApi
	 * @brief		:	通过GetVersionEx，获取系统版本信息
	 * @other[1]	:	GetVersionEx从Win8.1发布后就被废弃，如果想获取到正确的版本信息，
	 					需要在配置属性->清单工具->输入和输出-附加清单文件 添加兼容文件：compatibility.xml
	 					否则Win8以上无法获取到正确版本信息
	 * @other[2]	:	当程序设置以兼容模式运行时，返回的是兼容模式的系统版本，不是当前系统版本
	 */
	static	std::string		GetOsNameByApi				(void);


	static	bool			IsWin7OS					(void);	//不规范
	static	BOOL			Is64System					(void);
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
/*
 * 系统版本号：https://docs.microsoft.com/zh-cn/windows/desktop/SysInfo/operating-system-version

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