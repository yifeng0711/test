//---------------------------------------------------------------------------
#ifndef MI_FSFILTERKER_H
#define MI_FSFILTERKER_H
//---------------------------------------------------------------------------
#ifdef _NTDDK_
	#define __T(x)      L##x
	#include "E:\MyProgram2009\ZKinkooWareShare\ZKinkooWareShareSYS\Result\Inc\ZKinkooWareShareSYS.h"
#else
	#define __T(x)      x
	#include "E:\MyProgram2009\ZKinkooWareShare\ZKinkooWareShareLIB\Result\Inc\ZKinkooWareShareLIB.h"
#endif
//---------------------------------------------------------------------------
#define PROCESS_NAMELEN		256
#define PROCESS_COUNT		64
//---------------------------------------------------------------------------
#pragma pack(1)//×Ö½Ú¶ÔÆë 
struct DriveStatus
{
	int		Index;
	int		AccessStatus;
};
#pragma pack()
//---------------------------------------------------------------------------
#define DEVICE_NAME_FSFILTERKER_WORKNAME	__T("fsfilterker")
#define DEVICE_NAME_FSFILTERKER_FILENAME	__T("fsfilterker.sys")

#define DEVICE_NAME_FSFILTERKER_DDK		__T("\\Device\\fsfilterker")
#define DEVICE_NAME_FSFILTERKER_DOS		__T("\\DosDevices\\fsfilterker")
#define DEVICE_NAME_FSFILTERKER_API		__T("\\\\.\\fsfilterker")
//---------------------------------------------------------------------------
#define IOCTL_FSFILTERKER_SetAccessStatus		CTL_CODE(FILE_DEVICE_UNKNOWN,		0x1001, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_FSFILTERKER_SetGrantProcess		CTL_CODE(FILE_DEVICE_UNKNOWN,		0x1002, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_FSFILTERKER_CancelGrantProcess	CTL_CODE(FILE_DEVICE_UNKNOWN,		0x1003, METHOD_BUFFERED, FILE_ANY_ACCESS)
//---------------------------------------------------------------------------
#endif
