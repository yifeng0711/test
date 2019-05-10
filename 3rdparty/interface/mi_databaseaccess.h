//------------------------------------------------------------------------------
#ifndef MI_DatabaseAccessH
#define MI_DatabaseAccessH
//------------------------------------------------------------------------------
#if (defined WIN32) || (defined WIN64)
	#define WINDOWS
	#include <Windows.h>
#endif

#ifndef interface
    #define interface   struct
#endif

#ifndef HANDLE
    typedef void *HANDLE;
#endif
//------------------------------------------------------------------------------
#ifdef WINDOWS
    #define DLLName_DatabaseAccess  "DatabaseAccess.dll"
#endif
#ifdef LINUX
    #define DLLName_DatabaseAccess  "DatabaseAccess.so"
#endif
//------------------------------------------------------------------------------
#define	DB_VALUE_MAXLEN	8388608	//8*1024*1024
//------------------------------------------------------------------------------
interface IDatabaseAccess
{
    virtual bool		__stdcall 	Connect			(const char* vHost, short vPort, char* vUserName, char* vPassWord, char* vDBName, char* vLanguage="Latin1")			= 0;
    virtual bool		__stdcall 	IsConnected		(void)																												= 0;
    virtual bool		__stdcall 	Disconnect		(void)																												= 0; //关闭vDBFileName文件数据库

    virtual bool		__stdcall 	SetRecordValue	(const char* vTable, const char* vField, const char* vWhere, const char* vValue, int vLen)							= 0; //vlen无效,设定指定位置的值(可用于二进制数据)
    virtual bool		__stdcall 	GetRecordValue	(const char* vTable, const char* vField, const char* vWhere, char* vValue, int* vLen)								= 0; //获取指定位置的值(可用于二进制数据)

    virtual bool		__stdcall 	TransactionBegin(void)																												= 0; //事务处理开始
    virtual bool		__stdcall 	TransactionEnd	(bool vRollback=false)																								= 0; //事务处理结束操作, 若在事务执行过程中已经失败(失败将导致立即RolllBack), 此时仍然要求提交变更, 则也会返回失败.

    virtual bool		__stdcall 	Execute			(char* vSQL, int* vRowsAffected=0, int* vLastInsertId=0)															= 0; //执行单行SQL指令, vRowsAffected为受影响的行数, vLastInsertId为调用插入语句最后的自增长ID值
    virtual HANDLE		__stdcall 	Query			(char* vSQL)																										= 0;
    virtual bool		__stdcall 	EndQuery		(HANDLE vQuery)																										= 0;

    virtual int			__stdcall 	RecordCount		(HANDLE vQuery)																										= 0;
    virtual int			__stdcall 	FieldCount		(HANDLE vQuery)																										= 0;

    virtual int			__stdcall 	RecordValueI	(HANDLE vQuery, int vRecord, int vField)																			= 0;
    virtual char*		__stdcall 	RecordValueS	(HANDLE vQuery, int vRecord, int vField)																			= 0; //执行查询SQL后, 返回全部的记录, 记录的数目若超过KKDB_RECORD_MAX, 则返回错误.

	virtual int			__stdcall 	RecordValueByFI	(HANDLE vQuery, int vRecord, char* vFieldName)																			= 0;
	virtual char*		__stdcall 	RecordValueByFS	(HANDLE vQuery, int vRecord, char* vFieldName)																			= 0; //执行查询SQL后, 返回全部的记录, 记录的数目若超过KKDB_RECORD_MAX, 则返回错误.
};
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
