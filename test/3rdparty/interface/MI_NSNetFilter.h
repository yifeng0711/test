//---------------------------------------------------------------------------
#ifndef MI_NSNetFilterH
#define MI_NSNetFilterH
//---------------------------------------------------------------------------
#ifdef _NTDDK_
#define __T(x)      L##x
#else
#define __T(x)      x
#include <WinIoCtl.h>
#endif
//---------------------------------------------------------------------------
#define FILE_DEVICE_NDIS_HK					0x0000c501
#define DEVICE_NAME_NSNetFilter_WORKNAME	"nsnetfilter"
#define DEVICE_NAME_NSNetFilter_FILENAME	"nsnetfilter.sys"
#define DEVICE_NAME_NSNetFilter64_FILENAME	"nsnetfilter64.sys"

#define DEVICE_NAME_NSNetFilter_DDK			__T("\\Device\\nsnetfilter")
#define DEVICE_NAME_NSNetFilter_DOS			__T("\\DosDevices\\nsnetfilter")
#define DEVICE_NAME_NSNetFilter_API			__T("\\\\.\\nsnetfilter")
//---------------------------------------------------------------------------
#pragma pack(push,1)
//包过滤动作处理
enum NSFILTER_ACTION
{
	FILTER_ACTION_ALLOW	=0,
	FILTER_ACTION_DROP	=1,
	FILTER_ACTION_DELAY	=2,
	FILTER_ACTION_RES	=3,
	FILTER_ACTION_MAX
};
//数据流方向
enum NSDIRECTION
{
	DIRECTION_OUT	= 0,		/**< Client Send Package*/
	DIRECTION_IN	= 1,		/**< Client Recv Package*/
	DIRECTION_MAX
};
//过滤类型，决定调用的顺
enum NSFILTER_TYPE
{
	NSFILTER_ARP	= 0,		/**< Filter ARP Package*/
	NSFILTER_IP		= 1,		/**< Filter IP	Package*/
	NSFILTER_TCP	= 2,		/**< Filter TCP Package*/
	NSFILTER_UDP	= 3,		/**< Filter UDP Package*/
	NSFILTER_HTTP	= 4,		/**< Filter HTTP Package*/
	NSFILTER_DNS    = 5,
	NSFILTER_Other	= 6,		/**< Filter Other Package*/
	NSFILTER_MAX
};
//---------------------------------------------------------------------------
#define NSA_TrafficList_MAX 64
struct NSFilterContext
{
	bool	Continue;				//不再下传请设置为true
	UINT	TrafficId;				//限速0 ~ NSA_TrafficList_MAX(64)
	ULONG	BytePerSecond;			//限速每秒多少字节
};
//---------------------------------------------------------------------------
struct NSDNSFilterContext
{
	ULONG	FakeIP;
};
//---------------------------------------------------------------------------
struct NSFILTER_Version 
{
	ULONG	Ver;
	ULONG	WinPlatform;
};
//---------------------------------------------------------------------------
struct NSNetFilter_PACKET
{
	LIST_ENTRY			ListEntry;
	UINT				Iface;
	NSDIRECTION			Direction;

	USHORT				protocol;
	ULONG				LocalAddr;
	ULONG				RemoteAddr;
	USHORT				localPort;
	USHORT				remotePort;

	ULONG				DataLength;	//要发送的数据长度
	PUCHAR				Data;		//要发送的数据

	ULONG				ContextOffset;

	PVOID				IfaceX;
};
//---------------------------------------------------------------------------
//过滤函数定义
typedef NSFILTER_ACTION (*NSFilterRoutine)(NSNetFilter_PACKET* vPacket, OUT NSFilterContext* Context);
struct NSFILTER_SetFilterRoutine
{
	NSFILTER_TYPE	   FilterType;
	NSFilterRoutine	   FilterRoutine;	
	bool			   Remove;
};;
//---------------------------------------------------------------------------
#pragma pack(pop)
//---------------------------------------------------------------------------
#define IOCTL_NSNetFilter_GetFilterVersion	CTL_CODE(FILE_DEVICE_NDIS_HK, 0x1000, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_NSNetFilter_SetFilterRoutine	CTL_CODE(FILE_DEVICE_NDIS_HK, 0x2000, METHOD_BUFFERED, FILE_ANY_ACCESS)
//---------------------------------------------------------------------------
#endif
