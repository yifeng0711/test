//---------------------------------------------------------------------------
#ifndef MF_KKCOMMONFUNH
#define MF_KKCOMMONFUNH
//---------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include "md_basedefine.h"
#include <string.h>
//---------------------------------------------------------------------------
inline std::string module_path_name(const char* name = 0, HMODULE module = 0)
{
    char path[512] = {0};
#ifdef	__linux__
    if(0 == readlink("/proc/self/exe", path, 512))
        return "";

    char* pos = strrchr(path, '/');
#endif
#ifdef WINDOWS
    GetModuleFileNameA(module, path, 512);
    char* pos = strrchr(path, '\\');
#endif
    
    if(0 == pos)
        return "";

    *(pos+1) = 0;

    if(0 != name)
        strcat_s(path, 512, name);

    return path;
}
//---------------------------------------------------------------------------
#ifdef WINDOWS
#include <Tlhelp32.h>
inline bool LoadAppAndWait(const wchar_t* vCmdLine, int vShowMode, DWORD vTimeOut)
{
    STARTUPINFOW stStartUpInfo;
    memset(&stStartUpInfo, 0, sizeof(stStartUpInfo));
    stStartUpInfo.cb            = sizeof(stStartUpInfo);
    stStartUpInfo.dwFlags       = STARTF_USESHOWWINDOW;
    stStartUpInfo.wShowWindow   = vShowMode;

    PROCESS_INFORMATION stProcessInfo;
	
	wchar_t tCmdTmp[1024] = {0};	
	wsprintf(tCmdTmp, L"%s", vCmdLine);
   if(FALSE == CreateProcessW(0, tCmdTmp, 0, 0, 0, NORMAL_PRIORITY_CLASS, 0, 0, &stStartUpInfo, &stProcessInfo))
        return false;

    while(WAIT_OBJECT_0 != WaitForSingleObject(stProcessInfo.hProcess, vTimeOut))
		{
		
		};

	DWORD tExitCode = 0;
	if(FALSE == GetExitCodeProcess(stProcessInfo.hProcess, &tExitCode))
		return false;

    //释放句柄
    CloseHandle(stProcessInfo.hProcess);
    CloseHandle(stProcessInfo.hThread);

	
    return (0 == tExitCode);
}
#endif
//---------------------------------------------------------------------------
#define    xlib_bit_pos_add(_value, _pos)    ((_value) |= (1<<_pos))                
#define    xlib_bit_pos_del(_value, _pos)    ((_value) &= (~(1<<_pos)))            
#define    xlib_bit_pos_chk(_value, _pos)    (0 != ((_value)&(1<<_pos)))            

#define    xlib_bit_val_add(_value, _val)    ((_value) |= (_val))                
#define    xlib_bit_val_del(_value, _val)    ((_value) &= ~(_val))                
#define    xlib_bit_val_chk(_value, _val)    (_val == ((_value)&(_val)))
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif

