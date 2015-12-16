#pragma once

/*
* 将获取到的行情数据存内存
*/
#include "PublicFuncs.h"


class LocalMemory
{
public:
	static void Initialize(bool isWriteDB);
	static void Destroy(void);
	// 初始化MarketData的存储空间和输出文件
	static void InitFemasMarketData();

public:
	// 为MarketData开辟的存储空间
	// 数组femasMarketData[instrumentNum]中存储的是FemasMarketData类的指针，可以通过该指针访问对应的存储空间
	static FemasMarketData* femasMarketData[instrumentNum];
	// MdSpi的OnRtnDepthMarketData接收到的MarketData将存入此队列中，然后由LocalMemory线程函数处理
	static queue<CUstpFtdcDepthMarketDataField> newFemasMarketDataQueue;
	// newMarketDataQueue的临界区
	static CRITICAL_SECTION critSec_MdQue;
	// 事件：新MarketData，用于MdSpi通知LocalMemory
	static HANDLE hEventNewMd;
	// 是否存储MarketData到文件（后期可改为数据库）
	static bool m_bIsWriteDB;
	// LocalMemory的线程handle
	static HANDLE hThread_LocalMemory;
	// 映射表，将合约名映射到其下标，方便查找
	static map<string, int> instrumentMap;
};

// 将hh:mm:ss类型的时间字符串转换为整数hhmmss
long timeToInt(char *t);
// LocalMemory的线程起始函数
DWORD WINAPI ThreadFunc_LocalMemory(LPVOID pParam);

