//---------------------------------------------------------------------------
#ifndef MI_RESTCLIENTSDK_H
#define MI_RESTCLIENTSDK_H
//---------------------------------------------------------------------------
#ifdef _WIN32
    #define ModName_restclientsdk "zap_restclientsdk.dll"
    #include <Windows.h>
#endif
#ifdef __linux__
    #define ModName_restclientsdk "zap_restclientsdk.so"
#endif
//---------------------------------------------------------------------------
//错误码
//---------------------------------------------------------------------------
typedef size_t (__cdecl *OnWriteData)(void* buffer, size_t size, size_t nmemb, void* context);
//---------------------------------------------------------------------------
struct irestclientsdk
{
	/*url:http://host:port/device_ws*/
    virtual bool	__cdecl	Init		(OnWriteData fCallBack, int connect_timeout=3, int timeout=3)								= 0;
    virtual bool	__cdecl	UnInit		(void)																						= 0;
    
	/*/worksation/*/
	virtual int		__cdecl	REST_GET		(const char* uri, void* context, char* errMsg=0)										= 0;
    virtual int		__cdecl	REST_PUT		(const char* uri, const char* body, int bodyLen, void* context, char* errMsg=0)			= 0;
	virtual int		__cdecl	REST_POST		(const char* uri, const char* body, int bodyLen, void* context, char* errMsg=0)			= 0;
	virtual int		__cdecl	REST_DELETE		(const char* uri, void* context, char* errMsg=0)										= 0;
};
//---------------------------------------------------------------------------
#endif
