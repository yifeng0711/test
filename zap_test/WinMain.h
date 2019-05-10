//-------------------------------------------------------------------------------------------
#ifndef WINMAIN_H
#define WINMAIN_H
 
#define WIN32_LEAN_AND_MEAN
//------------------------------------------------------------------------------------------
#include <Windows.h>

#include <string>
#include <iostream>
using namespace std;
//------------------------------------------------------------------------------------------
#include "interface/mi_databaseaccess.h"
#include "dllhepler/mc_dllhelper.hpp"

#include "common/FUN_Convent.h"
#include "jsoncpp/json.h"

#include "MF_CommonFunc.h"
using namespace MF_CommonFunc;

#include "MC_Log.h"

//#define SELF_DEF_LOG
//#include "MC_Log.h"
//extern MC_Log tLog;
//#define LOG_INFO(__FORMAT__, ...)		tLog.Log(_LOG("INFO",	__FORMAT__, ##__VA_ARGS__))
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
