//---------------------------------------------------------------------------
#include "mc_syshelper.h"
//---------------------------------------------------------------------------
MC_SysHelper MySysHelper;
//---------------------------------------------------------------------------
MC_SysHelper::MC_SysHelper()
{
}
//---------------------------------------------------------------------------
bool MC_SysHelper::Init(/*QString vStorePath*/)
{
    GetNetWorkInfo();
//    if(false == GetStorageInfo(vStorePath.toLocal8Bit().data()))
//    {
//        qDebug()<<"Debug==>>MC_SysHelper::Init. GetStorageInfo return fail.path = null";
//        return false;
//    }
    return true;
}
//---------------------------------------------------------------------------
void MC_SysHelper::UnInit()
{
}
//---------------------------------------------------------------------------
std::string MC_SysHelper::GetMyComputerName()
{
    char tComputerName[256] = {0};
    DWORD tLen                = 256;
    BOOL tRet = GetComputerNameA(tComputerName, &tLen);
    if(0 == tRet)
    {
        char temp[256] = {0};
        sprintf_s(temp, sizeof(temp), "MC_SysHelper::GetMyComputerName GetComputerName Error : %d ", GetLastError());
        OutputDebugStringA(temp);
        return "";
    }
    return std::string(tComputerName);
}
//---------------------------------------------------------------------------
std::string MC_SysHelper::GetMac()
{
    return m_Mac.toStdString().c_str();
}
//---------------------------------------------------------------------------
std::string MC_SysHelper::GetIp()
{
    return m_Ip.toStdString().c_str();
}
//---------------------------------------------------------------------------
std::string MC_SysHelper::GetGateWay()
{
    std::string tGateWayStr;
    ULONG       tOutBufLen			= sizeof(IP_ADAPTER_INFO);
    PIP_ADAPTER_INFO pAdapterInfo	= (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
    if(pAdapterInfo == NULL)
        return false;

    if(GetAdaptersInfo(pAdapterInfo, &tOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *)malloc(tOutBufLen);
        if (pAdapterInfo == NULL)
            return false;
    }

    if(GetAdaptersInfo(pAdapterInfo, &tOutBufLen) == NO_ERROR)
    {
        for(PIP_ADAPTER_INFO pAdapter = pAdapterInfo; pAdapter != NULL; pAdapter = pAdapter->Next)
        {
            // 确保是以太网
            if(pAdapter->Type != MIB_IF_TYPE_ETHERNET)
                continue;
            // 确保MAC地址的长度为 00-00-00-00-00-00
            if(pAdapter->AddressLength != 6)
                continue;

            // IP地址与本机IP地址不匹配
            if(strcmp(pAdapter->IpAddressList.IpAddress.String, m_Ip.toStdString().c_str()) != 0)
                continue;

            tGateWayStr     = pAdapter->GatewayList.IpAddress.String;
            break;
        }
    }
    free(pAdapterInfo);
    return tGateWayStr;
}
//---------------------------------------------------------------------------
int MC_SysHelper::GetTotalStorage()
{
    return m_TotalStorage;

}
//---------------------------------------------------------------------------
int MC_SysHelper::GetRemainStorage()
{
    return m_RemainStorage;
}
//---------------------------------------------------------------------------
std::string MC_SysHelper::GetSysVersion()
{
    SYSTEM_INFO info;                //用SYSTEM_INFO结构判断64位AMD处理器
    GetSystemInfo(&info);            //调用GetSystemInfo函数填充结构
    OSVERSIONINFOEX os;
    os.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);
    if(GetVersionEx((OSVERSIONINFO *)&os))
    {
        std::string vname;

        //下面根据版本信息判断操作系统名称
        switch(os.dwMajorVersion){                        //判断主版本号
            case 4:
                switch(os.dwMinorVersion){                //判断次版本号
                    case 0:
                        if(os.dwPlatformId==VER_PLATFORM_WIN32_NT)
                            vname= "Microsoft Windows NT 4.0";  //1996年7月发布
                        else if(os.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
                            vname= "Microsoft Windows 95";
                        break;
                    case 10:
                        vname= "Microsoft Windows 98";
                        break;
                    case 90:
                        vname= "Microsoft Windows Me";
                        break;
                }
                break;

            case 5:
                switch(os.dwMinorVersion){               //再比较dwMinorVersion的值
                    case 0:
                        vname=  "Microsoft Windows 2000";    //1999年12月发布
                        break;
                    case 1:
                        vname=  "Microsoft Windows XP";      //2001年8月发布
                        break;
                    case 2:
                        if(os.wProductType==VER_NT_WORKSTATION &&
                           info.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
                        vname=  "Microsoft Windows XP Professional x64 Edition";
                    else if(GetSystemMetrics(SM_SERVERR2)==0)
                        vname= "Microsoft Windows Server 2003";   //2003年3月发布
                    else if(GetSystemMetrics(SM_SERVERR2)!=0)
                        vname=  "Microsoft Windows Server 2003 R2";
                    break;
                }
                 break;

            case 6:
                switch(os.dwMinorVersion){
                    case 0:
                        if(os.wProductType==VER_NT_WORKSTATION)
                            vname= "Microsoft Windows Vista";
                        else
                            vname= "Microsoft Windows Server 2008";   //服务器版本
                        break;
                    case 1:
                        if(os.wProductType==VER_NT_WORKSTATION)
                            vname=  "Microsoft Windows 7";
                        else
                            vname= "Microsoft Windows Server 2008 R2";
                        break;
                    case 2:
                    vname=  "Microsoft Windows 7";
                    break;
                }
                break;

            default:
                vname= "未知操作系统";
            }
        return vname;
    }
    else
        return "";
}
//---------------------------------------------------------------------------
std::string MC_SysHelper::GetWsVersion()
{
    return "1.26.10";
}
//---------------------------------------------------------------------------
bool MC_SysHelper::GetNetWorkInfo()
{
    QList<QNetworkInterface>  NetList = QNetworkInterface::allInterfaces();//获取所有网卡信息
    QNetworkInterface thisNet;
    for(int Neti = 0;Neti < NetList.count(); Neti++){
        thisNet = NetList[Neti];

        if(thisNet.isValid() && (thisNet.flags() & QNetworkInterface::IsUp) && !thisNet.addressEntries().isEmpty())
        {
            m_Mac = thisNet.hardwareAddress();
            break;
        }
    }
    QList<QNetworkAddressEntry> entryList = thisNet.addressEntries();
    QNetworkAddressEntry thisEntry;
    for(int i=0; i < entryList.count(); ++i)
    {
        thisEntry = entryList[i];
        if (thisEntry.ip() != QHostAddress::LocalHost && thisEntry.ip().toIPv4Address())
        {
            m_Ip        = thisEntry.ip().toString();
            break;
        }
    }
    return true;

}
//---------------------------------------------------------------------------
bool MC_SysHelper::GetStorageInfo(const char* vRootPath)
{

    ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
    if( !GetDiskFreeSpaceExA(vRootPath, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes))
    {
        qDebug() << "ERROR: Call to GetDiskFreeSpaceEx() failed.";
        return false;
    }
    m_TotalStorage  = (quint64) liTotalBytes.QuadPart/1024/1024;
    m_RemainStorage = (quint64) liTotalFreeBytes.QuadPart/1024/1024;
    return true;
}
//---------------------------------------------------------------------------
std::string MC_SysHelper::GetOSVerSion()
{
    return "Window 7";
}
//---------------------------------------------------------------------------
