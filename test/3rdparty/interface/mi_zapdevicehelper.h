//---------------------------------------------------------------------------
#ifndef MI_ZAPDEVICEHELPER_H
#define MI_ZAPDEVICEHELPER_H
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#ifdef _WIN32
#define ModName_ZAPDeviceHelper "zap_devicehelper.dll"
#endif
#ifdef __linux__
#define ModName_ZAPDeviceHelper "zap_devicehelper.so"
#endif
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
struct izapdevice_helper
{
	virtual	bool __cdecl onLoad			(void)													= 0;
	virtual	bool __cdecl onExit			(void)													= 0;

    virtual bool __cdecl xRegister    	(int index, const char* device_id, int len)				= 0;
    virtual	bool __cdecl unRegister		(int index)												= 0;

	virtual	bool __cdecl getDeviceId	(int index, char* device_id, int* len)					= 0;
	
	virtual bool __cdecl eject			(int index)												= 0;
	virtual bool __cdecl timeSyn		(int index)												= 0;
};
//---------------------------------------------------------------------------
#endif
