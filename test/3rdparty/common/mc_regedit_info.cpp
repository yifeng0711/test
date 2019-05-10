//---------------------------------------------------------------------------
#include "mc_regedit_info.h"
#include "mc_log.h"
#include <QDebug>
#include <QApplication>
//---------------------------------------------------------------------------
MC_Regedit_Info MyRegeditInfo;
//---------------------------------------------------------------------------
MC_Regedit_Info::MC_Regedit_Info()
{
    m_Avaible   = false;
}
//---------------------------------------------------------------------------
MC_Regedit_Info::~MC_Regedit_Info()
{
    m_Avaible   = false;
}
//---------------------------------------------------------------------------
bool MC_Regedit_Info::Initialize()
{
    m_Avaible   = true;
    return true;
}
//---------------------------------------------------------------------------
bool MC_Regedit_Info::UnInitialize()
{
    m_Avaible   = false;
    return true;
}
//---------------------------------------------------------------------------
bool MC_Regedit_Info::GetRegeditSid(QString vMac, QString& vClienSid)
{
    if(false == m_Avaible)  return false;
    if("" == vMac)          return false;
    vClienSid  = "";

    //本地授权信息 注册表([HKEY_LOCAL_MACHINE\\Software\\GOLDMSG\\WSClient\\)
    if(false == MC_RegHelper::RegKeyExist(CLIENTREG))
    {
        qDebug()<<"Debug==>>MC_Regedit_Info::GetRegeditSid.　RegKeyExist： "<<CLIENTREG<<" fail.";
        return false;
    }

    BYTE tRegister[1024]    = {0};
    DWORD tSize             = 0;
    if(false == MC_RegHelper::RegValueGetB(CLIENTREG, "register", tRegister, &tSize))
    {
        qDebug()<<QObject::tr("Debug==>>MC_Regedit_Info::GetRegeditSid.　MC_RegHelper::RegValueGetB  errcode :%1 ").arg(GetLastError());
        return false;
    }

    KKBuffer tKKBuffer;
    char tBuf[BUFFSIZE] = {0};
    if(0 != tSize)
    {
        if(tSize > BUFFSIZE)
            return false;
        memcpy(tBuf, tRegister, tSize);
        tKKBuffer._len = BUFFSIZE;
        tKKBuffer._dat = tBuf;
        if(false == kinkoo_SMS4.DecryptBuffer(&tKKBuffer, SMS4KEY))
        {
            qDebug()<<QObject::tr("Debug==>>MC_Regedit_Info::GetRegeditSid.　kinkoo_SMS4.DecryptBuffer tBuf(%1) errcode :%2 ").arg(tBuf).arg(GetLastError());
            return false;
        }
    }

    QString tMac        = QString(tBuf).section("#",0,0);
    QString tClientSid  = QString(tBuf).section("#",1,1);    
    gLog.Log(QObject::tr("Debug==>>MC_Regedit_Info::GetRegeditSid. [vMac:(%1),tMac:(%2),tClientSid:(%3),tKKBuffer._dat:(%4)]").arg(vMac).arg(tMac).arg(tClientSid).arg(tKKBuffer._dat).toUtf8().data());

    /*比较MAC是否相同*/
    if(vMac != tMac)
    {
        qDebug()<<"Debug==>>MC_Regedit_Info::GetRegeditSid. Mac is not same";
        return false;
    }

    vClienSid = tClientSid;
    return true;
}
//---------------------------------------------------------------------------
bool MC_Regedit_Info::SetRegeditSid(QString vMac, QString &vClientSid)
{
    if(false == m_Avaible)                  return false;

    //不存在创建
    if(false == MC_RegHelper::RegKeyExist(CLIENTREG))
    {
        if(false == MC_RegHelper::RegKeyAdd(CLIENTREG, false))
        {
            qDebug()<<QObject::tr("Debug==>>MC_Regedit_Info::SetRegeditSid. RegKeyAdd:(%1) errorCode:(%2)").arg(CLIENTREG).arg(GetLastError());
            return false;
        }
    }

    /*将MAC地址和客户端标示加密*/
    QString tRegister   = vMac + "#" + vClientSid;
    char tBuf[BUFFSIZE] = {0};
    memcpy(tBuf, tRegister.toStdString().c_str(), tRegister.length() + 1);
    KKBuffer tKKBuffer;
    tKKBuffer._len = BUFFSIZE;
    tKKBuffer._dat = tBuf;
    if(false == kinkoo_SMS4.EncryptBuffer(&tKKBuffer, SMS4KEY))
    {
        qDebug()<<QObject::tr("Debug==>>MC_Regedit_Info::SetRegeditSid. kinkoo_SMS4.EncryptBuffer tBuf:(%1) errorCode:(%2)").arg(tBuf).arg(GetLastError());
        return false;
    }

    if(false == MC_RegHelper::RegValueSet(CLIENTREG, "register", tBuf, REG_BINARY, BUFFSIZE))
    {
        qDebug()<<QObject::tr("Debug==>>MC_Regedit_Info::SetRegeditSid. RegValueSet client_sid:(%1) errorCode:(%2)").arg(vClientSid).arg(GetLastError());
        return false;
    }

    //将运行目录写入注册表
    QString tPath = GetParrentPath();
    qDebug()<<"----------------------------------------------------------------------------------tPath : "<<tPath;
    if("" == tPath)
    {
        qDebug()<<QObject::tr("Debug==>>MC_Regedit_Info::SetRegeditSid. GetParrentPath errorCode:(%1)").arg(GetLastError());
        return false;
    }

    if(false == MC_RegHelper::RegValueSetS(CLIENTREG, "installdir", (char*)tPath.toStdString().c_str()))
    {
        qDebug()<<QObject::tr("Debug==>>MC_Regedit_Info::SetRegeditSid. RegValueSetS(installdir : %1) errorCode:(%2)").arg(tPath).arg(GetLastError());
        return false;

    }
    return true;
}
//---------------------------------------------------------------------------
bool MC_Regedit_Info::ClientRegValueSet(QString vKey, QString vValue)
{
    //不存在创建
    if(false == MC_RegHelper::RegKeyExist(CLIENTREG))
    {
        if(false == MC_RegHelper::RegKeyAdd(CLIENTREG, false))
        {
            qDebug()<< "Debug==>>MC_Regedit_Info::ClientRegValueSet fail.RegKeyAdd: "<<CLIENTREG<<", ErrCode: "<<GetLastError();
            return false;
        }
    }

    if(false == MC_RegHelper::RegValueSet(CLIENTREG, vKey.toUtf8().data(), vValue.toUtf8().data(), REG_SZ, vValue.length() + 1))
    {
        qDebug()<< "Debug==>>MC_Regedit_Info::ClientRegValueSet fail.RegValueSet value: "<<vValue<<", ErrCode: "<<GetLastError();
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
QString MC_Regedit_Info::ClientRegValueGet(QString vKey)
{
    if(false == m_Avaible)      return "";
    if("" == vKey)              return "";

    if(false == MC_RegHelper::RegKeyExist(CLIENTREG))
    {
        qDebug()<<"Debug==>> MC_Regedit_Info::ClientRegValueGet.RegKeyExist: 0"<<CLIENTREG<<", ErrCode: "<<GetLastError();
        return "";

    }
    std::string tValue        = MC_RegHelper::RegValueGet(CLIENTREG, vKey.toUtf8().data());
    return tValue.c_str();
}
//---------------------------------------------------------------------------
QString MC_Regedit_Info::GetParrentPath()
{
    QString tCurPath        = QApplication::applicationDirPath();
    QString tParrentPath    = tCurPath;
    QString tPath           = "";
    for(int i=0; i<4; ++i)
    {
        tPath       = tCurPath.left(tCurPath.lastIndexOf('/'));
        tCurPath    = tPath;
        tPath       = "";
    }

    return tCurPath;
}
//---------------------------------------------------------------------------
