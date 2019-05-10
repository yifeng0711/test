//---------------------------------------------------------------------------
//该接口由国迈科技实现,提供给插件开发者调用
//---------------------------------------------------------------------------
#ifndef KKDEFileInterfaceH
#define KKDEFileInterfaceH
//---------------------------------------------------------------------------
#include <windows.h>
//---------------------------------------------------------------------------
//实现该接口的DLL文件名
#define DLLFileName_KKDEFileInterface		"KKDEFileInterface.dll"
//---------------------------------------------------------------------------
#define KKDE_SUCCESS			0x00000000	//成功
#define KKDE_ERROR_FAIL			0x00000001	//失败
#define KKDE_ERROR_NOT_GAU		0x00000002	//该盘不是安全u盘
#define KKDE_ERROR_NOT_INIT		0x00000003	//U盘没有进行密钥初始化
#define KKDE_ERROR_PWD_ERROR	0x00000004	//密码错误
#define KKDE_ERROR_PWD_LOCK		0x00000005	//密码已被锁定，需要重置密码
#define KKDE_ERROR_PROTECTED	0x00000006	//该U盘受保护，或调用者不合法
//---------------------------------------------------------------------------
//根目录格式
#define RootName				"S%d\\"				//文件系统可以分多个区,第一个区的根目录名为"S0\" 第二个为"S1" ...

//文件打开模式(可以通过"|"的方式组合)
#define	OM_READ					0x00000001			//以读的方式打开文件
#define	OM_WRITE				0x00000002			//以写的方式打开文件
#define	OM_CREATE				0x00000004			//创建文件(如果文件不存在,则创建;如果文件存在,打开失败)
#define	OM_CREATE_NEW			0x00000008			//创建文件(如果文件不存在,则创建;如果文件存在,则清空文件内容)

//设置文件指针的基址
#define FILE_BEGIN				0					//文件的起始位置
#define FILE_CURRENT			1					//文件的当前位置
#define FILE_END				2					//文件的末尾
//---------------------------------------------------------------------------
#pragma pack(1)
//文件遍历结构
struct KKFS_Find_Data
{
	DWORD		FileAttributes;						//文件属性
	FILETIME	CreationTime;						//创建时间
	FILETIME	LastAccessTime;						//最后一次访问的时间
	FILETIME	LastWriteTime;						//最后一次写的时间
	__int64		FileSize;							//文件大小
	char		FileName[260];						//文件名(不包含路径)
	DWORD		Reserved1;							//保留
	DWORD		Reserved2;							//保留
};
#pragma pack()
//---------------------------------------------------------------------------
//加载与卸载文件操作模块
bool		__stdcall	DE_IsGAUDisk					(int vIndex);
bool		__stdcall	DE_InitUDisk					(int vIndex);
bool		__stdcall	DE_FinalUDisk					(void);
bool		__stdcall	DE_GetUDiskInfo					(OUT __int64& vUsbCapacity,    	// U盘总容量(Byte为单位) （输出参数）
														OUT char* lpszFactoryName,		// U盘厂家名;最大长度256字节
														OUT char* lpszUsbSerialNumber,  // U盘设备唯一标识;U盘序列号256字节
														OUT char* szCompanyName,   		// 公司名 256字节
														OUT char* szDepartmentName,		// 部门名 256字节
														OUT char* szUserName       		// 用户名 256字节
														);

bool		__stdcall	DE_Login						(char* vPinCode, char* vUserPwd);	//登录，授权认证U盘

//文件操作
HANDLE		 __stdcall	DE_Open							(const char* vPath, DWORD vOpenMode);
void		 __stdcall	DE_Close						(HANDLE vFile);
DWORD		 __stdcall	DE_Read							(HANDLE vFile, char* vBuff, DWORD vBuffLen);
DWORD		 __stdcall	DE_Write						(HANDLE vFile, char* vBuff, DWORD vBuffLen);
bool		 __stdcall	DE_Flush						(HANDLE vFile);
__int64		 __stdcall	DE_FilePointerSet				(HANDLE vFile, __int64 vFilePointer, DWORD vMoveMethod);	//该方法返回新的文件指针位置
__int64		 __stdcall	DE_FilePointerGet				(HANDLE vFile);

__int64		 __stdcall	DE_FileSize						(const char* vPath);
bool		 __stdcall	DE_FileExists					(const char* vPath);
bool		 __stdcall	DE_FileDelete					(const char* vPath);
bool		 __stdcall	DE_FileRename					(const char* vPath, const char* vNewName); //vNewName为新的文件名,不包括路径

//目录操作
bool		 __stdcall	DE_DirectoryCreate				(const char* vPath);
bool		 __stdcall	DE_DirectoryRemove				(const char* vPath);	//只能删除空目录
bool		 __stdcall	DE_DirectoryExists				(const char* vPath);

HANDLE		 __stdcall	DE_FindFirst					(const char* vPath, KKFS_Find_Data* vFindData);	//获取vPath目录下第一个目录项(目录或文件),通常第一项是名称为"."的目录,
														//返回值为目录遍历句柄,供FindNext和CloseFind使用,该函数的使用可以参考
														//Windows API:FindFirstFile的使用
														//参数vPath不支持设置过滤条件,例如"C:\Text\*.*",只支持"C:\Text"形式的路径参数

bool		 __stdcall	DE_FindNext						(HANDLE vHandle, KKFS_Find_Data* vFindData, bool& vIsLast);	//获取下一个目录项,参考Windows API:FindNextFile的使用
bool		 __stdcall	DE_CloseFind					(HANDLE vHandle);								

//属性操作
DWORD		 __stdcall	DE_FileAttributesGet			(const char* vPath);
bool		 __stdcall	DE_FileAttributesSet			(const char* vPath, DWORD vAttr);
bool		 __stdcall	DE_FileTimeGet					(const char* vPath, PFILETIME vCreationTime, PFILETIME vLastAccessTime, PFILETIME vLastWriteTime);
bool		 __stdcall	DE_FileTimeSet					(const char* vPath, PFILETIME vCreationTime, PFILETIME vLastAccessTime, PFILETIME vLastWriteTime);

//分区操作接口
int			 __stdcall	DE_PartitionCount				(void);
__int64		 __stdcall	DE_GetPartitionFreeSpace		(const char* vRoot);
__int64		 __stdcall	DE_GetTotalSpace				(int iIndex);

DWORD		 __stdcall	DE_KGetLastError				(void);
//---------------------------------------------------------------------------
#endif