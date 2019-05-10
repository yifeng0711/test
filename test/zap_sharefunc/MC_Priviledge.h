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
	 * @brief		:	����ǰ��������ΪdebugȨ��
	 */
	static	BOOL	EnablePriviledge				();										//Ȩ������,eg:EnablePriviledge(SE_DEBUG_NAME)

	/**
	 * @Method		:	EnableFileAccountPrivilege
	 * @brief		:	�޸��û����ļ��в���Ȩ��
	 * @param[in]	:	vDestPath	Ŀ���ļ��У�����Ϊ��
	 * @param[in]	:	vAccount	�˺���������Ϊ��
	 * @return		:	
	 */
	static	BOOL	EnableFileAccountPrivilege		(const char* vDestPath, const char* vAccount, const DWORD vAccess=GENERIC_ALL);


	static	BOOL	GetProcessPriviledge			(HANDLE vProcess);
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
/*
ͨ�÷���Ȩ�޶���ĳ���
����				����
GENERIC_ALL			����д��ִ�з���
GENERIC_EXECUTE		ִ��
GENERIC_READ		��
GENERIC_WRITE		д
 */




