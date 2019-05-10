#include "MC_Disk.h"
#include "MC_Log.h"
//---------------------------------------------------------------------------
#include <Windows.h>

#include <iostream>
using namespace std;
//---------------------------------------------------------------------------
int MC_Disk::GetDiskCount()
{
	DWORD tDiskCount = 0; 

	//����GetLogicalDrives()�������Ի�ȡϵͳ���߼����������������������ص���һ��32λ�޷����������ݡ�  
	DWORD tDiskInfo = GetLogicalDrives();  

	//ͨ��ѭ�������鿴ÿһλ�����Ƿ�Ϊ1�����Ϊ1�����Ϊ��,���Ϊ0����̲����ڡ�  
	while(tDiskInfo)  
	{  
		//ͨ��λ������߼���������ж��Ƿ�Ϊ1  
		if(tDiskInfo&1)  
		{  
			tDiskCount++;  
		}  
		tDiskInfo = tDiskInfo >> 1;//ͨ��λ��������Ʋ�����֤ÿѭ��һ��������λ�������ƶ�һλ��
	}  

	return tDiskCount;
}
//---------------------------------------------------------------------------
bool MC_Disk::GetDiskType()
{
	//ͨ��GetLogicalDriveStrings()������ȡ�����������ַ�����Ϣ����  
	int tDSLength = GetLogicalDriveStrings(0, NULL);  

	char* tDiskStr = new char[tDSLength];  
	memset(tDiskStr, 0, tDSLength);  

	//ͨ��GetLogicalDriveStrings���ַ�����Ϣ���Ƶ�����������,���б�������������������Ϣ��  
	GetLogicalDriveStringsA(tDSLength, tDiskStr);  

	int	tType	= 0;  
	//��ȡ����������Ϣ������tDiskStr�ڲ����ݸ�ʽ��A:\NULLB:\NULLC:\NULL������tDSLength/4���Ի�þ����ѭ����Χ  
	for(int i=0; i<tDSLength/4; ++i)  
	{  
		char* tStrDriver = tDiskStr + i*4;  
		tType = GetDriveTypeA(tStrDriver);	//GetDriveType���������Ի�ȡ���������ͣ�����Ϊ�������ĸ�Ŀ¼  
		switch (tType)  
		{  
		case DRIVE_FIXED:  
			{  
				printf("���ش���%s\n", tStrDriver);  
			}  
			break;  
		case DRIVE_CDROM:  
			{  
				printf("DVD������%s\n", tStrDriver);  
			}  
			break;  
		case DRIVE_REMOVABLE:  
			{  
				printf("���ƶ�����%s\n", tStrDriver);  
			}  
			break;  
		case DRIVE_REMOTE:  
			{  
				printf("�������%s\n", tStrDriver);  
			}  
			break;  
		case DRIVE_RAMDISK:  
			{  
				printf("����RAM����%s\n", tStrDriver);  
			}  
		case DRIVE_NO_ROOT_DIR:
			{
				printf("�޸�����\n");
			}
			break;  
		case DRIVE_UNKNOWN:  
			{  
				printf("δ֪�豸\n");  
			}  
			break;  
		default:  
			printf("δ֪�豸\n");  
			break;  
		}  
	}

	delete[] tDiskStr;
	return true;
}
//---------------------------------------------------------------------------
bool MC_Disk::GetDiskSpace(const char* vPath, __int64& vAllKB, __int64& vUsedKB, __int64& vFreeKB)
{
	ULARGE_INTEGER	tFreeBytesAvailable;
	ULARGE_INTEGER	tTotalBytes;
	ULARGE_INTEGER	tTotalFreeBytes;
	if(FALSE == GetDiskFreeSpaceExA(vPath, &tFreeBytesAvailable, &tTotalBytes, &tTotalFreeBytes))
	{
		return false;
	}
	vAllKB    = tTotalBytes.QuadPart / 1024;
	vFreeKB   = tTotalFreeBytes.QuadPart / 1024;
	vUsedKB   = vAllKB - vFreeKB;

	return true;
}
//---------------------------------------------------------------------------
std::string MC_Disk::GetSizeFormat(__int64 vSize)
{
	char tResult[64] = {0};
	//vSize *= 512;

	if (vSize >= 1024 * 1024 * 1024)
	{
		int tTemp = (int)(vSize / 1024 /1024 /1024);

		if(tTemp >= 1024)	//ת����TB
			sprintf_s(tResult, sizeof(tResult), "%0.2lfTB", (double)vSize / 1024 / 1024 / 1024 / 1024);

		else	//ת����GB
			sprintf_s(tResult, sizeof(tResult), "%0.2lfGB", (double)vSize / 1024 / 1024 / 1024);
	}
	//ת����MB
	else if (vSize >= 1024 * 1024)
	{
		sprintf_s(tResult, sizeof(tResult), "%0.2lfMB", (double)vSize / 1024 / 1024);
	}
	//ת����KB
	else if (vSize >= 1024)
	{
		sprintf_s(tResult, sizeof(tResult), "%0.2lfKB", (double)vSize / 1024);
	}
	else
	{
		//if(0 != vSize)
		sprintf_s(tResult, sizeof(tResult), "%I64dByte", vSize);
	}

	std::string tSizeFormat = std::string(tResult);
	return tSizeFormat;
}
//---------------------------------------------------------------------------
bool MC_Disk::GetSpaceInfo(const char* vPath)
{
	//�ó����̵Ŀ��ÿռ�
    DWORD dwTotalClusters;//�ܵĴ�
    DWORD dwFreeClusters;//���õĴ�
    DWORD dwSectPerClust;//ÿ�����ж��ٸ�����
    DWORD dwBytesPerSect;//ÿ�������ж��ٸ��ֽ�
    BOOL bResult = GetDiskFreeSpace(TEXT("C:"), &dwSectPerClust, &dwBytesPerSect, &dwFreeClusters, &dwTotalClusters);
    if(bResult)
	{
        cout << "ʹ��GetDiskFreeSpace������ȡ���̿ռ���Ϣ" << endl;
        cout << "�ܴ�����: " << dwTotalClusters << endl;
        cout << "���õĴ�: " << dwFreeClusters << endl;
        cout << "ÿ�����ж��ٸ�����: " << dwSectPerClust << endl;
        cout << "ÿ�������ж��ٸ��ֽ�: " <<  dwBytesPerSect << endl;
        cout << "����������: " <<  dwTotalClusters * (DWORD64)dwSectPerClust * (DWORD64)dwBytesPerSect << endl;
        cout << "���̿�������: " << dwFreeClusters * (DWORD64)dwSectPerClust * (DWORD64)dwBytesPerSect << endl;
    }
    cout << "\n\n" << endl;
 
    DWORD64 qwFreeBytes, qwFreeBytesToCaller, qwTotalBytes;
    bResult = GetDiskFreeSpaceEx(TEXT("C:"), (PULARGE_INTEGER)&qwFreeBytesToCaller, (PULARGE_INTEGER)&qwTotalBytes, (PULARGE_INTEGER)&qwFreeBytes);
    if(bResult)
	{
        cout << "ʹ��GetDiskFreeSpaceEx������ȡ���̿ռ���Ϣ" << endl;
        cout << "����������: " <<  qwTotalBytes << endl;
        cout << "���õĴ��̿�������: " << qwFreeBytes << endl;
        cout << "���̿�������: " << qwFreeBytesToCaller << endl;
    }

	return true;
}
//---------------------------------------------------------------------------
bool MC_Disk::CheckDiskWirteable(const int vIndex)
{
	if(0 > vIndex || 25 < vIndex)	
		return false;

	char tPath[512]		= {0};
	char temp[512]		= {0};
	char tDevice[512]	= {0};

	sprintf_s(tDevice, _countof(tDevice), "\\\\.\\%c:", 'A'+vIndex);
	HANDLE	tDevHandle = CreateFileA(tDevice, 0, 0, 0, CREATE_ALWAYS, FILE_FLAG_DELETE_ON_CLOSE, 0);
	DWORD	tLastError = GetLastError();
	if(INVALID_HANDLE_VALUE == tDevHandle && 87 == tLastError /*|| 21 == tLastError)*/)
	{
		LOG_ERROR("��������û�д��̣�����������%c�в������", 'A' + vIndex);
		return false;
	}
	CloseHandle(tDevHandle);

	//OPEN_EXISTING
	sprintf(tPath, "%c:\\check.txt", 'A' + vIndex);
	HANDLE tHandle = CreateFileA(tPath, 0, 0, 0, CREATE_ALWAYS, FILE_FLAG_DELETE_ON_CLOSE|FILE_ATTRIBUTE_HIDDEN, 0);
	if(tHandle == 0 || INVALID_HANDLE_VALUE == tHandle)
	{
		LOG_ERROR("����%c���ɶ�д", 'A' + vIndex);
		return false;
	}
	CloseHandle(tHandle);

	return true;
}
//---------------------------------------------------------------------------
bool MC_Disk::GetMountPointsList(std::vector<std::string>& vMountPointList, std::string vWildcard/* = */)
{
	char	tVolumeName[MAX_PATH]	= {0};	//������ ����
	HANDLE	tVolNameHandle			= FindFirstVolumeA(tVolumeName, MAX_PATH);	//���������ϵ�һ����������������������
	if(INVALID_HANDLE_VALUE == tVolNameHandle)
	{
		LOG_ERROR("tVolNameHandle invalid, errno: %d.", GetLastError());
		return false;
	}

	vMountPointList.clear();
	do 
	{
		if(false == ListMountPointOfVolume(tVolumeName, vMountPointList, vWildcard))
		{
			LOG_ERROR("ListMountPointOfVolume fail.");
			FindVolumeClose(tVolNameHandle);
			return false;
		}

	} while (FindNextVolumeA(tVolNameHandle, tVolumeName, MAX_PATH));

	FindVolumeClose(tVolNameHandle);
	return true;
}
//---------------------------------------------------------------------------
bool MC_Disk::ListMountPointOfVolume(const char* vVolumeName, std::vector<std::string>& vMountPointList, std::string vWildcard/* = */)
{
	if(0 == vVolumeName)		return false;
	if(0 == *vVolumeName)		return false;

	char	tDriverString[MAX_PATH]	= {0};	//������ �̷���(C:\\)
	DWORD	tReturnLen				= 0;
	if(FALSE == GetVolumePathNamesForVolumeNameA(vVolumeName, tDriverString, MAX_PATH, &tReturnLen))
	{
		LOG_ERROR("GetVolumePathNamesForVolumeNameA fail! errno: %d", GetLastError());
		return false;
	}

	char	tVolumeMountPoint[MAX_PATH] = {0};	//������ ���ص�
	HANDLE	tVolMountPointHandle	= FindFirstVolumeMountPointA(vVolumeName, tVolumeMountPoint, MAX_PATH);	//��ȡ��������һ�����ص�
	if(INVALID_HANDLE_VALUE == tVolMountPointHandle)
	{
		//LOG_WARN("tVolMountPointHandle invalid, errno: %d", GetLastError());
		return true;
	}

	do 
	{
		std::string tMountPoint = std::string(tDriverString) + std::string(tVolumeMountPoint);
		if(std::string::npos == tMountPoint.find("disk") || std::string::npos == tMountPoint.find(vWildcard))
		{
			continue;
		}

		vMountPointList.push_back(tMountPoint);

	} while (FindNextVolumeMountPointA(tVolMountPointHandle, tVolumeMountPoint, MAX_PATH));


	FindVolumeMountPointClose(tVolMountPointHandle);
	return true;
}
//---------------------------------------------------------------------------
bool MC_Disk::DeleteMountPoint(const char* vMountPoint)
{
	if(0 == vMountPoint)	return false;
	if(0 == *vMountPoint)	return false;

	if(0 == DeleteVolumeMountPointA(vMountPoint))
	{
		LOG_ERROR("DeleteVolumeMountPointA %s fail! errno: %d", vMountPoint, GetLastError());
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool MC_Disk::ChangeMountPoint(const char* vOldMountPoint, const char* vNewMountPoint)
{
	if(0 == vOldMountPoint)		return false;
	if(0 == *vOldMountPoint)	return false;
	if(0 == vNewMountPoint)		return false;
	if(0 == *vNewMountPoint)	return false;

	char tVolumeName[MAX_PATH]= {0};
	if(0 == GetVolumeNameForVolumeMountPointA(vOldMountPoint, tVolumeName, MAX_PATH))	//��ȡ����������
	{
		LOG_ERROR("Get Volume Name For Volume Mount Point: %s fail! errno: %d", vOldMountPoint, GetLastError());
		return false;
	}

	if(0 == SetVolumeMountPointA(vNewMountPoint, tVolumeName))
	{
		LOG_ERROR("SetVolumeMountPoint: %s fail! errno: %d", vNewMountPoint, GetLastError());
		return false;
	}

	if(false == DeleteVolumeMountPointA(vOldMountPoint))
	{
		LOG_ERROR("DeleteVolumeMountPointA: %s fail! errno: %d", vOldMountPoint, GetLastError());
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
std::string MC_Disk::GetDiskVolumeName(const int vIndex)
{
	if(0 > vIndex || 25 < vIndex)
		return "";

	char	tVolumeName[MAX_PATH]	= {0};	//������ ����
	HANDLE	tVolNameHandle			= FindFirstVolumeA(tVolumeName, MAX_PATH);	//���������ϵ�һ����������������������
	if(INVALID_HANDLE_VALUE == tVolNameHandle)
	{
		LOG_ERROR("tVolNameHandle invalid, errno: %d.", GetLastError());
		return false;
	}

	char tDriverStr[MAX_PATH] = {0};
	sprintf_s(tDriverStr, sizeof(tDriverStr), "%c:\\", 'A'+vIndex);
	
	do 
	{
		if(tDriverStr == GetDriverString(tVolumeName))
			break;

	} while (FindNextVolumeA(tVolNameHandle, tVolumeName, MAX_PATH));

	FindVolumeClose(tVolNameHandle);

	return tVolumeName;
}
//---------------------------------------------------------------------------
std::string MC_Disk::GetDriverString(const char* vVolumeName)
{
	if(0 == vVolumeName)	return false;
	if(0 == *vVolumeName)	return false;

	char	tDriverString[MAX_PATH]	= {0};	//������ �̷�
	DWORD	tReturnLen				= 0;

	if(FALSE == GetVolumePathNamesForVolumeNameA(vVolumeName, tDriverString, MAX_PATH, &tReturnLen))
	{
		LOG_ERROR("GetVolumePathNamesForVolumeNameA fail! errno: %d", GetLastError());
		return false;
	}

	return std::string(tDriverString);
}
//---------------------------------------------------------------------------