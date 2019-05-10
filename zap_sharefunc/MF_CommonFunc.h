#ifndef MF_CommonFunc_H
#define MF_CommonFunc_H
//---------------------------------------------------------------------------
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//---------------------------------------------------------------------------
#include <Windows.h>
#include <string>
using namespace std;
//---------------------------------------------------------------------------
namespace MF_CommonFunc
{
	bool			IsBigEndian						(void);	//≈–∂œ¥Û–°∂À

	std::string		TransErrorCode					(DWORD vErrorCode);
};
//---------------------------------------------------------------------------
#endif