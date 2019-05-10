#ifndef MC_ComputerInfo_H
#define MC_ComputerInfo_H
//---------------------------------------------------------------------------
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//---------------------------------------------------------------------------
#include <Windows.h>
#include <string>
using namespace std;
//---------------------------------------------------------------------------
class MC_ComputerInfo
{
public:
	static	bool			PowerOff					(bool vRestart);	//true=重启，false=关机

	/**
	 * @Method		:	GetScreenResolution
	 * @brief		:	获取屏幕分辨率
	 * @param[out]	:	vLateral	横向分辨率
	 * @param[out]	:	vVertical	纵向分辨率
	 */
	static	bool			GetScreenResolution			(int& vLateral, int& vVertical);

	static	int				GetCpuUsedPercent			(void);	//CPU使用率
	static	DWORD			GetRamUserPercent			(void);	//内存使用率
	static	std::string		GetMac						(void);	

private:
};
//---------------------------------------------------------------------------
#endif