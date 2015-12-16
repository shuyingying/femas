#include "PublicFuncs.h"

int Strategy::firstOrderWaitTime;
int Strategy::virtualFeedWaitTime;

int realCounter = 0;
int virtualCounter = 0;

Strategy::Strategy(void)
{
	diffCursor = -1;
	timeStamp = 0;
	memset(&newFeed, 0, sizeof(newFeed));
	memset(&feed1, 0, sizeof(feed1));
	memset(&feed2, 0, sizeof(feed2));
	memset(&virtualFeed, 0, sizeof(virtualFeed));
	is2ndFeedArrived = false;
	openShortPrice = 0;
	openLongPrice = 0;
	posInd = 0;
	isRunStrategy = true;
	window_length = windowLengthMax / 2;

	hEventFeed = CreateEvent(NULL,						// ��ȫ���ԣ�Ĭ��
							TRUE,						// ��λ��ʽ���˹���λ
							FALSE,						// ��ʼ״̬�����ź�
							NULL);	
	hEventOnRtnOrder = CreateEvent(NULL,						// ��ȫ���ԣ�Ĭ��
									TRUE,						// ��λ��ʽ���˹���λ
									FALSE,						// ��ʼ״̬�����ź�
									NULL);	
	hEventOnRtnTrade = CreateEvent(NULL,						// ��ȫ���ԣ�Ĭ��
									TRUE,						// ��λ��ʽ���˹���λ
									FALSE,						// ��ʼ״̬�����ź�
									NULL);

	hObject[0] = hEventFeed;
	hObject[1] = hEventOnRtnOrder;
	hObject[2] = hEventOnRtnTrade;

	// ��ʼ��Critical section
	InitializeCriticalSection(&critSec_FeedQue);
	InitializeCriticalSection(&critSec_RtnOrderQue);
	InitializeCriticalSection(&critSec_RtnTradeQue);

	// tradeSpecialCaseNo == 0 ���������м۵�
	// ��������������Լ�����
}

Strategy::~Strategy(void)
{
}

// �趨priceTick���µ��ж���ӵļ۸�tick��
void Strategy::SetPriceTick(double _price_tick, int _add_units_immediate_deal, int _add_units_entry, int _add_units_exit, 
							int _add_units_stopLoss, int _add_units_forceStop)
{
	price_tick = _price_tick;
	add_units_immediate_deal = _add_units_immediate_deal;
	add_units_entry = _add_units_entry;
	add_units_exit = _add_units_exit;
	add_units_stopLoss = _add_units_stopLoss;
	add_units_forceStop = _add_units_forceStop;
}

// �趨������صĲ���
void Strategy::SetStrategyParam(int _window_length, int UnitsCoefOpenPosi, int UnitsCoefStopLoss, int UnitsCoefStopLossProt, 
								int UnitsCoefForceStop, int Units_diff_thre1, int Units_diff_thre2, int _stopWaitTime, 
								int _firstOrderWaitTime, int _virtualFeedWaitTime)
{
	window_length = _window_length;
	coefOpenPosi = UnitsCoefOpenPosi * price_tick;
	coefStopLoss = UnitsCoefStopLoss * price_tick;
	coefStopLossProt = UnitsCoefStopLossProt * price_tick;
	coefForceStop = UnitsCoefForceStop * price_tick;
	diff_thre1 = Units_diff_thre1 * price_tick;
	diff_thre2 = Units_diff_thre2 * price_tick;
	stopWaitTime = _stopWaitTime;
	firstOrderWaitTime = _firstOrderWaitTime;
	virtualFeedWaitTime = _virtualFeedWaitTime;
}

// �趨�����������isTest��_instrumentID�Զ�ѡ����Բ���������runThread֮ǰ����SetParamters
void Strategy::SetParamters(bool isTest, char* _instrumentID, double _price_tick)
{
	SetPriceTick(_price_tick,			// save price_tick
		150,							// add_units_immediate_deal
		-100,								// add_units_entry
		10,								// add_units_exit
		20,								// add_units_stopLoss
		25);							// add_units_forceStop

	char instrument[3];
	strncpy(instrument, _instrumentID, 2);
	instrument[2] = '\0';
	if (strcmp("IF", instrument) == 0)
	{	
		if (isTest)
		{
			SetStrategyParam(20,			// window_length
							0,				// ������������λ��ticks
							50,				// ֹ������
							25,				// ֹ�𱣻�
							100,			// ǿ��ֹ��
							5,				// ��һ��Լ�̿�
							15,				// �ڶ���Լ�̿�
							300,			// ֹ��ȴ�ʱ�䣬��λ��s
							200,			// ��һ��������ȴ�ʱ�䣬֮�󳷵�����λ��ms
							5);				// ��һ��feed�����ȴ�ʱ�䣬֮��������feed
		}
		else
		{
			SetStrategyParam(100, 6, 25, 5, 64, 5, 15, 300, 200, 5);
		}
	}
	else if (strcmp("ni", instrument) == 0)
	{
		if (isTest)
		{// modify
			SetStrategyParam(20, -5, 10, 25, 100, 5, 15, 300, 200, 5);
		}
		else
		{// modify
			SetStrategyParam(100, 6, 25, 5, 64, 5, 15, 300, 200, 5);
		}
	}
	else if (strcmp("rb", instrument) == 0)
	{
		// ...
	}
	

}

