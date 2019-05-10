//---------------------------------------------------------------------------
#ifndef MI_ZAPUSBPORTDETECT_H
#define MI_ZAPUSBPORTDETECT_H
//---------------------------------------------------------------------------
#ifdef _WIN32
#define ModName_zapusbportdetect "zap_usbportdetect.dll"
#endif
#ifdef __linux__
#define ModName_zapusbportdetect "zap_usbportdetect.so"
#endif
//---------------------------------------------------------------------------
#include "string"
//---------------------------------------------------------------------------
/*[字段名]:[类型]:[可选值]:说明
* [port]:[int]:[0~15]:对应端口信息
* [state]:[int]:[0,1,2]:0,识别中，1,识别成功，2,识别失败
* 以下信息识别成功才有效
* [index]:[int]:[0~25]:设备盘符索引号
* [bind]:[int]:[0,1,2,3,4]:绑定警员个数,0表示没有绑定
* [pc_id1]:[string]:[...]:绑定警员编号1
* [pc_id2]:[string]:[...]:绑定警员编号2
* [pc_id3]:[string]:[...]:绑定警员编号3
* [pc_id4]:[string]:[...]:绑定警员编号4
* [serial]:[string]:[...]:外壳编号
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
struct izapusbportdetect
{
	virtual	bool 	__cdecl onLoad	(void)							= 0;
	virtual	bool 	__cdecl onExit	(void)							= 0;

    virtual	bool 	__cdecl start	(izapusb_event* event)			= 0;
	virtual	bool 	__cdecl stop	(void)							= 0;

	/*获取对应端口详细信息，JSON定义见说明*/
	virtual	std::string	__cdecl getInfo		(int port)												= 0;	

	/*绑定用户，最多支持4个*/
//	virtual	bool 		__cdecl bindUser	(int port, std::string& serial, std::string& pc_id[4])	= 0;
	virtual bool 		__cdecl eject		(int port)												= 0;
	virtual bool 		__cdecl timeSyn		(int port)												= 0;
};
//---------------------------------------------------------------------------
#endif
