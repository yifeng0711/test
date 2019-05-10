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
    virtual bool		__stdcall 	Disconnect		(void)																												= 0; //�ر�vDBFileName�ļ����ݿ�

    virtual bool		__stdcall 	SetRecordValue	(const char* vTable, const char* vField, const char* vWhere, const char* vValue, int vLen)							= 0; //vlen��Ч,�趨ָ��λ�õ�ֵ(�����ڶ���������)
    virtual bool		__stdcall 	GetRecordValue	(const char* vTable, const char* vField, const char* vWhere, char* vValue, int* vLen)								= 0; //��ȡָ��λ�õ�ֵ(�����ڶ���������)

    virtual bool		__stdcall 	TransactionBegin(void)																												= 0; //������ʼ
    virtual bool		__stdcall 	TransactionEnd	(bool vRollback=false)																								= 0; //�������������, ��������ִ�й������Ѿ�ʧ��(ʧ�ܽ���������RolllBack), ��ʱ��ȻҪ���ύ���, ��Ҳ�᷵��ʧ��.

    virtual bool		__stdcall 	Execute			(char* vSQL, int* vRowsAffected=0, int* vLastInsertId=0)															= 0; //ִ�е���SQLָ��, vRowsAffectedΪ��Ӱ�������, vLastInsertIdΪ���ò����������������IDֵ
    virtual HANDLE		__stdcall 	Query			(char* vSQL)																										= 0;
    virtual bool		__stdcall 	EndQuery		(HANDLE vQuery)																										= 0;

    virtual int			__stdcall 	RecordCount		(HANDLE vQuery)																										= 0;
    virtual int			__stdcall 	FieldCount		(HANDLE vQuery)																										= 0;

    virtual int			__stdcall 	RecordValueI	(HANDLE vQuery, int vRecord, int vField)																			= 0;
    virtual char*		__stdcall 	RecordValueS	(HANDLE vQuery, int vRecord, int vField)																			= 0; //ִ�в�ѯSQL��, ����ȫ���ļ�¼, ��¼����Ŀ������KKDB_RECORD_MAX, �򷵻ش���.

	virtual int			__stdcall 	RecordValueByFI	(HANDLE vQuery, int vRecord, char* vFieldName)																			= 0;
	virtual char*		__stdcall 	RecordValueByFS	(HANDLE vQuery, int vRecord, char* vFieldName)																			= 0; //ִ�в�ѯSQL��, ����ȫ���ļ�¼, ��¼����Ŀ������KKDB_RECORD_MAX, �򷵻ش���.
};
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