// �����̵߳���ʼ����������Ϊһ�����������߳̽��Ӹú�����ʼ����
// ��ͬ�Ĳ��Զ�����ԴӲ�ͬ�ĺ�����ʼ���У�Ҳ���Դ���ͬ�ĺ�����ʼ�������໥Ӱ��
void Strategy::runThread(LPTHREAD_START_ROUTINE pfnThreadProc)
{
	// Handle ��Ҫclose
	hThread_St = CreateThread(NULL,  
		0,
		pfnThreadProc,
		this,
		0,								// or CREATE_SUSPENDED
		NULL);
}

// �����̵߳���ʼ������Ҳ����Ϊ�������Զ�����������ʼ������ֻҪ�����������ݸ�runThread����
DWORD WINAPI ThreadFunc_St(LPVOID pParam){
	// ��ȡ�����Strategy��ָ��
	Strategy* st = (Strategy*) pParam;
	while(1)  	
	{    
		DWORD dw = WaitForMultipleObjects(3, st->hObject, FALSE, INFINITE);     

		switch(dw)
		{
		case WAIT_FAILED:
			printf("ThreadFunc_St: WaitForMultipleObjects failed!\n");
			break;
		case WAIT_TIMEOUT:
			printf("ThreadFunc_St: WaitForMultipleObjects timeout!\n");
			break;
		case WAIT_OBJECT_0: // feed (set, need reset, each strategy has its own feed)
			st->DealWithFeed();
			break;
		case WAIT_OBJECT_0 + 1: // OnRtnOrder (set, need reset)
			// update local order table
			st->DealWithRtnOrder();
			break;
		case WAIT_OBJECT_0 + 2: // OnRtnTrade (set, need reset)
			// update local order table
			st->DealWithRtnTrade();
			break;
		default:;
		}
	}
	return 0;
}

// �����¼�Feed���Ӻ��������಻�����أ�����ֻ������UpdateMD4St��RunStrategy
void Strategy::DealWithFeed(void)
{
	while(1)
	{
		// ��ȡ����Feed
		EnterCriticalSection(&critSec_FeedQue);
		newFeed = FeedQue.front();
		LeaveCriticalSection(&critSec_FeedQue);
		// UpdateMD4St���²����е�Feed���ݣ����ж��Ƿ����ִ�в��Ե�����
		if (UpdateMD4St() && OrderManageSys::m_bIsRunStrategy)
		{
			// �ж��µ����������ɶ����ź�
			RunStrategy();
			OrderManageSys::m_bIsRunStrategy = false;
		}
		// pop top and deal with rest (if any)
		EnterCriticalSection(&critSec_FeedQue);
		FeedQue.pop();
		if (0 == FeedQue.size())
		{
			ResetEvent(hEventFeed);
			LeaveCriticalSection(&critSec_FeedQue);
			break;
		}
		LeaveCriticalSection(&critSec_FeedQue);
	}
}

// �����¼�OnRtnOrder���Ӻ��������಻������
void Strategy::DealWithRtnOrder(void)
{
	int orderRef, i;
	pCMyOrder pOrder;
	CUstpFtdcOrderField* pRtnOrder2;
	while(1)
		{
			// ��ȡ����RtnOrder
			EnterCriticalSection(&critSec_RtnOrderQue);
			pRtnOrder2 = &RtnOrderQue2.front();
			LeaveCriticalSection(&critSec_RtnOrderQue);
			orderRef = atoi(pRtnOrder2->UserOrderLocalID);
			// ��localOrderTable��Ѱ��orderRef��ͬ�ļ�¼�������ұȽϿ�
			for (i = localOrderTable.size() - 1; i>= 0; i--)
			{
				pOrder = &localOrderTable[i];
				if (orderRef == pOrder->orderRef)
				{
					if (0 == pOrder->responseTime)
					{
						// ��¼��һ��RtnOrder��ʱ��
						QueryPerformanceCounter(&performanceCounter);
						pOrder->responseTime = performanceCounter.QuadPart;
					}
					// ���¶���״̬
					pOrder->status = pRtnOrder2->OrderStatus;
					break; // break for
				}
			}
			printf("%s",localOrderTable.size());
			if (i == -1)
			{
				printf("OnRtnOrder: Order not found in the local orderTable!\n");
			}
			// �������صĶ�RtnOrder�Ķ��⴦������������ʵ��
			StrategyOnRtnOrder(pOrder, pRtnOrder2);
			// pop top and deal with rest (if any)
			EnterCriticalSection(&critSec_RtnOrderQue);
			RtnOrderQue2.pop();
			if (0 == RtnOrderQue2.size())
			{
				ResetEvent(hEventOnRtnOrder);
				LeaveCriticalSection(&critSec_RtnOrderQue);
				break; // break while
			}
			LeaveCriticalSection(&critSec_RtnOrderQue);
		}	
}

