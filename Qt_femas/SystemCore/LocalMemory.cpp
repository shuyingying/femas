#include "PublicFuncs.h"

// ��ʼ������static����
FemasMarketData* LocalMemory::femasMarketData[instrumentNum] = {};
queue<CUstpFtdcDepthMarketDataField> LocalMemory::newFemasMarketDataQueue;
CRITICAL_SECTION LocalMemory::critSec_MdQue;
bool LocalMemory::m_bIsWriteDB = false;
map<string, int> LocalMemory::instrumentMap;

HANDLE LocalMemory::hEventNewMd = CreateEvent(NULL,								// ��ȫ���ԣ�Ĭ��
											  TRUE,								// ��λ��ʽ���˹���λ
											  FALSE,							// ��ʼ״̬�����ź�
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


// ��ʼ��MarketData�Ĵ洢�ռ������ļ�
void LocalMemory::InitFemasMarketData()
{
	printf("------->> %s\n", __FUNCTION__);
	// ��ȡ��ǰʱ��
	struct tm *local;
	time_t t = time(NULL);
	local = localtime(&t);
	char fileName[128];

	for (int i=0; i<instrumentNum; i++)
	{

		// Ϊÿ��MarketData����һƬ�ڴ�ռ�
		femasMarketData[i] = new FemasMarketData(ppInstrumentID[i], m_bIsWriteDB);
		// ӳ�������Լ��ӳ�䵽���±꣬�������
		instrumentMap[ppInstrumentID[i]] = i;


		if (m_bIsWriteDB)
		{
			// ��������ļ���ÿ��MarketDataһ���������ļ�
			sprintf(fileName, "Femas_%s_%d%02d%02d_%02d%02d%02d.txt\0", ppInstrumentID[i], local->tm_year-100, local->tm_mon+1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
			femasMarketData[i]->mdFile.open(fileName);
		}
	}
}



// ��hh:mm:ss���͵�ʱ���ַ���ת��Ϊ����hhmmss
long timeToInt(char *t)
{
	return (t[0]-'0')*100000+(t[1]-'0')*10000+(t[3]-'0')*1000+(t[4]-'0')*100+(t[6]-'0')*10+(t[7]-'0');
}

// LocalMemory���߳���ʼ����
DWORD WINAPI ThreadFunc_LocalMemory(LPVOID pParam)
{
	int index;
	// ��ʼ��Critical section
	InitializeCriticalSection(&LocalMemory::critSec_MdQue);
	// �ȴ�MdSpi֪ͨ��MarketData
	CUstpFtdcDepthMarketDataField *pMarketData = NULL;
	while(WAIT_OBJECT_0 == WaitForSingleObject(LocalMemory::hEventNewMd, INFINITE)) 
	{
			while(1)
			{
				EnterCriticalSection(&LocalMemory::critSec_MdQue);
				pMarketData = &LocalMemory::newFemasMarketDataQueue.front();	// �Ӷ����ж�ȡ��MarketData
				LeaveCriticalSection(&LocalMemory::critSec_MdQue);
				// ͨ��ӳ����ȡMarketData���±�
				index = LocalMemory::instrumentMap[pMarketData->InstrumentID];
				// ��MarketData�����ڴ�
				LocalMemory::femasMarketData[index]->SaveData(pMarketData);
				if (pStrategyGlobal != NULL)
				{
					// ֪ͨStrategy�µ�Feed�����ж����ͬʱ����ʱ������ݺ�Լ���ֱ�֪ͨ
					// �����µ�Feed
					CMyFeed newFeed = {timeToInt(pMarketData->UpdateTime), pMarketData->UpdateMillisec, 
						index, LocalMemory::femasMarketData[index]->getListCursor()}; 
					// ��newFeed����Strategy��Feed����
					EnterCriticalSection(&pStrategyGlobal->critSec_FeedQue);
					pStrategyGlobal->FeedQue.push(newFeed);
					SetEvent(pStrategyGlobal->hEventFeed);
					LeaveCriticalSection(&pStrategyGlobal->critSec_FeedQue);
				}
				if (LocalMemory::m_bIsWriteDB) // �洢���ļ�
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