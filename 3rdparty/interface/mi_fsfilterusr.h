//---------------------------------------------------------------------------
#ifndef MI_SampleExVCH
#define MI_SampleExVCH
//---------------------------------------------------------------------------
#include <windows.h>
//---------------------------------------------------------------------------
#define DLLFileName_NSUDKProtectUsr	"fsfilterusr.dll"
//---------------------------------------------------------------------------
#define NSA_SecDisk_AccessDisable			0
#define NSA_SecDisk_AccessReadOnly			1
#define NSA_SecDisk_AccessAllRight			2
//---------------------------------------------------------------------------
interface ifsfilterusr
{
	virtual bool __stdcall OnLoad	(void)																		= 0;
	virtual bool __stdcall OnExit	(void)																		= 0;

	virtual bool __stdcall SetAccessStatus		(int vDriveIndex, int vAccess=NSA_SecDisk_AccessDisable)		= 0;
	virtual bool __stdcall SetGrantProcess		(char* vProcessName)											= 0;
	virtual bool __stdcall CancelGrantProcess	(char* vProcessName)											= 0;
};
//---------------------------------------------------------------------------
#endif
