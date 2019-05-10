#include "MC_Priviledge.h"
#include "MC_Log.h"

#include <AccCtrl.h>
#include <Aclapi.h>
//---------------------------------------------------------------------------
#define	SafeCloseHHandle(_X_)	{if(0 != _X_) {CloseHandle(_X_); _X_= 0;}}
//---------------------------------------------------------------------------
BOOL MC_Priviledge::EnablePriviledge()
{
	BOOL				tRet		= FALSE;	
	HANDLE				tToken		= NULL;
	LUID				tLuid;
	TOKEN_PRIVILEGES	tTokenPriv;
	ZeroMemory(&tTokenPriv, sizeof(TOKEN_PRIVILEGES));

	tRet = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &tToken);
	if (FALSE == tRet) 
	{
		if (GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)	//系统不支持该功能	
		{
			SetLastError(ERROR_SUCCESS);
			SafeCloseHHandle(tToken);
			return TRUE;
		}

		LOG_ERROR("OpenProcessToken fail! errno: %d", GetLastError());
		SafeCloseHHandle(tToken);
		return FALSE;
	}

	tRet = LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tLuid);
	if (FALSE == tRet)
	{
		LOG_ERROR("LookupPrivilegeValue fail! errno: %d", GetLastError());
		SafeCloseHHandle(tToken);
		return FALSE;
	}
	tTokenPriv.PrivilegeCount				= 1;
	tTokenPriv.Privileges[0].Luid			= tLuid;
	tTokenPriv.Privileges[0].Attributes		= SE_PRIVILEGE_ENABLED;

	//1300: 进程没有该权限
	tRet = AdjustTokenPrivileges(tToken, FALSE, &tTokenPriv, 0, NULL, 0);
	if(FALSE == tRet)
	{
		LOG_ERROR("AdjustTokenPrivileges fail! errno: %d", GetLastError());
		SafeCloseHHandle(tToken);
		return FALSE;
	}

	SafeCloseHHandle(tToken);
	return TRUE;
}
//---------------------------------------------------------------------------
BOOL MC_Priviledge::EnableFileAccountPrivilege(const char* vDestPath, const char* vAccount, const DWORD vAccess/* =GENERIC_ALL */)
{
	if(0 == vDestPath)		return FALSE;
	if(0 == *vDestPath)		return FALSE;
	if(0 == vAccount)		return FALSE;
	if(0 == *vAccount)		return FALSE;

	BOOL				tSuccess	= TRUE;
	PACL				tOldDacl	= NULL;
	PACL				tNewDacl	= NULL;
	EXPLICIT_ACCESSA	tEa;

	do
	{
		// 获取文件(夹)安全对象的DACL列表(自由访问控制列表)
		if(ERROR_SUCCESS != GetNamedSecurityInfoA((LPSTR)vDestPath, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &tOldDacl, NULL, NULL))
		{
			tSuccess =FALSE;
			break;
		}

		// 此处不可直接用AddAccessAllowedAce函数,因为已有的DACL长度是固定,必须重新创建一个DACL对象
		// 生成指定用户帐户的访问控制信息(这里指定赋予全部的访问权限)
		BuildExplicitAccessWithNameA(&tEa, (LPSTR)vAccount, vAccess, GRANT_ACCESS, SUB_CONTAINERS_AND_OBJECTS_INHERIT);

		// 创建新的ACL对象(合并已有的ACL对象和刚生成的用户帐户访问控制信息)
		if(ERROR_SUCCESS != SetEntriesInAclA(1, &tEa, tOldDacl, &tNewDacl))
		{
			tSuccess =FALSE;
			break;
		}

		// 设置文件(夹)安全对象的DACL列表
		if (ERROR_SUCCESS != SetNamedSecurityInfoA((LPSTR)vDestPath, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, tNewDacl, NULL))
		{
			tSuccess = FALSE;
			break;
		}
	}while(FALSE);

	if (NULL != tNewDacl)
	{
		LocalFree(tNewDacl);
	}

	return tSuccess;
}
//---------------------------------------------------------------------------
BOOL MC_Priviledge::GetProcessPriviledge(HANDLE hProcess)
{
	const int tMaxName = 256;

	DWORD i, dwSize = 0, dwResult = 0;
	HANDLE hToken;
	PTOKEN_GROUPS pGroupInfo;
	PSID pSID = NULL;
	SID_IDENTIFIER_AUTHORITY SIDAuth = SECURITY_NT_AUTHORITY;
	char lpName[tMaxName];
	char lpDomain[tMaxName];
	SID_NAME_USE SidType;

	// Open a handle to the access token for the calling process.
	//TOKEN_QUERY:Required to query an access token.
	//GetCurrentProcess()返回进程句柄
	//[out]hToken是access token的句柄
	if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
	{
		printf("OpenProcessToken Error %u\n", GetLastError());
		return FALSE;
	}

	//前后两次调用GetTokenInformation的目的不同
	// Call GetTokenInformation to get the buffer size.
	//The TOKEN_GROUPS structure contains information about the group security identifiers (SIDs) in an access token.
	if (!GetTokenInformation(hToken, TokenGroups, NULL, dwSize, &dwSize))
	{
		dwResult = GetLastError();
		if (dwResult != ERROR_INSUFFICIENT_BUFFER) {
			printf("GetTokenInformation Error %u\n", dwResult);
			return FALSE;
		}
	}

	// Allocate the buffer.
	pGroupInfo = (PTOKEN_GROUPS)GlobalAlloc(GPTR, dwSize);

	// Call GetTokenInformation again to get the group information.

	if (!GetTokenInformation(hToken, TokenGroups, pGroupInfo,
		dwSize, &dwSize))
	{
		printf("GetTokenInformation Error %u\n", GetLastError());
		return FALSE;
	}

	// Create a SID for the BUILTIN\Administrators group.

	if (!AllocateAndInitializeSid(&SIDAuth, 2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&pSID))
	{
		printf("AllocateAndInitializeSid Error %u\n", GetLastError());
		return FALSE;
	}
	// Loop through the group SIDs looking for the administrator SID.
	//
	for (i = 0; i < pGroupInfo->GroupCount; i++)
	{
		if (EqualSid(pSID, pGroupInfo->Groups[i].Sid))
		{

			// Lookup the account name and print it.

			dwSize = tMaxName;
			if (!LookupAccountSidA(NULL, pGroupInfo->Groups[i].Sid,
				lpName, &dwSize, lpDomain,
				&dwSize, &SidType))
			{
				dwResult = GetLastError();
				if (dwResult == ERROR_NONE_MAPPED)
					strcpy_s(lpName, dwSize, "NONE_MAPPED");
				else
				{
					printf("LookupAccountSid Error %u\n", GetLastError());
					return FALSE;
				}
			}

			printf("Current user is a member of the %s\\%s group\n",
				lpDomain, lpName);

			// Find out whether the SID is enabled in the token.
			if (pGroupInfo->Groups[i].Attributes & SE_GROUP_ENABLED)
				printf("The group SID is enabled.\n");
			else if (pGroupInfo->Groups[i].Attributes &
				SE_GROUP_USE_FOR_DENY_ONLY)
				printf("The group SID is a deny-only SID.\n");
			else
				printf("The group SID is not enabled.\n");
		}
	}
	if (pSID)
		FreeSid(pSID);
	if (pGroupInfo)
		GlobalFree(pGroupInfo);

	return TRUE;
}
//---------------------------------------------------------------------------
