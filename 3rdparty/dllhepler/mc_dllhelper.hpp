//--------------------------------------------------
#ifndef MC_DLLHELPER_H
#define MC_DLLHELPER_H
//--------------------------------------------------
#include "md_basedefine.h"
//--------------------------------------------------
#ifdef WINDOWS
	#include <windows.h>
	#define EXPORT_FUNC extern "C" __declspec( dllexport )
	#define XLoadLibrary(_X_)	LoadLibraryA(_X_)
	#define XGetProcAddress		GetProcAddress
	#define XFreeLibrary		FreeLibrary
#endif

#ifdef __linux__
	#include <dlfcn.h>
	#include <unistd.h>
	#include <linux/limits.h>
    #define __cdecl
	typedef void*			HMODULE;
	#define EXPORT_FUNC extern "C" __attribute ((visibility("default")))
	#define XLoadLibrary(_X_)	dlopen(_X_, RTLD_LAZY);
	#define XGetProcAddress		dlsym
	#define XFreeLibrary		dlclose
#endif
//--------------------------------------------------
#define Kinkoo_InterfaceExport(_X_)                                                         \
    EXPORT_FUNC bool __cdecl Kinkoo_GetInterface(void** IPtr)                               \
    {                                                                                       \
        if(0 == IPtr)   return false;                                                       \
        *IPtr   = new _X_;                                                                  \
        return true;                                                                        \
    }																						\
	EXPORT_FUNC bool __cdecl Kinkoo_FreeInterface(void* IPtr)         	                    \
	{                                                                                       \
        if(0 == IPtr)   return false;                                                       \
        delete (_X_*)IPtr;																	\
        return true;                                                                        \
    }																						\
//--------------------------------------------------
typedef bool (__cdecl *FuncKinkoo_GetInterface)(void** IPtr);
typedef bool (__cdecl *FuncKinkoo_FreeInterface)(void* IPtr);
//--------------------------------------------------
template<class Interface_Type>
class mc_dllhelper
{
public:
    mc_dllhelper();
    
    bool    load    (const char* vDllName);
    bool    free    (void);
    
    Interface_Type *operator->() const
    {
        return m_Interface;
    }
    
    Interface_Type*     m_Interface;
    HMODULE             m_Instance;
    bool                m_Avaliable;
};
//--------------------------------------------------
template<class Interface_Type>
mc_dllhelper<Interface_Type>::mc_dllhelper()
{
    m_Interface     = 0;
    m_Instance      = 0;
    m_Avaliable     = false;
}
//--------------------------------------------------
template<class Interface_Type>
bool mc_dllhelper<Interface_Type>::load(const char* vDllName)
{
    if(true == m_Avaliable)
        return true;        

    m_Instance = XLoadLibrary(vDllName);
	if(0 == m_Instance)
	{
#ifdef __linux__
        printf("XLoadLibrary fail(%s)\n", dlerror());
#endif
#ifdef _WIN32
        char tText[1024] = {0};
		sprintf_s(tText,sizeof(tText), "XLoadLibrary fail(%d),Path=%s\n", GetLastError(),vDllName);
        OutputDebugStringA(tText);
#endif

		return false;
	}

	FuncKinkoo_GetInterface tFunc = (FuncKinkoo_GetInterface)XGetProcAddress(m_Instance, "Kinkoo_GetInterface");

	if(0 == tFunc)
	{
		XFreeLibrary(m_Instance);

        printf("Kinkoo_GetInterface not exit.\n");
        return false;
	}

	tFunc((void**)&m_Interface);
    if(0 == m_Interface)
    {
        XFreeLibrary(m_Instance);

        printf("get interface fail.\n");
        return false;
    }

    m_Avaliable = true;
    return true;    
}
//--------------------------------------------------
template<class Interface_Type>
bool mc_dllhelper<Interface_Type>::free(void)
{
    if(false == m_Avaliable)
        return true;
	
    FuncKinkoo_FreeInterface tFunc = (FuncKinkoo_FreeInterface)XGetProcAddress(m_Instance, "Kinkoo_FreeInterface");
	tFunc(m_Interface);
	
    if(0 != m_Instance)
        XFreeLibrary(m_Instance);
        
    m_Interface     = 0;
    m_Instance      = 0;
    m_Avaliable     = false;
    return true;
}
//--------------------------------------------------
#endif
