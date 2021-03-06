// PublicFuncs.h: interface for the PublicFuncs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUBLICFUNCS_H__F565A242_2630_4908_B2B8_011FDAA45C89__INCLUDED_)
#define AFX_PUBLICFUNCS_H__F565A242_2630_4908_B2B8_011FDAA45C89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <memory.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <vector>
#include <queue>
#include <string>
#include <stack>

#ifdef WIN32
#include <windows.h>
typedef HANDLE THREAD_HANDLE ;
#else
#include <pthread.h>
typedef pthread_t THREAD_HANDLE ;
#endif
const int BUFLEN=512;



// added by Chen Le
// system header
#include <tchar.h>
#include <fstream>
#include <iostream>

using namespace std;

#pragma warning(disable : 4996)

// Femas
#include "FemasAPI/USTPFtdcMduserApi.h"
#include "FemasAPI/USTPFtdcTraderApi.h"
#include "FemasAPI/USTPFtdcUserApiDataType.h"
#include "FemasAPI/USTPFtdcUserApiStruct.h"

#include "SystemCore/Global.h"
#include "MySpi/femasMdSpi.h"
#include "MySpi/femasTraderSpi.h"
#include "MarketData/FemasMarketData.h"


// Strategy
#include "Strategy/Strategy.h"
#include "Strategy/StrategyTask.h"

// System Core
#include "SystemCore/LocalMemory.h"
#include "SystemCore/OrderManageSys.h"

#endif // !defined(AFX_PUBLICFUNCS_H__F565A242_2630_4908_B2B8_011FDAA45C89__INCLUDED_)
