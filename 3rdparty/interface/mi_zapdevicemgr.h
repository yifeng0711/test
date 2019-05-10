//---------------------------------------------------------------------------
#ifndef MI_ZAPDEVICEMGR_H
#define MI_ZAPDEVICEMGR_H
//---------------------------------------------------------------------------
#ifdef _WIN32
#define ModName_zapdevicemgr "zap_devicemgr.dll"
#endif
#ifdef __linux__
#define ModName_zapdevicemgr "zap_devicemgr.so"
#endif
#include <string>
#include <vector>
#include <windows.h>
#include <QStringList>
using namespace std;
//---------------------------------------------------------------------------
/*[字段名]:[类型]:[可选值]:说明
* [port]:[int]:[0~15]:对应端口信息
* [state]:[int]:[0,1,2]:0,识别中，1,识别成功，2,识别失败，3,端口无设备
* 以下信息识别成功才有效
* [guid]:[string]:[...]:设备唯一编号
* [index]:[int]:[0~25]:设备盘符索引号
* [bind_user]:[string]:绑定警员编号
* [sn]:[string]:[...]:硬件序列号
* [capacity]:[int]:[...]:总容量mb
* [remain]:[int]:[...]:剩余容量mb
* [product_id]:[string]:[...]:pid
* [vender_id]:[string]:[...]:vid
* [register_time]:[string]:[...]:注册时间
*/
//---------------------------------------------------------------------------
struct izapusb_event
{
    virtual	bool 	__cdecl onArrive	(int port)			= 0;
    virtual	bool 	__cdecl onLeave     (int port)  		= 0;
};
//---------------------------------------------------------------------------
struct izapdevicemgr
{
    virtual	bool 	__cdecl onLoad	(const char* port_config_path,const char* device_support_path,izapusb_event* event)	= 0;
    virtual	bool 	__cdecl onExit	(void)                                                                              = 0;

	/*获取对应端口详细信息，JSON定义见说明*/
    virtual	std::string	__cdecl getInfo         (int port)												= 0;

	/*绑定用户,只能绑定一个*/
    virtual	bool 		__cdecl bindUser        (int port, const char* pc_id)	                        = 0;
    virtual bool        __cdecl unBindUser      (int port)                                              = 0;

    //日志提取
    virtual std::string __cdecl getContent      (int port)                                              = 0;
    virtual bool        __cdecl getLogPath      (int port, QStringList& log_list)                       = 0;

    //重要视频文件信息
    virtual bool        __cdecl getMarkContent  (int port, QStringList& mark_content)                   = 0;
    virtual bool        __cdecl getMarkPath     (int port, QStringList& mark_list)                      = 0;
	
	//GPS信息提取
	virtual std::string __cdecl getGPSContent	(int port)						                        = 0;
    virtual bool        __cdecl getGPSPath 	    (int port, QStringList& GPS_list)						= 0;

    //设备相关
    virtual bool 		__cdecl eject           (int port)												= 0;
    virtual bool 		__cdecl timeSyn         (int port)												= 0;
};
//---------------------------------------------------------------------------
#endif
