//---------------------------------------------------------------------------
#ifndef MI_ZAPUSBPORTENUM_H
#define MI_ZAPUSBPORTENUM_H
//---------------------------------------------------------------------------
#ifdef _WIN32
#define ModName_zapusbportenum "zap_usbportenum.dll"
#endif
#ifdef __linux__
#define ModName_zapusbportenum "zap_usbportenum.so"
#endif
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <windows.h>
using namespace std;
//---------------------------------------------------------------------------
struct usb_port_info
{
	int port;
	std::string serial;
	std::string keyname;
};
//---------------------------------------------------------------------------
struct izapusbportenum
{
    virtual	bool 	__cdecl onLoad		(const char* port_config_path)		        			= 0;
	virtual	bool 	__cdecl onExit		(void)													= 0;

	virtual	int 	__cdecl getPort		(int index, std::string& vSN)							= 0;
    virtual	int		__cdecl enumPort	(void)													= 0;

    virtual	int		__cdecl enumPortEx	(std::vector<usb_port_info>& portList, DWORD vLastPort)	= 0;
};
//---------------------------------------------------------------------------
#endif
