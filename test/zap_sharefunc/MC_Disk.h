#ifndef MC_Disk_H
#define MC_Disk_H
//---------------------------------------------------------------------------
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//---------------------------------------------------------------------------
#include <vector>
#include <string>
using namespace std;
//---------------------------------------------------------------------------
class MC_Disk
{
public:
	static	int				GetDiskCount				(void);
	static	bool			GetDiskType					(void);
	static	bool			GetDiskSpace				(const char* vPath, __int64& vAllKB, __int64& vUsedKB, __int64& vFreeKB);	//合法的参数：NULL, C:, C:\\test 
	static	std::string		GetSizeFormat				(__int64 vSize);

	static	bool			GetSpaceInfo				(const char* vPath);	

	static	bool			CheckDiskWirteable			(const int vIndex);	//检测磁盘是否可写

	//驱动器挂载点操作		MountPoint 必须以 "\\"结尾
	static	bool			GetMountPointsList			(std::vector<std::string>& vMountPointList, std::string vWildcard="");	//获取系统挂载点 vWildcard表示匹配的关键字
	static	bool			ListMountPointOfVolume		(const char* vVolumeName, std::vector<std::string>& vMountPointList, std::string vWildcard="");	//挂载点查询

	static	bool			DeleteMountPoint			(const char* vMountPoint);
	static	bool			ChangeMountPoint			(const char* vOldMountPoint, const char* vNewMountPoint);

	static	std::string		GetDiskVolumeName			(const int vIndex);	
	static	std::string		GetDriverString				(const char* vVolumeName);	//盘符, 返回值 C:\\;
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------