//---------------------------------------------------------------------------
#ifndef MD_BASEDEFINE_H
#define MD_BASEDEFINE_H
//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string>
//---------------------------------------------------------------------------
#if (defined WIN32) || (defined _WIN64)
    #define WINDOWS
    #include <windows.h>
#endif

#ifdef __linux__
    typedef void*        HANDLE;
    typedef void*        HMODULE;
    #define __cdecl
#endif
//---------------------------------------------------------------------------
#ifndef KKS
	typedef std::string        KKS;
#endif
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
