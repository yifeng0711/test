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

	//利用GetLogicalDrives()函数可以获取系统中逻辑驱动器的数量，函数返回的是一个32位无符号整型数据。  
	DWORD tDiskInfo = GetLogicalDrives();  

	//通过循环操作查看每一位数据是否为1，如果为1则磁盘为真,如果为0则磁盘不存在。  
	while(tDiskInfo)  
	{  
		//通过位运算的逻辑与操作，判断是否为1  
		if(tDiskInfo&1)  
		{  
			tDiskCount++;  
		}  
		tDiskInfo = tDiskInfo >> 1;//通过位运算的右移操作保证每循环一次所检查的位置向右移动一位。
	}  

	return tDiskCount;
}
//---------------------------------------------------------------------------
bool MC_Disk::GetDiskType()
{
	//通过GetLogicalDriveStrings()函数获取所有驱动器字符串信息长度  
	int tDSLength = GetLogicalDriveStrings(0, NULL);  

	char* tDiskStr = new char[tDSLength];  
	memset(tDiskStr, 0, tDSLength);  

	//通过GetLogicalDriveStrings将字符串信息复制到堆区数组中,其中保存了所有驱动器的信息。  
	GetLogicalDriveStringsA(tDSLength, tDiskStr);  

	int	tType	= 0;  
	//读取各驱动器信息，由于tDiskStr内部数据格式是A:\NULLB:\NULLC:\NULL，所以tDSLength/4可以获得具体大循环范围  
	for(int i=0; i<tDSLength/4; ++i)  
	{  
		char* tStrDriver = tDiskStr + i*4;  
		tType = GetDriveTypeA(tStrDriver);	//GetDriveType函数，可以获取驱动器类型，参数为驱动器的根目录  
		switch (tType)  
		{  
		case DRIVE_FIXED:  
			{  
				printf("本地磁盘%s\n", tStrDriver);  
			}  
			break;  
		case DRIVE_CDROM:  
			{  
				printf("DVD驱动器%s\n", tStrDriver);  
			}  
			break;  
		case DRIVE_REMOVABLE:  
			{  
				printf("可移动磁盘%s\n", tStrDriver);  
			}  
			break;  
		case DRIVE_REMOTE:  
			{  
				printf("网络磁盘%s\n", tStrDriver);  
			}  
			break;  
		case DRIVE_RAMDISK:  
			{  
				printf("虚拟RAM磁盘%s\n", tStrDriver);  
			}  
		case DRIVE_NO_ROOT_DIR:
			{
				printf("无根磁盘\n");
			}
			break;  
		case DRIVE_UNKNOWN:  
			{  
				printf("未知设备\n");  
			}  
			break;  
		default:  
			printf("未知设备\n");  
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

		if(tTemp >= 1024)	//转换成TB
			sprintf_s(tResult, sizeof(tResult), "%0.2lfTB", (double)vSize / 1024 / 1024 / 1024 / 1024);

		else	//转换成GB
			sprintf_s(tResult, sizeof(tResult), "%0.2lfGB", (double)vSize / 1024 / 1024 / 1024);
	}
	//转换成MB
	else if (vSize >= 1024 * 1024)
	{
		sprintf_s(tResult, sizeof(tResult), "%0.2lfMB", (double)vSize / 1024 / 1024);
	}
	//转换成KB
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
	//得出磁盘的可用空间
    DWORD dwTotalClusters;//总的簇
    DWORD dwFreeClusters;//可用的簇
    DWORD dwSectPerClust;//每个簇有多少个扇区
    DWORD dwBytesPerSect;//每个扇区有多少个字节
    BOOL bResult = GetDiskFreeSpace(TEXT("C:"), &dwSectPerClust, &dwBytesPerSect, &dwFreeClusters, &dwTotalClusters);
    if(bResult)
	{
        cout << "使用GetDiskFreeSpace函数获取磁盘空间信息" << endl;
        cout << "总簇数量: " << dwTotalClusters << endl;
        cout << "可用的簇: " << dwFreeClusters << endl;
        cout << "每个簇有多少个扇区: " << dwSectPerClust << endl;
        cout << "每个扇区有多少个字节: " <<  dwBytesPerSect << endl;
        cout << "磁盘总容量: " <<  dwTotalClusters * (DWORD64)dwSectPerClust * (DWORD64)dwBytesPerSect << endl;
        cout << "磁盘空闲容量: " << dwFreeClusters * (DWORD64)dwSectPerClust * (DWORD64)dwBytesPerSect << endl;
    }
    cout << "\n\n" << endl;
 
    DWORD64 qwFreeBytes, qwFreeBytesToCaller, qwTotalBytes;
    bResult = GetDiskFreeSpaceEx(TEXT("C:"), (PULARGE_INTEGER)&qwFreeBytesToCaller, (PULARGE_INTEGER)&qwTotalBytes, (PULARGE_INTEGER)&qwFreeBytes);
    if(bResult)
	{
        cout << "使用GetDiskFreeSpaceEx函数获取磁盘空间信息" << endl;
        cout << "磁盘总容量: " <<  qwTotalBytes << endl;
        cout << "可用的磁盘空闲容量: " << qwFreeBytes << endl;
        cout << "磁盘空闲容量: " << qwFreeBytesToCaller << endl;
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
		LOG_ERROR("驱动器中没有磁盘，请在驱动器%c中插入磁盘", 'A' + vIndex);
		return false;
	}
	CloseHandle(tDevHandle);

	//OPEN_EXISTING
	sprintf(tPath, "%c:\\check.txt", 'A' + vIndex);
	HANDLE tHandle = CreateFileA(tPath, 0, 0, 0, CREATE_ALWAYS, FILE_FLAG_DELETE_ON_CLOSE|FILE_ATTRIBUTE_HIDDEN, 0);
	if(tHandle == 0 || INVALID_HANDLE_VALUE == tHandle)
	{
		LOG_ERROR("磁盘%c不可读写", 'A' + vIndex);
		return false;
	}
	CloseHandle(tHandle);

	return true;
}
//---------------------------------------------------------------------------
bool MC_Disk::GetMountPointsList(std::vector<std::string>& vMountPointList, std::string vWildcard/* = */)
{
	char	tVolumeName[MAX_PATH]	= {0};	//驱动器 名称
	HANDLE	tVolNameHandle			= FindFirstVolumeA(tVolumeName, MAX_PATH);	//查找主机上第一个驱动器，返回驱动器名
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

	char	tDriverString[MAX_PATH]	= {0};	//驱动器 盘符，(C:\\)
	DWORD	tReturnLen				= 0;
	if(FALSE == GetVolumePathNamesForVolumeNameA(vVolumeName, tDriverString, MAX_PATH, &tReturnLen))
	{
		LOG_ERROR("GetVolumePathNamesForVolumeNameA fail! errno: %d", GetLastError());
		return false;
	}

	char	tVolumeMountPoint[MAX_PATH] = {0};	//驱动器 挂载点
	HANDLE	tVolMountPointHandle	= FindFirstVolumeMountPointA(vVolumeName, tVolumeMountPoint, MAX_PATH);	//获取驱动器第一个挂载点
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
	if(0 == GetVolumeNameForVolumeMountPointA(vOldMountPoint, tVolumeName, MAX_PATH))	//获取驱动器名称
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

	char	tVolumeName[MAX_PATH]	= {0};	//驱动器 名称
	HANDLE	tVolNameHandle			= FindFirstVolumeA(tVolumeName, MAX_PATH);	//查找主机上第一个驱动器，返回驱动器名
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

	char	tDriverString[MAX_PATH]	= {0};	//驱动器 盘符
	DWORD	tReturnLen				= 0;

	if(FALSE == GetVolumePathNamesForVolumeNameA(vVolumeName, tDriverString, MAX_PATH, &tReturnLen))
	{
		LOG_ERROR("GetVolumePathNamesForVolumeNameA fail! errno: %d", GetLastError());
		return false;
	}

	return std::string(tDriverString);
}
//---------------------------------------------------------------------------