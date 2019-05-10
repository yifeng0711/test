//---------------------------------------------------------------------------
#ifndef MI_DEVICE_SUPPORT_H
#define MI_DEVICE_SUPPORT_H
//---------------------------------------------------------------------------
#include <windows.h>
//---------------------------------------------------------------------------
#define  OPERATION_UNKNOWN          0
#define  OPERATION_BOOT             1
#define  OPERATION_SHUTDOWN         2
#define  OPERATION_START_VEDIO      3
#define  OPERATION_END_VEDIO        4
#define  OPERATION_START_RECOND     5
#define  OPERATION_END_RECOND       6
#define  OPERATION_TAKE_PICTURE     7
#define  OPERATION_USB_CONNECT      8
#define  OPERATION_USB_DISCONNECT   9
#define  OPERATION_PLAYBACK         10
#define  OPERATION_ADCONNECT        11
#define  OPERATION_USERCONNECT      12
#define  OPERATION_ENTERSYSMENU     13

#define  OPERATION_OTHERS           100
//---------------------------------------------------------------------------
/*�����ӿ� getLogContent
*  ����
*	vDrvIndex  : ��Ӧ�̷�����
*	getLogContent vLogs ��ʽ
*		"[{"operation":1 , "operation_time":"2014-03-23 12:32:32"}, {"operation":2,"operation_time":"2014-03-23 12:32:32"}]"
*  ����ֵ
*	�ɹ� true, ʧ�� false
*/

/* �����ӿ� getLogPath
*  ����
*	vDrvIndex  : ��Ӧ�̷�����
*	vLogs      : ��־����(��ʽ��������)
*		"[{"log_path":"F:/LOG/123123.dat"},{"log_path":"F:/LOG/dary.dat"}]"
*  ����ֵ
*	�ɹ� true, ʧ�� false
*/

/* �����ӿ� getMarkPoint
*  ����
*	vDrvIndex  : ��Ӧ�̷�����
*	vMarkPoint : �����Ϣ����(��ʽ��������)
		[{"time":"2014-03-23 12:32:32"}, {"time":"2014-03-23 12:32:32"}]
*  ����ֵ
*	�ɹ� true, ʧ�� false
*/

/* �����ӿ� getMarkPath
*  ����
*	vDrvIndex  : ��Ӧ�̷�����
*	vMarkPaths : �����Ϣ·���б�(��ʽ��������)
		[{"path":"F:/DCIM/100MEDIA/FILE00001.dat"}, {"path":"F:/DCIM/100MEDIA/FILE00002.dat"}]
*  ����ֵ
*	�ɹ� true, ʧ�� false
*/
 //---------------------------------------------------------------------------
struct idevicesupport
{
	virtual bool	__cdecl onLoad			(void)															= 0;
	virtual bool	__cdecl onExit			(void)															= 0;

	virtual int 	__cdecl getDevices		(void)															= 0;

	virtual	bool	__cdecl synTime			(int vIndex)													= 0;
	virtual bool	__cdecl changeToDisk 	(int vIndex)													= 0;

	//��Ҫ�ļ������Ϣ
    virtual bool    __cdecl getMarkPoint 	(int vDrvIndex, char** vMarkPoint)								= 0;
    virtual bool    __cdecl getMarkPath 	(int vDrvIndex, char** vMarkPaths)								= 0;
	
	//GPS��Ϣ��ȡ
    virtual bool    __cdecl getGPSContent	(int vDrvIndex, char** vGPS)								    = 0;
    virtual bool    __cdecl getGPSPath 	    (int vDrvIndex, char** vGPSPaths)								= 0;	

	//��־��ȡ
	virtual bool    __cdecl getLogPath 		(int vDrvIndex, char** vLogPaths)								= 0;
	virtual bool	__cdecl getLogContent   (int vDrvIndex, char** vLogs)									= 0;
	virtual void	__cdecl	freeRes			(char* resBuf)													= 0;
};
//---------------------------------------------------------------------------
#endif
