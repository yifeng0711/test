#include "MC_RegHelper.h"
//---------------------------------------------------------------------------
bool g_Wow64System = false;
//---------------------------------------------------------------------------
HKEY MC_RegHelper::RegKeyRoot(char* vKeyPath)
{
	if(0 == vKeyPath)		return 0;
	if(0 == *vKeyPath)		return 0;

	char tRootKey[128];
	memset(tRootKey, 0, 128);
	strncpy(tRootKey, vKeyPath, 64);

	char* tPos = strchr(tRootKey, '\\');
	if(0 == tPos)
		return 0;
	else
		*tPos = 0;

	if(0 == _stricmp(tRootKey, "HKEY_CLASSES_ROOT"))		return  HKEY_CLASSES_ROOT;
	if(0 == _stricmp(tRootKey, "HKEY_CURRENT_USER"))		return  HKEY_CURRENT_USER;
	if(0 == _stricmp(tRootKey, "HKEY_LOCAL_MACHINE"))		return  HKEY_LOCAL_MACHINE;
	if(0 == _stricmp(tRootKey, "HKEY_USERS"))				return  HKEY_USERS;

	if(0 == _stricmp(tRootKey, "HKEY_CURRENT_CONFIG"))		return  HKEY_CURRENT_CONFIG;
	if(0 == _stricmp(tRootKey, "HKEY_PERFORMANCE_DATA"))	return  HKEY_PERFORMANCE_DATA;
	if(0 == _stricmp(tRootKey, "HKEY_DYN_DATA"))			return  HKEY_DYN_DATA;

	return 0;
}
//---------------------------------------------------------------------------
bool MC_RegHelper::RegKeyExist(char* vKeyPath)
{
	if(0 == vKeyPath)		return false;
	if(0 == *vKeyPath)		return false;

	HKEY tRootKey = RegKeyRoot(vKeyPath);
	if(0 == tRootKey)
		return false;

	char* tSubKey = strchr(vKeyPath, '\\');
	if(0 == tSubKey)
		return false;
	else
		tSubKey++;

	HKEY tRetKey;
	if(g_Wow64System)
	{
		if(ERROR_SUCCESS != RegOpenKeyExA(tRootKey, tSubKey, 0, KEY_READ | KEY_WOW64_64KEY, &tRetKey))
			return false;
	}
	else
	{
		if(ERROR_SUCCESS != RegOpenKeyExA(tRootKey, tSubKey, 0, KEY_READ, &tRetKey))
			return false;
	}

	RegCloseKey(tRetKey);
	return true;
}
//---------------------------------------------------------------------------
bool MC_RegHelper::RegKeyAdd(char* vKeyPath, bool vVolatile)
{
	if(0 == vKeyPath)		return false;
	if(0 == *vKeyPath)		return false;

	/*if(false != RegKeyExist(vKeyPath))
		return true;*/

	HKEY tRootKey = RegKeyRoot(vKeyPath);
	if(0 == tRootKey)
		return false;

	char* tSubKey = strchr(vKeyPath, '\\');
	if(0 == tSubKey)
		return false;
	else
		tSubKey++;

	DWORD tOption = REG_OPTION_NON_VOLATILE;
	if(true == vVolatile)
		tOption = REG_OPTION_VOLATILE;

	HKEY tRetKey;
	if(g_Wow64System)
	{
		if(ERROR_SUCCESS != RegCreateKeyExA(tRootKey, tSubKey, 0, 0, tOption, KEY_READ | KEY_WOW64_64KEY, 0, &tRetKey, 0))
			return false;
	}
	else
	{
		if(ERROR_SUCCESS != RegCreateKeyExA(tRootKey, tSubKey, 0, 0, tOption, KEY_READ, 0, &tRetKey, 0))
			return false;
	}

	RegCloseKey(tRetKey);
	return true;
}
//---------------------------------------------------------------------------
bool MC_RegHelper::RegKeyDel(char* vKeyPath)
{
	if(0 == vKeyPath)		return false;
	if(0 == *vKeyPath)		return false;

	/*if(false == RegKeyExist(vKeyPath))
		return true;*/

	int tCount = RegKeySubKeyCount(vKeyPath);
	for(int i=tCount-1; i>=0; i--)
	{
		std::string tSubName = RegKeySubKeyName(vKeyPath, i);
		if(tSubName == "")
			return false;

		char tFullName[512];
		strcpy(tFullName, vKeyPath);
		strcat(tFullName, "\\");
		strcat(tFullName, tSubName.c_str());
		if(false == RegKeyDel(tFullName))
			return false;
	}

	HKEY tRootKey = RegKeyRoot(vKeyPath);
	if(0 == tRootKey)
		return false;

	char* tSubKey = strchr(vKeyPath, '\\');
	if(0 == tSubKey)
		return false;
	else
		tSubKey++;

	if(ERROR_SUCCESS != RegDeleteKeyA(tRootKey, tSubKey))
		return false;

	return true;
}
//---------------------------------------------------------------------------
int MC_RegHelper::RegKeySubKeyCount(char* vKeyPath)
{
	if(0 == vKeyPath)		return 0;
	if(0 == *vKeyPath)		return 0;

	HKEY tRootKey = RegKeyRoot(vKeyPath);
	if(0 == tRootKey)
		return 0;

	char* tSubKey = strchr(vKeyPath, '\\');
	if(0 == tSubKey)
		return 0;
	else
		tSubKey++;

	HKEY tRetKey;
	if(g_Wow64System)
	{
		if(ERROR_SUCCESS != RegOpenKeyExA(tRootKey, tSubKey, 0, KEY_READ | KEY_WOW64_64KEY, &tRetKey))
			return 0;
	}
	else
	{
		if(ERROR_SUCCESS != RegOpenKeyExA(tRootKey, tSubKey, 0, KEY_READ, &tRetKey))
			return 0;
	}

	char tKeyName[256];
	int i;
	for(i=0; ; i++)
	{
		DWORD tLen = 256;
		DWORD tResult = RegEnumKeyExA(tRetKey, i, tKeyName, &tLen, 0, 0, 0, 0);

		if(ERROR_SUCCESS == tResult)
			continue;
		if(ERROR_NO_MORE_ITEMS == tResult)
			break;
		else
		{
			i = 0;
			break;
		}
	}

	RegCloseKey(tRetKey);
	return i;
}
//---------------------------------------------------------------------------
std::string MC_RegHelper::RegKeySubKeyName(char* vKeyPath, int vIndex)
{
	if(0 == vKeyPath)		return "";
	if(0 == *vKeyPath)		return "";
	if(0 > vIndex)			return "";

	HKEY tRootKey = RegKeyRoot(vKeyPath);
	if(0 == tRootKey)
		return "";

	char* tSubKey = strchr(vKeyPath, '\\');
	if(0 == tSubKey)
		return "";
	else
		tSubKey++;

	HKEY tRetKey;
	if(g_Wow64System)
	{
		if(ERROR_SUCCESS != RegOpenKeyExA(tRootKey, tSubKey, 0, KEY_READ | KEY_WOW64_64KEY, &tRetKey))
			return "";
	}
	else
	{
		if(ERROR_SUCCESS != RegOpenKeyExA(tRootKey, tSubKey, 0, KEY_READ, &tRetKey))
			return "";
	}

	char tKeyName[256];
	DWORD tLen = 256;
	if(ERROR_SUCCESS != RegEnumKeyExA(tRetKey, vIndex, tKeyName, &tLen, 0, 0, 0, 0))
		tKeyName[0] = 0;

	RegCloseKey(tRetKey);
	return (std::string)tKeyName;
}
//---------------------------------------------------------------------------
int MC_RegHelper::RegValueCount(char* vKeyPath)
{
	if(0 == vKeyPath)		return 0;
	if(0 == *vKeyPath)		return 0;

	HKEY tRootKey = RegKeyRoot(vKeyPath);
	if(0 == tRootKey)
		return 0;

	char* tSubKey = strchr(vKeyPath, '\\');
	if(0 == tSubKey)
		return 0;
	else
		tSubKey++;

	HKEY tRetKey;
	if(g_Wow64System)
	{
		if(ERROR_SUCCESS != RegOpenKeyExA(tRootKey, tSubKey, 0, KEY_READ | KEY_WOW64_64KEY, &tRetKey))
			return 0;
	}
	else
	{
		if(ERROR_SUCCESS != RegOpenKeyExA(tRootKey, tSubKey, 0, KEY_READ, &tRetKey))
			return 0;
	}

	char tValueName[256];
	int i;
	for(i=0; ; i++)
	{
		DWORD tLen = 256;
		DWORD tResult = RegEnumValueA(tRetKey, i, tValueName, &tLen, 0, 0, 0, 0);

		if(ERROR_SUCCESS == tResult)
			continue;
		if(ERROR_NO_MORE_ITEMS == tResult)
			break;
		else
		{
			i = 0;
			break;
		}
	}
	RegCloseKey(tRetKey);
	return i;
}
//---------------------------------------------------------------------------
std::string MC_RegHelper::RegValueName(char* vKeyPath, int vIndex)
{
	if(0 == vKeyPath)		return "";
	if(0 == *vKeyPath)		return "";
	if(0 > vIndex)			return "";

	HKEY tRootKey = RegKeyRoot(vKeyPath);
	if(0 == tRootKey)
		return "";

	char* tSubKey = strchr(vKeyPath, '\\');
	if(0 == tSubKey)
		return "";
	else
		tSubKey++;

	HKEY tRetKey;
	if(g_Wow64System)
	{
		if(ERROR_SUCCESS != RegOpenKeyExA(tRootKey, tSubKey, 0, KEY_READ | KEY_WOW64_64KEY, &tRetKey))
			return "";
	}
	else
	{
		if(ERROR_SUCCESS != RegOpenKeyExA(tRootKey, tSubKey, 0, KEY_READ, &tRetKey))
			return "";
	}

	char tValueName[256];
	DWORD tLen = 256;
	if(ERROR_SUCCESS != RegEnumValueA(tRetKey, vIndex, tValueName, &tLen, 0, 0, 0, 0))
		tValueName[0] = 0;

	RegCloseKey(tRetKey);
	return (std::string)tValueName;
}
//---------------------------------------------------------------------------
bool MC_RegHelper::RegValueExist(char* vKeyPath, char* vValue)
{
	if(0 == vKeyPath)		return false;
	if(0 == *vKeyPath)		return false;

	HKEY tRootKey = RegKeyRoot(vKeyPath);
	if(0 == tRootKey)
		return false;

	char* tSubKey = strchr(vKeyPath, '\\');
	if(0 == tSubKey)
		return false;
	else
		tSubKey++;

	HKEY tRetKey;
	if(g_Wow64System)
	{
		if(ERROR_SUCCESS != RegOpenKeyExA(tRootKey, tSubKey, 0, KEY_READ | KEY_WOW64_64KEY, &tRetKey))
			return false;
	}
	else
	{
		if(ERROR_SUCCESS != RegOpenKeyExA(tRootKey, tSubKey, 0, KEY_READ, &tRetKey))
			return false;

	}

	if(ERROR_SUCCESS != RegQueryValueExA(tRetKey, vValue, 0, 0, 0, 0))
	{
		RegCloseKey(tRetKey);
		return false;
	}

	RegCloseKey(tRetKey);
	return true;
}
//---------------------------------------------------------------------------
bool MC_RegHelper::RegValueSet(char* vKeyPath, char* vValue, char* vData, int vType, int vSize)
{
	if(0 == vKeyPath)		return false;
	if(0 == *vKeyPath)		return false;
	if(0 == vData)			return false;

	HKEY tRootKey = RegKeyRoot(vKeyPath);
	if(0 == tRootKey)
		return false;

	char* tSubKey = strchr(vKeyPath, '\\');
	if(0 == tSubKey)
		return false;
	else
		tSubKey++;

	DWORD tSize = 0;
	switch(vType)
	{
	case REG_SZ:
	case REG_EXPAND_SZ:
		tSize = (int)strlen(vData)+1;
		break;

	case REG_MULTI_SZ:
		while(true)
		{
			if((0 == vData[tSize]) && (0 == vData[tSize+1]))
				break;

			tSize++;
		}

		tSize++;
		tSize++;
		break;

	case REG_BINARY:
		tSize = vSize;
		break;

	default:
		return false;
	}

	HKEY tRetKey;
	if(g_Wow64System)
	{
		if(ERROR_SUCCESS != RegCreateKeyExA(tRootKey, tSubKey, 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_WOW64_64KEY, 0, &tRetKey, 0))
			return false;
	}
	else
	{
		if(ERROR_SUCCESS != RegCreateKeyExA(tRootKey, tSubKey, 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &tRetKey, 0))
			return false;
	}

	if(ERROR_SUCCESS != RegSetValueExA(tRetKey, vValue, 0, vType, (BYTE*)vData, tSize))
	{
		RegCloseKey(tRetKey);
		return false;
	}

	RegCloseKey(tRetKey);
	return true;
}
//---------------------------------------------------------------------------
bool MC_RegHelper::RegValueSet(char* vKeyPath, char* vValue, DWORD vData)
//HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer
{
	if(0 == vKeyPath)		return false;
	if(0 == *vKeyPath)		return false;

	HKEY tRootKey = RegKeyRoot(vKeyPath);
	if(0 == tRootKey)
		return false;

	char temp[256] = {0};
	char* tSubKey = strchr(vKeyPath, '\\');
	if(0 == tSubKey)
		return false;
	else
		tSubKey++;


	HKEY tRetKey;
	if(g_Wow64System)
	{
		if(ERROR_SUCCESS != RegCreateKeyExA(tRootKey, tSubKey, 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_WOW64_64KEY, 0, &tRetKey, 0))	
			return false;
	}
	else
	{
		if(ERROR_SUCCESS != RegCreateKeyExA(tRootKey, tSubKey, 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &tRetKey, 0))	
			return false;
	}


	if(ERROR_SUCCESS != RegSetValueExA(tRetKey, vValue, 0, REG_DWORD, (BYTE*)&vData, sizeof(DWORD)))
	{
		RegCloseKey(tRetKey);
		return false;
	}

	RegCloseKey(tRetKey);
	return true;
}
//---------------------------------------------------------------------------
std::string MC_RegHelper::RegValueGet(char* vKeyPath, char* vValue)
{
	if(0 == vKeyPath)		return false;
	if(0 == *vKeyPath)		return false;

	HKEY tRootKey = RegKeyRoot(vKeyPath);
	if(0 == tRootKey)
		return "";

	char* tSubKey = strchr(vKeyPath, '\\');
	if(0 == tSubKey)
		return "";
	else
		tSubKey++;

	HKEY tRetKey;
	if(g_Wow64System)
	{
		if(ERROR_SUCCESS != RegOpenKeyExA(tRootKey, tSubKey, 0, KEY_READ | KEY_WOW64_64KEY, &tRetKey))
			return "";
	}
	else
	{
		if(ERROR_SUCCESS != RegOpenKeyExA(tRootKey, tSubKey, 0, KEY_READ, &tRetKey))
			return "";

	}

	DWORD tType;
	DWORD tSize;
	if(ERROR_SUCCESS != RegQueryValueExA(tRetKey, vValue, 0, &tType, 0, &tSize))
	{
		RegCloseKey(tRetKey);
		return "";
	}

	std::string tRetVal = "";
	switch(tType)
	{
	case REG_DWORD:
		{
			DWORD tData = 0;
			if(ERROR_SUCCESS != RegQueryValueExA(tRetKey, vValue, 0, &tType, (BYTE*)&tData, &tSize))
				break;

			tRetVal.resize(16);
			wsprintfA((char*)tRetVal.data(), "%d", tData);
			break;
		}

	case REG_SZ:
	case REG_EXPAND_SZ:
	case REG_MULTI_SZ:
	case REG_BINARY:
		{
			tRetVal.resize(tSize);
			if(ERROR_SUCCESS != RegQueryValueExA(tRetKey, vValue, 0, &tType, (BYTE*)tRetVal.data(), &tSize))
				tRetVal = "";

			break;
		}

	default:
		break;
	}

	RegCloseKey(tRetKey);
	return tRetVal;
}
//---------------------------------------------------------------------------
bool MC_RegHelper::RegValueDel(char* vKeyPath, char* vValue)
{
	if(0 == vKeyPath)		return false;
	if(0 == *vKeyPath)		return false;

	HKEY tRootKey = RegKeyRoot(vKeyPath);
	if(0 == tRootKey)
		return false;

	char* tSubKey = strchr(vKeyPath, '\\');
	if(0 == tSubKey)
		return false;
	else
		tSubKey++;

	HKEY tRetKey;
	if(g_Wow64System)
	{
		if(ERROR_SUCCESS != RegOpenKeyExA(tRootKey, tSubKey, 0, KEY_WRITE | KEY_WOW64_64KEY, &tRetKey))
			return false;
	}
	else
	{
		if(ERROR_SUCCESS != RegOpenKeyExA(tRootKey, tSubKey, 0, KEY_WRITE, &tRetKey))
			return false;
	}

	if(ERROR_SUCCESS != RegDeleteValueA(tRetKey, vValue))
	{
		RegCloseKey(tRetKey);
		return false;
	}

	RegCloseKey(tRetKey);
	return true;
}
//---------------------------------------------------------------------------