// �����¼�OnRtnTrade���Ӻ��������಻������
void Strategy::DealWithRtnTrade(void)
{
	int orderRef, i;
	pCMyOrder pOrder;
	CUstpFtdcTradeField* pRtnTrade2;
	while(1)
		{
			// ��ȡ����RtnTrade
			EnterCriticalSection(&critSec_RtnTradeQue);
			pRtnTrade2 = &RtnTradeQue2.front();
			LeaveCriticalSection(&critSec_RtnTradeQue);
			orderRef = atoi(pRtnTrade2->UserOrderLocalID);
			// ��localOrderTable��Ѱ��orderRef��ͬ�ļ�¼�������ұȽϿ�
			for (i = localOrderTable.size() - 1; i>= 0; i--)
			{
				pOrder = &localOrderTable[i];
				if (orderRef == pOrder->orderRef)
				{
					// ��¼RtnTrade��ʱ��
					QueryPerformanceCounter(&performanceCounter);
					// update localOrderTable
					pOrder->tradeTime = performanceCounter.QuadPart;
					pOrder->tradePrice = pRtnTrade2->TradePrice;
					pOrder->status = USTP_FTDC_OS_AllTraded;
					printf("OnRtnTrade: OrderRef= %d traded\n", orderRef);
					break; // break for
				}
			}
			if (i == -1)
			{
			//	printf("OnRtnTrade: Order not found in the local orderTable!\n");
			}
			// �������صĶ�RtnTrade�Ķ��⴦������������ʵ��
			StrategyOnRtnTrade(pOrder, pRtnTrade2);
			// pop top and deal with rest (if any)
			EnterCriticalSection(&critSec_RtnTradeQue);
			RtnTradeQue2.pop();
			if (0 == RtnTradeQue2.size())
			{
				ResetEvent(hEventOnRtnTrade);
				LeaveCriticalSection(&critSec_RtnTradeQue);
				break; // break while
			}
			LeaveCriticalSection(&critSec_RtnTradeQue);
		}
}


// ���
double sum(double num[], int n)
{
	double sum = 0;
	for(int i=0; i<n; i++)
	{
		sum += num[i];
	}
	return sum;
}

// ƽ��ֵ
double average(double num[], int n)
{
	double sum = 0;
	for(int i=0; i<n; i++)
	{
		sum += num[i];
	}
	return sum/n;
}

// ��׼��
double stdev(double num[], int n)
{
	double ave = average(num, n);
	double sum = 0;
	for(int i=0; i<n; i++)
	{
		sum += (num[i] - ave) * (num[i] - ave);
	}
	return sqrt(sum/(n-1));
}

// ��ʽ���۸񣬱���һλС��
double formatPrice(double price)
{
	int price_int = (int)(price * 10 + 0.5);
	if (price_int < 0)
	{
		price_int--;
	}
	return (double)price_int / 10;
}

// �̺߳������ȴ�virtualFeedWaitTime���룬���ڶ���feed��û��������ǰһ��tick��feed��Ϊ����feed���Դ�������
DWORD WINAPI ThreadFunc_WaitAndSendVirtualFeed(LPVOID pParam)
{
	// ��ȡ�����Strategy��ָ��
	Strategy* pSt = (Strategy*) pParam;
	// �ȴ�virtualFeedWaitTime����
	Sleep(Strategy::virtualFeedWaitTime);
	// �ڶ���feed��û��
	if (!pSt->is2ndFeedArrived)
	{
		// ��������feed����ʵfeed����֮����¼���������ٴ�������
		EnterCriticalSection(&pSt->critSec_FeedQue);
		pSt->FeedQue.push(pSt->virtualFeed);
		SetEvent(pSt->hEventFeed);
		LeaveCriticalSection(&pSt->critSec_FeedQue);
		// ����debug
		virtualCounter++;
	}
	else
	{
		// ����debug
		realCounter++;
	}
	printf("---------------------virtualCounter= %d, realCounter= %d\n", virtualCounter, realCounter);
	return 0;
}