//---------------------------------------------------------------------------
//�ýӿ��ɹ����Ƽ�ʵ��,�ṩ����������ߵ���
//---------------------------------------------------------------------------
#ifndef KKDEFileInterfaceH
#define KKDEFileInterfaceH
//---------------------------------------------------------------------------
#include <windows.h>
//---------------------------------------------------------------------------
//ʵ�ָýӿڵ�DLL�ļ���
#define DLLFileName_KKDEFileInterface		"KKDEFileInterface.dll"
//---------------------------------------------------------------------------
#define KKDE_SUCCESS			0x00000000	//�ɹ�
#define KKDE_ERROR_FAIL			0x00000001	//ʧ��
#define KKDE_ERROR_NOT_GAU		0x00000002	//���̲��ǰ�ȫu��
#define KKDE_ERROR_NOT_INIT		0x00000003	//U��û�н�����Կ��ʼ��
#define KKDE_ERROR_PWD_ERROR	0x00000004	//�������
#define KKDE_ERROR_PWD_LOCK		0x00000005	//�����ѱ���������Ҫ��������
#define KKDE_ERROR_PROTECTED	0x00000006	//��U���ܱ�����������߲��Ϸ�
//---------------------------------------------------------------------------
//��Ŀ¼��ʽ
#define RootName				"S%d\\"				//�ļ�ϵͳ���Էֶ����,��һ�����ĸ�Ŀ¼��Ϊ"S0\" �ڶ���Ϊ"S1" ...

//�ļ���ģʽ(����ͨ��"|"�ķ�ʽ���)
#define	OM_READ					0x00000001			//�Զ��ķ�ʽ���ļ�
#define	OM_WRITE				0x00000002			//��д�ķ�ʽ���ļ�
#define	OM_CREATE				0x00000004			//�����ļ�(����ļ�������,�򴴽�;����ļ�����,��ʧ��)
#define	OM_CREATE_NEW			0x00000008			//�����ļ�(����ļ�������,�򴴽�;����ļ�����,������ļ�����)

//�����ļ�ָ��Ļ�ַ
#define FILE_BEGIN				0					//�ļ�����ʼλ��
#define FILE_CURRENT			1					//�ļ��ĵ�ǰλ��
#define FILE_END				2					//�ļ���ĩβ
//---------------------------------------------------------------------------
#pragma pack(1)
//�ļ������ṹ
struct KKFS_Find_Data
{
	DWORD		FileAttributes;						//�ļ�����
	FILETIME	CreationTime;						//����ʱ��
	FILETIME	LastAccessTime;						//���һ�η��ʵ�ʱ��
	FILETIME	LastWriteTime;						//���һ��д��ʱ��
	__int64		FileSize;							//�ļ���С
	char		FileName[260];						//�ļ���(������·��)
	DWORD		Reserved1;							//����
	DWORD		Reserved2;							//����
};
#pragma pack()
//---------------------------------------------------------------------------
//������ж���ļ�����ģ��
bool		__stdcall	DE_IsGAUDisk					(int vIndex);
bool		__stdcall	DE_InitUDisk					(int vIndex);
bool		__stdcall	DE_FinalUDisk					(void);
bool		__stdcall	DE_GetUDiskInfo					(OUT __int64& vUsbCapacity,    	// U��������(ByteΪ��λ) �����������
														OUT char* lpszFactoryName,		// U�̳�����;��󳤶�256�ֽ�
														OUT char* lpszUsbSerialNumber,  // U���豸Ψһ��ʶ;U�����к�256�ֽ�
														OUT char* szCompanyName,   		// ��˾�� 256�ֽ�
														OUT char* szDepartmentName,		// ������ 256�ֽ�
														OUT char* szUserName       		// �û��� 256�ֽ�
														);

bool		__stdcall	DE_Login						(char* vPinCode, char* vUserPwd);	//��¼����Ȩ��֤U��

//�ļ�����
HANDLE		 __stdcall	DE_Open							(const char* vPath, DWORD vOpenMode);
void		 __stdcall	DE_Close						(HANDLE vFile);
DWORD		 __stdcall	DE_Read							(HANDLE vFile, char* vBuff, DWORD vBuffLen);
DWORD		 __stdcall	DE_Write						(HANDLE vFile, char* vBuff, DWORD vBuffLen);
bool		 __stdcall	DE_Flush						(HANDLE vFile);
__int64		 __stdcall	DE_FilePointerSet				(HANDLE vFile, __int64 vFilePointer, DWORD vMoveMethod);	//�÷��������µ��ļ�ָ��λ��
__int64		 __stdcall	DE_FilePointerGet				(HANDLE vFile);

__int64		 __stdcall	DE_FileSize						(const char* vPath);
bool		 __stdcall	DE_FileExists					(const char* vPath);
bool		 __stdcall	DE_FileDelete					(const char* vPath);
bool		 __stdcall	DE_FileRename					(const char* vPath, const char* vNewName); //vNewNameΪ�µ��ļ���,������·��

//Ŀ¼����
bool		 __stdcall	DE_DirectoryCreate				(const char* vPath);
bool		 __stdcall	DE_DirectoryRemove				(const char* vPath);	//ֻ��ɾ����Ŀ¼
bool		 __stdcall	DE_DirectoryExists				(const char* vPath);

HANDLE		 __stdcall	DE_FindFirst					(const char* vPath, KKFS_Find_Data* vFindData);	//��ȡvPathĿ¼�µ�һ��Ŀ¼��(Ŀ¼���ļ�),ͨ����һ��������Ϊ"."��Ŀ¼,
														//����ֵΪĿ¼�������,��FindNext��CloseFindʹ��,�ú�����ʹ�ÿ��Բο�
														//Windows API:FindFirstFile��ʹ��
														//����vPath��֧�����ù�������,����"C:\Text\*.*",ֻ֧��"C:\Text"��ʽ��·������

bool		 __stdcall	DE_FindNext						(HANDLE vHandle, KKFS_Find_Data* vFindData, bool& vIsLast);	//��ȡ��һ��Ŀ¼��,�ο�Windows API:FindNextFile��ʹ��
bool		 __stdcall	DE_CloseFind					(HANDLE vHandle);								

//���Բ���
DWORD		 __stdcall	DE_FileAttributesGet			(const char* vPath);
bool		 __stdcall	DE_FileAttributesSet			(const char* vPath, DWORD vAttr);
bool		 __stdcall	DE_FileTimeGet					(const char* vPath, PFILETIME vCreationTime, PFILETIME vLastAccessTime, PFILETIME vLastWriteTime);
bool		 __stdcall	DE_FileTimeSet					(const char* vPath, PFILETIME vCreationTime, PFILETIME vLastAccessTime, PFILETIME vLastWriteTime);

//���������ӿ�
int			 __stdcall	DE_PartitionCount				(void);
__int64		 __stdcall	DE_GetPartitionFreeSpace		(const char* vRoot);
__int64		 __stdcall	DE_GetTotalSpace				(int iIndex);

DWORD		 __stdcall	DE_KGetLastError				(void);
//---------------------------------------------------------------------------
#endif