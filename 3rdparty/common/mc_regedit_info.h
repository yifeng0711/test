//---------------------------------------------------------------------------
#ifndef MC_REGEDIT_INFO_H
#define MC_REGEDIT_INFO_H
//---------------------------------------------------------------------------
#include <QString>
#include "MC_KKSMS4.h"
#include "MC_RegHelper.hpp"
//---------------------------------------------------------------------------
#define CLIENTREG   "HKEY_LOCAL_MACHINE\\Software\\GOLDMSG\\WSClient\\"
#define SMS4KEY     "123456789ABCDEF"
#define	BUFFSIZE    16 * 8
//---------------------------------------------------------------------------
class MC_Regedit_Info
{
public:
    MC_Regedit_Info             (void);
    ~MC_Regedit_Info            (void);

    bool    Initialize          (void);
    bool    UnInitialize        (void);

    /*判断是否注册  false 未注册 , true 已经注册并返回客户端信息*/
    bool    GetRegeditSid       (QString vMac, QString& vClienSid);
    bool    SetRegeditSid       (QString vMac, QString& vClientSid);

    //供客户端注入注册表相关信息
    bool    ClientRegValueSet   (QString vKey, QString vValue);
    QString ClientRegValueGet   (QString vKey);


private:
    QString GetParrentPath      (void);

private:
    bool    m_Avaible;
};
//---------------------------------------------------------------------------
extern MC_Regedit_Info MyRegeditInfo;
//---------------------------------------------------------------------------
#endif // MC_REGEDIT_INFO_H
//---------------------------------------------------------------------------
