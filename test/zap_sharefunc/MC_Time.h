//---------------------------------------------------------------------------
#ifndef	MC_Time_H
#define MC_Time_H
//---------------------------------------------------------------------------
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//---------------------------------------------------------------------------
#include <string>
using namespace std;
//---------------------------------------------------------------------------
#ifndef KKString
#define KKString(_X_)	(char*)(_X_.c_str())
#endif
//---------------------------------------------------------------------------
class MC_Time
{
public:
	static	std::string		GetCurrTime				(std::string vTimePos = ":");	//当前时间，默认格式：00:00:00
	static	std::string 	GetCurrDate				(std::string vDatePos = "-");	//当前日期，默认格式：0000-00-00
	static	std::string 	GetCurrDateTime			(std::string vDatePos = "-", std::string vDtPos = " ", std::string vTimePos = ":"); //当前日期+时间，默认格式：0000-00-00 00:00:00
	static	std::string 	GetCurrTimeWithWeek		();

	static	__int64			GetTimestamp			(std::string vTimes="");	//获取时间戳
	static	std::string		FmtTimestamp			(__int64 vTimestamp);		//时间戳转格式化时间

	static	std::string		FmtDateTime				(const std::string& vDateTime);	//转换时间间隔符

	static	std::string		GetNowTime				(void);		//当前时间，跟GetCurrTime结果一样
	static	std::string		GetLastWeek				(void);		//获取七天前的日期，比如当前为周一，那么获取到的是上周一的日期

private:
	static	std::string		STRReplace				(const std::string& vString, const std::string& vSubStrFrom, const std::string& vSubStrTo);
};
//---------------------------------------------------------------------------
#endif	
//---------------------------------------------------------------------------
/*
cout << MC_Time::GetCurrTime() << endl;
cout << MC_Time::GetCurrDate() << endl;
cout << MC_Time::GetCurrDateTime() << endl;
cout << MC_Time::GetCurrTimeWithWeek() << endl;

__int64 tTimeStamp =  MC_Time::GetTimestamp() ;
cout << MC_Time::GetTimestamp() << endl;
cout << MC_Time::FmtTimestamp(tTimeStamp) << endl;

cout << MC_Time::FmtDateTime("2019-24-15 15:15:15") << endl;

cout << MC_Time::GetNowTime() << endl;
cout << MC_Time::GetLastWeek() << endl;
 */