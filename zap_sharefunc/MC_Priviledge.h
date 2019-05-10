#ifndef MC_Priviledge_H
#define MC_Priviledge_H
//---------------------------------------------------------------------------
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//---------------------------------------------------------------------------
#include <Windows.h>
//---------------------------------------------------------------------------
class MC_Priviledge
{
public:
	/**
	 * @Method		:	EnablePriviledge
	 * @brief		:	将当前程序提升为debug权限
	 */
	static	BOOL	EnablePriviledge				();										//权限提升,eg:EnablePriviledge(SE_DEBUG_NAME)

	/**
	 * @Method		:	EnableFileAccountPrivilege
	 * @brief		:	修改用户的文件夹操作权限
	 * @param[in]	:	vDestPath	目标文件夹，不能为空
	 * @param[in]	:	vAccount	账号名，不能为空
	 * @return		:	
	 */
	static	BOOL	EnableFileAccountPrivilege		(const char* vDestPath, const char* vAccount, const DWORD vAccess=GENERIC_ALL);


	static	BOOL	GetProcessPriviledge			(HANDLE vProcess);
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
/*
通用访问权限定义的常量
常量				解释
GENERIC_ALL			读、写和执行访问
GENERIC_EXECUTE		执行
GENERIC_READ		读
GENERIC_WRITE		写
 */




