#include "MF_CommonFunc.h"
//---------------------------------------------------------------------------
bool MF_CommonFunc::IsBigEndian()
{
	//int		a = 0x1234;
	//char	b = *(char*)&a;	//通过将int强制类型转换成char单字节，通过判断起始存储位置。即等于 取b等于a的低地址部分  
	//if(0x12 == b)
	//	return true;

	//return false;


	//联合体union的存放顺序是所有成员都从低地址开始存放，利用该特性可以轻松地获得了CPU对内存采用Little-endian还是Big-endian模式读写
	union NUM  
	{  
		int a;  
		char b;	//b只有一个字节，这里相当于就是a四个字节的低地址，所以看它是高字节还是低字节就能判断了  
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
	setlocale(LC_ALL, "Chinese-simplified");  //汉语显示
	DWORD tLocaleLangId = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

	BOOL tRet = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, vErrorCode, tLocaleLangId, (char*)&tLocaleHandle, 0 , NULL);
	if(FALSE == tRet)
	{
		// 可能是网络错误
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