//---------------------------------------------------------------------------
#ifndef MI_RPCCLIENT_H
#define MI_RPCCLIENT_H
//---------------------------------------------------------------------------
#ifdef _WIN32
    #define ModName_rpcclient "zap_coreservercli.dll"
    #include <Windows.h>
#endif
#ifdef __linux__
    #define ModName_rpcclient "zap_coreservercli.so"
#endif
//---------------------------------------------------------------------------
struct icoreservercli
{
    virtual bool	__cdecl	start		(const char* host, int port, int timeoutsec=3*60*1000)								= 0;
    virtual bool	__cdecl	stop		(void)																						= 0;
    virtual bool	__cdecl	ping		(void)																						= 0;
    virtual int		__cdecl	request	(const char* reqId, const char* reqBuf, int reqLen, char** resBuf, int& resLen, char* errMsg=0)	= 0;
	virtual void	__cdecl	freeRes	(char* resBuf)																			= 0;
};
//---------------------------------------------------------------------------
#endif
