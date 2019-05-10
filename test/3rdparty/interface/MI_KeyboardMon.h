//---------------------------------------------------------------------------
#ifndef MI_keyboardmonH
#define MI_keyboardmonH
//---------------------------------------------------------------------------
#ifdef _NTDDK_
#define __T(x)      L##x
#include "E:\MyProgram2009\ZKinkooWareShare\ZKinkooWareShareSYS\Result\Inc\ZKinkooWareShareSYS.h"
#else
#define __T(x)      x
#include "E:\MyProgram2009\ZKinkooWareShare\ZKinkooWareShareLIB\Result\Inc\ZKinkooWareShareLIB.h"
#endif
//---------------------------------------------------------------------------
#define DEVICE_NAME_KeyboardMon_WORKNAME	__T("keyboardmon")
#define DEVICE_NAME_KeyboardMon_FILENAME	__T("keyboardmon.sys")
#define DEVICE_NAME_KeyboardMon64_FILENAME	__T("keyboardmon64.sys")

#define DEVICE_NAME_KeyboardMon_DDK 		__T("\\device\\keyboardmon")
#define DEVICE_NAME_KeyboardMon_DOS 		__T("\\dosdevices\\keyboardmon")
#define DEVICE_NAME_KeyboardMon_API 		__T("\\\\.\\keyboardmon")
//---------------------------------------------------------------------------
#define IOCTL_KeyboardMon_SetAdmin			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x7700, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_KeyboardMon_SetUser			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x7701, METHOD_BUFFERED, FILE_ANY_ACCESS)
//---------------------------------------------------------------------------
#endif