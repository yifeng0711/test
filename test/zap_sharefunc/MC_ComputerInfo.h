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
	static	bool			PowerOff					(bool vRestart);	//true=������false=�ػ�

	/**
	 * @Method		:	GetScreenResolution
	 * @brief		:	��ȡ��Ļ�ֱ���
	 * @param[out]	:	vLateral	����ֱ���
	 * @param[out]	:	vVertical	����ֱ���
	 */
	static	bool			GetScreenResolution			(int& vLateral, int& vVertical);

	static	int				GetCpuUsedPercent			(void);	//CPUʹ����
	static	DWORD			GetRamUserPercent			(void);	//�ڴ�ʹ����
	static	std::string		GetMac						(void);	

private:
};
//---------------------------------------------------------------------------
#endif