#ifndef __MC_LogH__
#define __MC_LogH__
//--------------------------------------
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//--------------------------------------
#include <Windows.h>
#include <string>
#include <stdio.h>
//--------------------------------------
using namespace std;
//--------------------------------------
inline string LogTextCombine(char* vFormat,...){

    if(0 == vFormat)			return "";
    if(0 == *vFormat)			return "";

    const int tMaxLen = 4096;
    char tStr[tMaxLen];
    memset(tStr, 0, tMaxLen);

    va_list tArg;
    va_start(tArg, vFormat);

    try
    {
        _vsnprintf(tStr, tMaxLen-1, vFormat, tArg);
    }
    catch(...)
    {
        return "";
    }

    return tStr;

}
//--------------------------------------
#define _LOG(__TYPE__, __FORMAT__, ...)	LogTextCombine("%-8s [L:%04u] %-32s>> " __FORMAT__ , __TYPE__, __LINE__, __FUNCTION__, ##__VA_ARGS__).c_str()
#define _LOG2(__FORMAT__, ...)			LogTextCombine("" __FORMAT__ , ##__VA_ARGS__).c_str()
#define LOG_ERROR(__FORMAT__, ...)		gLog.Log(_LOG("ERROR", __FORMAT__, ##__VA_ARGS__))
#define LOG_WARN(__FORMAT__, ...)		gLog.Log(_LOG("WARN", __FORMAT__, ##__VA_ARGS__))
#define LOG_INFO(__FORMAT__, ...)		gLog.Log(_LOG("INFO", __FORMAT__, ##__VA_ARGS__))
#define LOG_TEXT(__FORMAT__, ...)		gLog.Log(_LOG2(__FORMAT__, ##__VA_ARGS__))
//--------------------------------------
class MC_Log
{
public:
    MC_Log(const char* vLogFileName = 0, int vBufferSize = 4096);
    ~MC_Log(void);

    void Log(const char* vLogData);

private:
    int		m_BufferSize;	//kb
    string	m_LogFileName;

    HANDLE	m_EventHandle;
};
//--------------------------------------
extern MC_Log gLog;
//--------------------------------------
#endif
