//---------------------------------------------------------------------------
#ifndef WinMain_H
#define WinMain_H
//---------------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN
//---------------------------------------------------------------------------
#include <Windows.h>
#include <iostream>
using namespace std;
//---------------------------------------------------------------------------
#define MyPrintf(_X_)	{cout << _X_ << endl;}
#define COUT_PAUSE(_X_)	{cout << _X_ << endl; system("pause");}

#ifndef SafeDelArrayPoint
#define SafeDelArrayPoint(_X_)	{ if(0 != _X_)	{delete[] _X_; _X_ = 0;} }
#endif

typedef struct  FileData_tag
{
	unsigned char* data;
	int size;

	FileData_tag()
	{
		data = 0;
		size = 0;
	}

	~FileData_tag()
	{
		Release();
	}

	void Release(void)
	{
		if(0 != data)
		{
			delete[] data;
			data = 0;
		}
		size = 0;
	}

}FILEDATA_tag;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif