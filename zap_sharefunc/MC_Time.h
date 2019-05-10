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
	static	std::string		GetCurrTime				(std::string vTimePos = ":");	//��ǰʱ�䣬Ĭ�ϸ�ʽ��00:00:00
	static	std::string 	GetCurrDate				(std::string vDatePos = "-");	//��ǰ���ڣ�Ĭ�ϸ�ʽ��0000-00-00
	static	std::string 	GetCurrDateTime			(std::string vDatePos = "-", std::string vDtPos = " ", std::string vTimePos = ":"); //��ǰ����+ʱ�䣬Ĭ�ϸ�ʽ��0000-00-00 00:00:00
	static	std::string 	GetCurrTimeWithWeek		();

	static	__int64			GetTimestamp			(std::string vTimes="");	//��ȡʱ���
	static	std::string		FmtTimestamp			(__int64 vTimestamp);		//ʱ���ת��ʽ��ʱ��

	static	std::string		FmtDateTime				(const std::string& vDateTime);	//ת��ʱ������

	static	std::string		GetNowTime				(void);		//��ǰʱ�䣬��GetCurrTime���һ��
	static	std::string		GetLastWeek				(void);		//��ȡ����ǰ�����ڣ����統ǰΪ��һ����ô��ȡ����������һ������

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