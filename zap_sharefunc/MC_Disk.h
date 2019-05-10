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
	static	bool			GetDiskSpace				(const char* vPath, __int64& vAllKB, __int64& vUsedKB, __int64& vFreeKB);	//�Ϸ��Ĳ�����NULL, C:, C:\\test 
	static	std::string		GetSizeFormat				(__int64 vSize);

	static	bool			GetSpaceInfo				(const char* vPath);	

	static	bool			CheckDiskWirteable			(const int vIndex);	//�������Ƿ��д

	//���������ص����		MountPoint ������ "\\"��β
	static	bool			GetMountPointsList			(std::vector<std::string>& vMountPointList, std::string vWildcard="");	//��ȡϵͳ���ص� vWildcard��ʾƥ��Ĺؼ���
	static	bool			ListMountPointOfVolume		(const char* vVolumeName, std::vector<std::string>& vMountPointList, std::string vWildcard="");	//���ص��ѯ

	static	bool			DeleteMountPoint			(const char* vMountPoint);
	static	bool			ChangeMountPoint			(const char* vOldMountPoint, const char* vNewMountPoint);

	static	std::string		GetDiskVolumeName			(const int vIndex);	
	static	std::string		GetDriverString				(const char* vVolumeName);	//�̷�, ����ֵ C:\\;
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------