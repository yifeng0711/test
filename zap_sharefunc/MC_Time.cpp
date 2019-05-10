#include "MC_Time.h"

#include <time.h>
#include <map>
#include <Windows.h>
//---------------------------------------------------------------------------
std::string MC_Time::GetCurrTime(std::string vTimePos)
{
	time_t tSystemTime	= time(NULL);
	struct tm tTime;
	char tTimeBuf[128];

	if(0 != localtime_s(&tTime, &tSystemTime))
	{
		return "";
	}
	sprintf_s(tTimeBuf, sizeof(tTimeBuf), "%02d%s%02d%s%02d", 
		tTime.tm_hour, vTimePos.c_str(), tTime.tm_min, vTimePos.c_str(), tTime.tm_sec);

	return std::string(tTimeBuf);
}
//---------------------------------------------------------------------------
std::string MC_Time::GetCurrDate(std::string vDatePos)
{
	time_t tSystemTime	= time(NULL);
	struct tm tTime;
	char tTimeBuf[128];

	if(0 != localtime_s(&tTime, &tSystemTime))
	{
		return "";
	}
	sprintf_s(tTimeBuf, sizeof(tTimeBuf), "%04d%s%02d%s%02d", 
		tTime.tm_year+1900, vDatePos.c_str(), tTime.tm_mon+1, vDatePos.c_str(), tTime.tm_mday);

	return std::string(tTimeBuf);
}
//---------------------------------------------------------------------------
std::string MC_Time::GetCurrDateTime(std::string vDatePos, std::string vDtPos, std::string vTimePos)
{
	time_t tSystemTime	= time(NULL);
	struct tm tTime;
	char tTimeBuf[128];

	if(0 != localtime_s(&tTime, &tSystemTime))
	{
		return "";
	}
	sprintf_s(tTimeBuf, sizeof(tTimeBuf), "%04d%s%02d%s%02d%s%02d%s%02d%s%02d", 
		tTime.tm_year+1900, vDatePos.c_str(), tTime.tm_mon+1, vDatePos.c_str(), tTime.tm_mday, 
		vDtPos.c_str(), tTime.tm_hour, vTimePos.c_str(), tTime.tm_min, vTimePos.c_str(), tTime.tm_sec);

	return std::string(tTimeBuf);
}
//---------------------------------------------------------------------------
std::string MC_Time::GetCurrTimeWithWeek()
{
	std::map<int, std::string> tMap;
	tMap[0]	= "��";
	tMap[1] = "һ";
	tMap[2] = "��";
	tMap[3] = "��";
	tMap[4] = "��";
	tMap[5] = "��";
	tMap[6] = "��";

	SYSTEMTIME tTime;
	memset(&tTime, 0, sizeof(tTime));
	char tTimeBuf[128] = {0};

	GetLocalTime(&tTime);
	sprintf_s(tTimeBuf, sizeof(tTimeBuf), "%04u-%02u-%02u_%02u:%02u:%02u, ����%s", 
		tTime.wYear, tTime.wMonth, tTime.wDay, tTime.wHour, tTime.wMinute, tTime.wSecond, tMap[tTime.wDayOfWeek].c_str());

	return std::string(tTimeBuf);
}
//---------------------------------------------------------------------------
__int64 MC_Time::GetTimestamp(std::string vTimes)
{
	time_t tTimestamp;
	if("" == vTimes)
		time(&tTimestamp);
	else
	{
		char* tTimes = KKString(vTimes);
		tm tTm;
		int tYear	= 0;
		int tMonth	= 0;
		int tDay	= 0;
		int tHour	= 0;
		int tMinute = 0;
		int tSecond = 0;
		sscanf_s(tTimes, "%d-%d-%d %d:%d:%d", &tYear, &tMonth, &tDay, &tHour, &tMinute, &tSecond);

		tTm.tm_year	= tYear - 1900; // �꣬����tm�ṹ��洢���Ǵ�1900�꿪ʼ��ʱ�䣬����tm_yearΪint��ʱ������ȥ1900��
		tTm.tm_mon	= tMonth - 1;   // �£�����tm�ṹ����·ݴ洢��ΧΪ0-11������tm_monΪint��ʱ������ȥ1��
		tTm.tm_mday	= tDay;         // �ա�
		tTm.tm_hour	= tHour;        // ʱ��
		tTm.tm_min	= tMinute;      // �֡�
		tTm.tm_sec	= tSecond;      // �롣
		tTm.tm_isdst= 0;			// ������ʱ��
		tTimestamp = mktime(&tTm);   // ��tm�ṹ��ת����time_t��ʽ��
	}

	return tTimestamp;
}
//---------------------------------------------------------------------------
std::string MC_Time::FmtTimestamp(__int64 vTimestamp)
{
	__int64 tTimestamp = vTimestamp;
	if(-1 == vTimestamp)
		tTimestamp = GetTimestamp();

	tm *pTm = new tm;
	localtime_s(pTm, &tTimestamp);

	char tTime[32] = {0};
	strftime(tTime, sizeof(tTime), "%Y-%m-%d %H:%M:%S", pTm);

	return tTime;

}
//---------------------------------------------------------------------------
std::string MC_Time::FmtDateTime(const std::string& vDateTime)
{
	std::string tRetString = vDateTime;
	tRetString = STRReplace(tRetString, " ", "");
	tRetString = STRReplace(tRetString, ":", "");
	tRetString = STRReplace(tRetString, "-", "");
	tRetString = STRReplace(tRetString, "_", "");

	return tRetString;
}
//---------------------------------------------------------------------------
std::string MC_Time::GetNowTime()
{
	char tBuf[16] = "\0";
	time_t tSystemTime = time(NULL);
	struct tm tTime;

	if(0 != localtime_s(&tTime, &tSystemTime))
	{
		return "";
	}
	strftime(tBuf, 16, "%H:%M:%S", &tTime);

	return std::string(tBuf);
}
//---------------------------------------------------------------------------
std::string MC_Time::GetLastWeek()
{
	char tBuf[16] = "\0";
	time_t tSystemTime = time(NULL) - 24*3600*7;
	struct tm tTime;

	if(0 != localtime_s(&tTime, &tSystemTime))
	{
		return "";
	}
	strftime(tBuf, 16, "%Y-%m-%d", &tTime);

	return std::string(tBuf);
}
//---------------------------------------------------------------------------
std::string MC_Time::STRReplace(const std::string& vString, const std::string& vSubStrFrom, const std::string& vSubStrTo)
{
	std::string tString = vString;
	std::string::size_type tPos = 0;
	std::string::size_type tFormLen = vSubStrFrom.size();
	std::string::size_type tToLen = vSubStrTo.size();

	while( (tPos=tString.find(vSubStrFrom, tPos)) != std::string::npos)
	{
		tString.replace( tPos, tFormLen, vSubStrTo);
		tPos += tToLen;
	}

	return tString;
}
//---------------------------------------------------------------------------