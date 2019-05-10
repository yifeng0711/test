#include "MF_CommonFunc.h"
//---------------------------------------------------------------------------
bool MF_CommonFunc::IsBigEndian()
{
	//int		a = 0x1234;
	//char	b = *(char*)&a;	//ͨ����intǿ������ת����char���ֽڣ�ͨ���ж���ʼ�洢λ�á������� ȡb����a�ĵ͵�ַ����  
	//if(0x12 == b)
	//	return true;

	//return false;


	//������union�Ĵ��˳�������г�Ա���ӵ͵�ַ��ʼ��ţ����ø����Կ������ɵػ����CPU���ڴ����Little-endian����Big-endianģʽ��д
	union NUM  
	{  
		int a;  
		char b;	//bֻ��һ���ֽڣ������൱�ھ���a�ĸ��ֽڵĵ͵�ַ�����Կ����Ǹ��ֽڻ��ǵ��ֽھ����ж���  
	}num;  

	num.a = 0x1234;  
	if( num.b == 0x12 )  
		return true;

	return false;

};
//---------------------------------------------------------------------------
std::string MF_CommonFunc::TransErrorCode(DWORD vErrorCode)
{
	HANDLE	tLocaleHandle	= NULL;
	setlocale(LC_ALL, "Chinese-simplified");  //������ʾ
	DWORD tLocaleLangId = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

	BOOL tRet = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, vErrorCode, tLocaleLangId, (char*)&tLocaleHandle, 0 , NULL);
	if(FALSE == tRet)
	{
		// �������������
		HMODULE tDll = LoadLibraryExA("netmsg.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
		if (NULL != tDll)
		{
			tRet = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, tDll, vErrorCode, tLocaleLangId, (char*)&tLocaleHandle, 0 , NULL);
			FreeLibrary(tDll);
		}
	}

	std::string tMsg = "";
	if(FALSE != tRet && NULL != tLocaleHandle)
	{
		tMsg = (char*)LocalLock(tLocaleHandle);

		LocalFree(tLocaleHandle);
		tLocaleHandle = NULL;
	}

	return tMsg;
}
//---------------------------------------------------------------------------