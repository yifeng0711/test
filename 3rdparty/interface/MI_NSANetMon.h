/*----------------------------------------------------------------------------
*������ģʽ:ֻ�й�����е�IP�˿ڶ��ܹ���������
*������ģʽ:������е�IP�˿ڶζ����ܷ�������
*����:���������ģʽ,�����������IP�˿ڶβ��ܷ�������;����Ǻ�����ģʽ,��������
*��Ŀ�������.������Ϊ��������־
----------------------------------------------------------------------------*/
//----------------------------------------------------------------------------
#ifndef MI_TdiFilterH
#define MI_TdiFilterH
//----------------------------------------------------------------------------
#ifdef _NTDDK_
#define __T(x)      L##x
#else
#define __T(x)      x
#include <WinIoCtl.h>
#endif
//----------------------------------------------------------------------------
#define FILE_DEVICE_TDI_HK                0x0000c502
#define Device_NSANetMon_Name_DDK	       __T("\\Device\\nsanetmon")
#define Device_NSANetMon_Name_DOS	       __T("\\DosDevices\\nsanetmon")
#define Device_NSANetMon_Name_API	       __T("\\\\.\\nsanetmon")
#define Device_NSANetMon_Name_WORKNAME	   "nsanetmon"
#define Device_NSANetMon_Name_FILENAME     "nsanetmon.sys"
#define Device_NSANetMon_Name64_FILENAME    "nsanetmon64.sys"
//----------------------------------------------------------------------------
#define IOCTL_NSANetMon_SetRule		    CTL_CODE(FILE_DEVICE_TDI_HK, 0x1000, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_NSANetMon_Start		    CTL_CODE(FILE_DEVICE_TDI_HK, 0x1001, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_NSANetMon_Stop		    CTL_CODE(FILE_DEVICE_TDI_HK, 0x1002, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_NSANetMon_GetLog		    CTL_CODE(FILE_DEVICE_TDI_HK, 0x1004, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_NSANetMon_GetUrlLog	    CTL_CODE(FILE_DEVICE_TDI_HK, 0x1008, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_NSANetMon_SetUrlRule	    CTL_CODE(FILE_DEVICE_TDI_HK, 0x1010, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_NSANetMon_ReSetUrlRule	CTL_CODE(FILE_DEVICE_TDI_HK, 0x1011, METHOD_BUFFERED, FILE_ANY_ACCESS)
//----------------------------------------------------------------------------
#define	TDI_ADDRESS_MAX_LENGTH	TDI_ADDRESS_LENGTH_OSI_TSAP
#define	TA_ADDRESS_MAX			(sizeof(TA_ADDRESS)	- 1	+ TDI_ADDRESS_MAX_LENGTH)
#define	TDI_ADDRESS_INFO_MAX	(sizeof(TDI_ADDRESS_INFO) -	1 +	TDI_ADDRESS_MAX_LENGTH)
//-----------------------------------------------------------------------------
//Э��
#define IPPROTO_IP			0
#define	IPPROTO_ICMP		1
#define IPPROTO_IGMP		2
#define IPPROTO_TCP			6
#define IPPROTO_UDP			17
#define IPPROTO_MAX			256
//----------------------------------------------------------------------------
//����������
#define TDIDIRECTION_OUT	0
#define TDIDIRECTION_IN		1
#define TDIDIRECTION_MAX	2
//----------------------------------------------------------------------------
#define ACTION_KNM_DROP			0
#define ACTION_KNM_PASS			1
//----------------------------------------------------------------------------
//����ģʽ
#define WHITE_MODE			0x1
#define BLACK_MODE			0x2
//----------------------------------------------------------------------------
#define Rule_Max_Item		256
#define Log_Max_Item		128
#define Skip_Max_Ip			8

#define Log_HashSize			109   //ɢ�б�ĳ��� ���˸���
//----------------------------------------------------------------------------
#define CALC_HASH(x, size)	((((ULONG)(x)) >> 5) % (size))
//----------------------------------------------------------------------------
enum ProxyType
{
	PROXYTYPE_HTTP	= 0,		//HTTP����
	PROXYTYPE_FTP	= 1,		//FTP����
	PROXYTYPE_SOCKET= 2,		//socket����
	PROXYTYPE_MAX
};
//----------------------------------------------------------------------------
#pragma pack(push,1)
struct	TLIST_KNM_Rule
{
	bool			EnableSys;
	bool			EnableLog;
	ULONG			SkipIP[4];
	int				Mode;
	int				ItemCount;
	int				NetflowKB;
	struct
	{
		bool		Allow;
		ULONG		IPFrom;
		ULONG		IPTo;
		ULONG		PortFrom;
		ULONG		PortTo;
		bool		NetflowLimit;
	}Item[Rule_Max_Item];
};
//----------------------------------------------------------------------------
struct RuleItem
{
	bool		Allow;
	ULONG		IPFrom;
	ULONG		IPTo;
	ULONG		PortFrom;
	ULONG		PortTo;
	bool		NetflowLimit;
};
//----------------------------------------------------------------------------
struct LogItem
{
	char				ProcessName[64];
	LARGE_INTEGER		FistTime;
	LARGE_INTEGER       LastTime;
	ULONG				LocalIp;
	ULONG				LocalPort;
	ULONG				RemoteIp;
	ULONG				RemotePort;
	ULONG				BytesOut;				//�ϴ���С 
	ULONG				ByteIn;					//���ش�С
	int					Protocol;				//Э������
	int					Action;					//0Ϊ�ܾ�, 1Ϊ���� 
};
//-----------------------------------------------------------------------------
struct LogItem_SYS                                //����hash��
{
	char				ProcessName[64];
	LARGE_INTEGER		FistTime;
	LARGE_INTEGER       LastTime;
	ULONG				LocalIp;
	ULONG				LocalPort;
	ULONG				RemoteIp;
	ULONG				RemotePort;
	ULONG				BytesOut;				//�ϴ���С 
	ULONG				ByteIn;					//���ش�С
	int					Protocol;				//Э������
	int					Action;					//0Ϊ�ܾ�, 1Ϊ���� 
	struct LogItem_SYS* Next;
};
//-----------------------------------------------------------------------------
struct Log 
{
	int			ItemCount;
	LogItem		Items[Log_Max_Item];
};
//-----------------------------------------------------------------------------
struct UrlItem
{
	char	UrlKey[32];
	ULONG   UrlKeyLen;
};
//-----------------------------------------------------------------------------
struct URLRule
{
	bool		EnableSys;
	bool		EnableLog;
	int		    Mode;					//������, ������ģʽ
	ULONG		DestIp;
	ULONG		ItemCount;
    UrlItem     Items[Rule_Max_Item];
};
//----------------------------------------------------------------------------
struct URLLogItem 
{
	char	        Url[128];
	char	        ProcessName[64];
	USHORT			LocalPort;
	ULONG			UrlLen;
	ULONG			RemoteIp;
	LARGE_INTEGER	LocalTime;
	int		        Action;
};
//----------------------------------------------------------------------------
struct URLLog 
{
	int			ItemCount;	
	URLLogItem	Items[Log_Max_Item];
};
//----------------------------------------------------------------------------
typedef struct	_ProxyInfo
{
	ULONG	IpAddr;
	ULONG	IpPort;
	int		ProxyFlag;
}ProxyInfo,*PProxyInfo;
#pragma pack(pop)
//----------------------------------------------------------------------------
#endif