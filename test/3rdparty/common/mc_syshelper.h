#ifndef MC_SYSHELPER_H
#define MC_SYSHELPER_H
//---------------------------------------------------------------------------
#include <Windows.h>
#include <string>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QHostInfo>
#include <iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
//---------------------------------------------------------------------------
class MC_SysHelper
{
public:
    MC_SysHelper();
    bool	Init	(void);
    void	UnInit	(void);

    std::string     GetMyComputerName	(void);
    std::string     GetMac              (void);
    std::string     GetIp               (void);
    std::string     GetGateWay          (void);
    std::string     GetOSVerSion        (void);
    std::string     GetSysVersion       (void);
    std::string     GetWsVersion        (void);
    int             GetTotalStorage     (void);
    int             GetRemainStorage    (void);

private:
    bool            GetNetWorkInfo      (void);
    bool            GetStorageInfo      (const char* vRootPath);

private:
    bool            m_Avalible;
    QString         m_Ip;
    QString         m_Mac;
    int             m_TotalStorage;
    int             m_RemainStorage;
};
//---------------------------------------------------------------------------
extern MC_SysHelper MySysHelper;
//---------------------------------------------------------------------------
#endif // MC_SYSHELPER_H
//---------------------------------------------------------------------------
