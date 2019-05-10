//---------------------------------------------------------------------------
#ifndef MI_ZAP_DATA_EXPORT_H
#define MI_ZAP_DATA_EXPORT_H
//---------------------------------------------------------------------------
#include <windows.h>
//---------------------------------------------------------------------------
#define DllName_ZapDataexport	"zap_dataexport.dll"
//---------------------------------------------------------------------------
#define ZAP_EXPORT_DEVICE_COMMON_UDISK		0x01
#define ZAP_EXPORT_DEVICE_GA_UDISK			0x02
#define ZAP_EXPORT_DEVICE_UNKOWN_UDISK		0xFF
//---------------------------------------------------------------------------
//文件打开模式(可以通过"|"的方式组合)
#define	OM_READ					0x00000001			//以读的方式打开文件
#define	OM_WRITE				0x00000002			//以写的方式打开文件
#define	OM_CREATE				0x00000004			//创建文件(如果文件不存在,则创建;如果文件存在,打开失败)
#define	OM_CREATE_NEW			0x00000008			//创建文件(如果文件不存在,则创建;如果文件存在,则清空文件内容)
//---------------------------------------------------------------------------
//设置文件指针的基址
#define FILE_BEGIN				0					//文件的起始位置
#define FILE_CURRENT			1					//文件的当前位置
#define FILE_END				2					//文件的末尾
//---------------------------------------------------------------------------
struct izapexportdevice
{
	virtual bool 	__cdecl init				(void)										= 0;
	virtual bool 	__cdecl final				(void)										= 0;
	
	virtual int 	__cdecl index				(void)  									= 0;
	virtual int 	__cdecl type				(void)										= 0;
	virtual bool 	__cdecl diskInf				(__int64& capacity,    							// U盘总容量(Byte为单位) （输出参数）
												char* factoryName,								// U盘厂家名;最大长度256字节
												char* usbSerialNumber,  						// U盘设备唯一标识;U盘序列号256字节
												char* companyName,   							// 公司名 256字节
												char* departmentName,							// 部门名 256字节
												char* userName) 							= 0;// 用户名 256字节
	
	virtual int 	__cdecl partitionCount		(void)										= 0;
	virtual __int64 __cdecl partitionFreeSpace	(int partition) 							= 0;
	
	virtual bool 	__cdecl login				(char* vUserPwd)							= 0;
	virtual HANDLE 	__cdecl open				(const char* path, DWORD mode)				= 0;
	virtual bool 	__cdecl close				(HANDLE handle)								= 0;
	virtual DWORD	__cdecl read				(HANDLE vFile, char* vBuff, DWORD vBuffLen)	= 0;
	virtual DWORD	__cdecl write				(HANDLE vFile, char* vBuff, DWORD vBuffLen) = 0;
	virtual bool	__cdecl flush				(HANDLE vFile)								= 0;
	virtual bool	__cdecl fileDelete			(const char* vPath)							= 0;

	virtual __int64	__cdecl	filePointerSet		(HANDLE vFile, __int64 vFilePointer, DWORD vMoveMethod) = 0;	//该方法返回新的文件指针位置
	virtual __int64	__cdecl	filePointerGet		(HANDLE vFile) = 0;	
	
	virtual bool	__cdecl	fileExists			(const char* vPath)	= 0;
	
	virtual bool	__cdecl	directoryExists		(const char* vPath)	= 0;
	virtual bool	__cdecl	createDirectory		(const char* vPath) = 0;

	virtual bool	__cdecl	fileTimeGet			(const char* vPath, PFILETIME vCreationTime, PFILETIME vLastAccessTime, PFILETIME vLastWriteTime) = 0;
	virtual bool	__cdecl	fileTimeSet			(const char* vPath, PFILETIME vCreationTime, PFILETIME vLastAccessTime, PFILETIME vLastWriteTime) = 0;
	virtual DWORD	__cdecl	getLastError		(void) = 0;	
};	
//---------------------------------------------------------------------------
struct izapdataexport
{
    virtual	bool 	__cdecl onLoad			(void)									= 0;
    virtual	bool	__cdecl onExit			(void)         						= 0;

	virtual bool 	__cdecl acquireDevice	(int index, izapexportdevice** device)	= 0;
	virtual bool 	__cdecl releaseDevice	(izapexportdevice* device)				= 0;

};
//---------------------------------------------------------------------------
#endif	//ZAP_DATA_EXPORT_H

