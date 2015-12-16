#include "PublicFuncs.h"

// 初始化所有static变量
FemasMarketData* LocalMemory::femasMarketData[instrumentNum] = {};
queue<CUstpFtdcDepthMarketDataField> LocalMemory::newFemasMarketDataQueue;
CRITICAL_SECTION LocalMemory::critSec_MdQue;
bool LocalMemory::m_bIsWriteDB = false;
map<string, int> LocalMemory::instrumentMap;

HANDLE LocalMemory::hEventNewMd = CreateEvent(NULL,								// 安全属性：默认
											  TRUE,								// 复位方式：人工复位
											  FALSE,							// 初始状态：无信号
											  NULL);

HANDLE LocalMemory::hThread_LocalMemory = CreateThread(NULL,  
														0,
														ThreadFunc_LocalMemory,
														NULL,
														0,										// or CREATE_SUSPENDED
														NULL);


void LocalMemory::Initialize(bool isWriteDB)
{
	m_bIsWriteDB = isWriteDB;
	
}

void LocalMemory::Destroy(void)
{
	for (int i=0; i<instrumentNum; i++)
	{
		delete femasMarketData[i];
	}
}


// 初始化MarketData的存储空间和输出文件
void LocalMemory::InitFemasMarketData()
{
	printf("------->> %s\n", __FUNCTION__);
	// 获取当前时间
	struct tm *local;
	time_t t = time(NULL);
	local = localtime(&t);
	char fileName[128];

	for (int i=0; i<instrumentNum; i++)
	{

		// 为每个MarketData开辟一片内存空间
		femasMarketData[i] = new FemasMarketData(ppInstrumentID[i], m_bIsWriteDB);
		// 映射表，将合约名映射到其下标，方便查找
		instrumentMap[ppInstrumentID[i]] = i;


		if (m_bIsWriteDB)
		{
			// 生成输出文件，每个MarketData一个独立的文件
			sprintf(fileName, "Femas_%s_%d%02d%02d_%02d%02d%02d.txt\0", ppInstrumentID[i], local->tm_year-100, local->tm_mon+1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
			femasMarketData[i]->mdFile.open(fileName);
		}
	}
}



// 将hh:mm:ss类型的时间字符串转换为整数hhmmss
long timeToInt(char *t)
{
	return (t[0]-'0')*100000+(t[1]-'0')*10000+(t[3]-'0')*1000+(t[4]-'0')*100+(t[6]-'0')*10+(t[7]-'0');
}

// LocalMemory的线程起始函数
DWORD WINAPI ThreadFunc_LocalMemory(LPVOID pParam)
{
	int index;
	// 初始化Critical section
	InitializeCriticalSection(&LocalMemory::critSec_MdQue);
	// 等待MdSpi通知新MarketData
	CUstpFtdcDepthMarketDataField *pMarketData = NULL;
	while(WAIT_OBJECT_0 == WaitForSingleObject(LocalMemory::hEventNewMd, INFINITE)) 
	{
			while(1)
			{
				EnterCriticalSection(&LocalMemory::critSec_MdQue);
				pMarketData = &LocalMemory::newFemasMarketDataQueue.front();	// 从队列中读取新MarketData
				LeaveCriticalSection(&LocalMemory::critSec_MdQue);
				// 通过映射表获取MarketData的下标
				index = LocalMemory::instrumentMap[pMarketData->InstrumentID];
				// 将MarketData存入内存
				LocalMemory::femasMarketData[index]->SaveData(pMarketData);
				if (pStrategyGlobal != NULL)
				{
					// 通知Strategy新的Feed，当有多策略同时运行时，需根据合约名分别通知
					// 生成新的Feed
					CMyFeed newFeed = {timeToInt(pMarketData->UpdateTime), pMarketData->UpdateMillisec, 
						index, LocalMemory::femasMarketData[index]->getListCursor()}; 
					// 将newFeed加入Strategy的Feed队列
					EnterCriticalSection(&pStrategyGlobal->critSec_FeedQue);
					pStrategyGlobal->FeedQue.push(newFeed);
					SetEvent(pStrategyGlobal->hEventFeed);
					LeaveCriticalSection(&pStrategyGlobal->critSec_FeedQue);
				}
				if (LocalMemory::m_bIsWriteDB) // 存储到文件
				{
					LocalMemory::femasMarketData[index]->WriteData();
				}
				// pop top and deal with the rest (if any)
				EnterCriticalSection(&LocalMemory::critSec_MdQue);
				LocalMemory::newFemasMarketDataQueue.pop();
				if (0 == LocalMemory::newFemasMarketDataQueue.size())
				{
					ResetEvent(LocalMemory::hEventNewMd);
					LeaveCriticalSection(&LocalMemory::critSec_MdQue);
					break; // while(1)
				}
				LeaveCriticalSection(&LocalMemory::critSec_MdQue);
			}
		}
	return 0;
}