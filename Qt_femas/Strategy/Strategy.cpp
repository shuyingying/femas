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

	hEventFeed = CreateEvent(NULL,						// 安全属性：默认
							TRUE,						// 复位方式：人工复位
							FALSE,						// 初始状态：无信号
							NULL);	
	hEventOnRtnOrder = CreateEvent(NULL,						// 安全属性：默认
									TRUE,						// 复位方式：人工复位
									FALSE,						// 初始状态：无信号
									NULL);	
	hEventOnRtnTrade = CreateEvent(NULL,						// 安全属性：默认
									TRUE,						// 复位方式：人工复位
									FALSE,						// 初始状态：无信号
									NULL);

	hObject[0] = hEventFeed;
	hObject[1] = hEventOnRtnOrder;
	hObject[2] = hEventOnRtnTrade;

	// 初始化Critical section
	InitializeCriticalSection(&critSec_FeedQue);
	InitializeCriticalSection(&critSec_RtnOrderQue);
	InitializeCriticalSection(&critSec_RtnTradeQue);

	// tradeSpecialCaseNo == 0 代表两个市价单
	// 其他情况由子类自己定义
}

Strategy::~Strategy(void)
{
}

// 设定priceTick和下单中额外加的价格tick数
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

// 设定策略相关的参数
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

// 设定参数，会根据isTest和_instrumentID自动选择策略参数；运行runThread之前需先SetParamters
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
							0,				// 开仓条件，单位：ticks
							50,				// 止损条件
							25,				// 止损保护
							100,			// 强制止损
							5,				// 第一合约盘口
							15,				// 第二合约盘口
							300,			// 止损等待时间，单位：s
							200,			// 第一单发出后等待时间，之后撤单，单位：ms
							5);				// 第一个feed到达后等待时间，之后发送虚拟feed
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

// 策略线程的起始函数，输入为一个函数名，线程将从该函数开始运行
// 不同的策略对象可以从不同的函数开始运行，也可以从相同的函数开始，不会相互影响
void Strategy::runThread(LPTHREAD_START_ROUTINE pfnThreadProc)
{
	// Handle 需要close
	hThread_St = CreateThread(NULL,  
		0,
		pfnThreadProc,
		this,
		0,								// or CREATE_SUSPENDED
		NULL);
}

// 策略线程的起始函数，也可以为其它策略定义其它的起始函数，只要将函数名传递给runThread即可
DWORD WINAPI ThreadFunc_St(LPVOID pParam){
	// 获取传入的Strategy的指针
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

// 处理事件Feed的子函数，子类不必重载；子类只需重载UpdateMD4St和RunStrategy
void Strategy::DealWithFeed(void)
{
	while(1)
	{
		// 获取队首Feed
		EnterCriticalSection(&critSec_FeedQue);
		newFeed = FeedQue.front();
		LeaveCriticalSection(&critSec_FeedQue);
		// UpdateMD4St更新策略中的Feed数据，并判断是否符合执行策略的条件
		if (UpdateMD4St() && OrderManageSys::m_bIsRunStrategy)
		{
			// 判断下单条件并生成订单信号
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

// 处理事件OnRtnOrder的子函数，子类不必重载
void Strategy::DealWithRtnOrder(void)
{
	int orderRef, i;
	pCMyOrder pOrder;
	CUstpFtdcOrderField* pRtnOrder2;
	while(1)
		{
			// 获取队首RtnOrder
			EnterCriticalSection(&critSec_RtnOrderQue);
			pRtnOrder2 = &RtnOrderQue2.front();
			LeaveCriticalSection(&critSec_RtnOrderQue);
			orderRef = atoi(pRtnOrder2->UserOrderLocalID);
			// 在localOrderTable中寻找orderRef相同的记录，倒着找比较快
			for (i = localOrderTable.size() - 1; i>= 0; i--)
			{
				pOrder = &localOrderTable[i];
				if (orderRef == pOrder->orderRef)
				{
					if (0 == pOrder->responseTime)
					{
						// 记录第一次RtnOrder的时间
						QueryPerformanceCounter(&performanceCounter);
						pOrder->responseTime = performanceCounter.QuadPart;
					}
					// 更新订单状态
					pOrder->status = pRtnOrder2->OrderStatus;
					break; // break for
				}
			}
			printf("%s",localOrderTable.size());
			if (i == -1)
			{
				printf("OnRtnOrder: Order not found in the local orderTable!\n");
			}
			// 与策略相关的对RtnOrder的额外处理，由子类重载实现
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

// 处理事件OnRtnTrade的子函数，子类不必重载
void Strategy::DealWithRtnTrade(void)
{
	int orderRef, i;
	pCMyOrder pOrder;
	CUstpFtdcTradeField* pRtnTrade2;
	while(1)
		{
			// 获取队首RtnTrade
			EnterCriticalSection(&critSec_RtnTradeQue);
			pRtnTrade2 = &RtnTradeQue2.front();
			LeaveCriticalSection(&critSec_RtnTradeQue);
			orderRef = atoi(pRtnTrade2->UserOrderLocalID);
			// 在localOrderTable中寻找orderRef相同的记录，倒着找比较快
			for (i = localOrderTable.size() - 1; i>= 0; i--)
			{
				pOrder = &localOrderTable[i];
				if (orderRef == pOrder->orderRef)
				{
					// 记录RtnTrade的时间
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
			// 与策略相关的对RtnTrade的额外处理，由子类重载实现
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


// 求和
double sum(double num[], int n)
{
	double sum = 0;
	for(int i=0; i<n; i++)
	{
		sum += num[i];
	}
	return sum;
}

// 平均值
double average(double num[], int n)
{
	double sum = 0;
	for(int i=0; i<n; i++)
	{
		sum += num[i];
	}
	return sum/n;
}

// 标准差
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

// 格式化价格，保留一位小数
double formatPrice(double price)
{
	int price_int = (int)(price * 10 + 0.5);
	if (price_int < 0)
	{
		price_int--;
	}
	return (double)price_int / 10;
}

// 线程函数，等待virtualFeedWaitTime毫秒，若第二个feed还没到，则发送前一个tick的feed作为虚拟feed，以触发策略
DWORD WINAPI ThreadFunc_WaitAndSendVirtualFeed(LPVOID pParam)
{
	// 获取传入的Strategy的指针
	Strategy* pSt = (Strategy*) pParam;
	// 等待virtualFeedWaitTime毫秒
	Sleep(Strategy::virtualFeedWaitTime);
	// 第二个feed还没到
	if (!pSt->is2ndFeedArrived)
	{
		// 发送虚拟feed，真实feed来了之后会记录，但不会再触发策略
		EnterCriticalSection(&pSt->critSec_FeedQue);
		pSt->FeedQue.push(pSt->virtualFeed);
		SetEvent(pSt->hEventFeed);
		LeaveCriticalSection(&pSt->critSec_FeedQue);
		// 用于debug
		virtualCounter++;
	}
	else
	{
		// 用于debug
		realCounter++;
	}
	printf("---------------------virtualCounter= %d, realCounter= %d\n", virtualCounter, realCounter);
	return 0;
}