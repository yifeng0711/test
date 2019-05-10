//---------------------------------------------------------------------------
#ifndef MI_ZAPSECCOMPONENT_H
#define MI_ZAPSECCOMPONENT_H
//---------------------------------------------------------------------------
#define ModName_ZAPDeviceMon 	"\\plugin\\devicemon\\zap_devicemon.dll"
#define ModName_ZAPExportMon 	"\\plugin\\exportmon\\zap_exportmon.dll"
#define ModName_ZAPNetMon 		"\\plugin\\netmon\\zap_netmon.dll"
#define ModName_ZAPSysMon 		"\\plugin\\sysmon\\zap_sysmon.dll"
//---------------------------------------------------------------------------
#define ZAP_COMPONENT_TYPE_DEVICE	0x1
#define ZAP_COMPONENT_TYPE_NET		0x2
#define ZAP_COMPONENT_TYPE_EXPORT	0x3
#define ZAP_COMPONENT_TYPE_SYS		0x4
#define ZAP_SAFEMODE_FILE			"c:\\zap_safemodefile"
//---------------------------------------------------------------------------
struct iseccomponent
{
	virtual bool __cdecl onLoad				(void)					 			= 0;
	virtual bool __cdecl onExit				(void) 								= 0;
	
	//组件信息，类型与版本(20个字节缓冲区)
	virtual bool __cdecl getComponentInf	(int& vType, char* vVer, int& vLen)	= 0;
	
	virtual bool __cdecl setRule			(const char* vXml, int vLen)		= 0;
	virtual bool __cdecl getRule			(char* vXml, int& vLen) 			= 0;

	virtual	bool __cdecl onDeviceArrive		(int vIndex, int vPort, const char* vSerial)        = 0;
	virtual	bool __cdecl onDeviceLeave		(int vIndex)                      					= 0;
};
//---------------------------------------------------------------------------
#endif 
