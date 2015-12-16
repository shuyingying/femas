#include "PublicFuncs.h"

// 初始化所有static变量
int OrderManageSys::orderRef = 0;
TUstpFtdcUserOrderLocalIDType OrderManageSys::ORDER_REF = "";	//报单引用
bool OrderManageSys::m_bIsRunStrategy = true;
CUstpFtdcInputOrderField OrderManageSys::basicOrder2;
CRITICAL_SECTION OrderManageSys::critSec_OrderTable;
vector<CMyOrder> OrderManageSys::orderTable;
CRITICAL_SECTION OrderManageSys::critSec_SignalQue;
queue<CMySignal> OrderManageSys::signalQue;
CRITICAL_SECTION OrderManageSys::critSec_RtnOrderQue;
queue<CUstpFtdcOrderField> OrderManageSys::RtnOrderQue2;
CRITICAL_SECTION OrderManageSys::critSec_RtnTradeQue;
queue<CUstpFtdcTradeField> OrderManageSys::RtnTradeQue2;

//ofstream OrderManageSys::entrustFile;
HANDLE OrderManageSys::hObject[4];

// 持仓数组
vector<CMyPosition> OrderManageSys::longPosition;
CRITICAL_SECTION  OrderManageSys::critSec_LongPos;
vector<CMyPosition> OrderManageSys::shortPosition;
CRITICAL_SECTION  OrderManageSys::critSec_ShortPos;

// 资金账户
CMyAccount OrderManageSys::myAccount = {10000000, 0, 0, 0};

HANDLE OrderManageSys::hEventSignal = CreateEvent(NULL,							// 安全属性：默认
												  TRUE,							// 复位方式：人工复位
												  FALSE,						// 初始状态：无信号
												  NULL);	 
HANDLE OrderManageSys::hEventOutput = CreateEvent(NULL,							// 安全属性：默认
												  TRUE,							// 复位方式：人工复位
												  FALSE,						// 初始状态：无信号
												  NULL);
HANDLE OrderManageSys::hEventOnRtnOrder = CreateEvent(NULL,						// 安全属性：默认
												  TRUE,							// 复位方式：人工复位
												  FALSE,						// 初始状态：无信号
												  NULL);	
HANDLE OrderManageSys::hEventOnRtnTrade = CreateEvent(NULL,						// 安全属性：默认
												  TRUE,							// 复位方式：人工复位
												  FALSE,						// 初始状态：无信号
												  NULL);	
// Handle 需要close
HANDLE OrderManageSys::hThread_OMS = CreateThread(NULL,  
	0,
	ThreadFunc_OMS,
	NULL,
	0,								// 立即执行
	NULL);


// OrderManageSystem的线程起始函数
DWORD WINAPI ThreadFunc_OMS(LPVOID pParam)
{
	// 初始化交易的输出文件
	struct tm *local;
	time_t t = time(NULL);
	local = localtime(&t);
	//char fileName[128];

	//sprintf(fileName, "Entrust_%d%02d%02d_%02d%02d%02d.txt\0", local->tm_year-100, local->tm_mon+1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
	//OrderManageSys::entrustFile.open(fileName);

	// 设置WaitForMultipleObjects所等待的事件
	OrderManageSys::hObject[0] = OrderManageSys::hEventSignal;
	OrderManageSys::hObject[1] = OrderManageSys::hEventOutput;
	OrderManageSys::hObject[2] = OrderManageSys::hEventOnRtnOrder;
	OrderManageSys::hObject[3] = OrderManageSys::hEventOnRtnTrade;

	// 初始化Critical section
	InitializeCriticalSection(&OrderManageSys::critSec_OrderTable);
	InitializeCriticalSection(&OrderManageSys::critSec_SignalQue);
	InitializeCriticalSection(&OrderManageSys::critSec_RtnOrderQue);
	InitializeCriticalSection(&OrderManageSys::critSec_RtnTradeQue);
	InitializeCriticalSection(&OrderManageSys::critSec_LongPos);
	InitializeCriticalSection(&OrderManageSys::critSec_ShortPos);


	// 初始化持仓数组
	for (int i=0; i<instrumentNum; i++)
	{
		CMyPosition newPosition = {0, 0, 0};
		OrderManageSys::longPosition.push_back(newPosition);
		OrderManageSys::shortPosition.push_back(newPosition);
	}

	int orderRefs, i;
	pCMySignal pSignal;
	pCMyOrder pOrder;
	Strategy* pStrategy;
	while(1)  	
	{    
		DWORD dw = WaitForMultipleObjects(4, OrderManageSys::hObject, FALSE, INFINITE);     

		switch(dw)
		{
		case WAIT_FAILED:
			printf("ThreadFunc_OMS: WaitForMultipleObjects failed!\n");
			break;
		case WAIT_TIMEOUT:
			printf("ThreadFunc_OMS: WaitForMultipleObjects timeout!\n");
			break;
		case WAIT_OBJECT_0: // signal
			while(1)
			{
				// 获取队首的signal
				EnterCriticalSection(&OrderManageSys::critSec_SignalQue);
				pSignal = &OrderManageSys::signalQue.front();
				LeaveCriticalSection(&OrderManageSys::critSec_SignalQue);
				// place order
				OrderManageSys::MyOrderInsert(pSignal->orderPriceType, pSignal->direction, pSignal->operation, 
					pSignal->instrumentID, pSignal->volume, pSignal->price);
				// pop top and deal with rest (if any)
				EnterCriticalSection(&OrderManageSys::critSec_SignalQue);
				OrderManageSys::signalQue.pop();
				if (0 == OrderManageSys::signalQue.size())
				{
					ResetEvent(OrderManageSys::hEventSignal);
					LeaveCriticalSection(&OrderManageSys::critSec_SignalQue);
					break; // break while
				}
				LeaveCriticalSection(&OrderManageSys::critSec_SignalQue);
			}
			break; // break case
		case WAIT_OBJECT_0 + 1: // Output
			// has been put in OnRtnTrade, not very good
			break;
		case WAIT_OBJECT_0 + 2: // OnRtnOrder
			// record RtnOrder
				while(1)
				{
					// 获取队首的RtnOrder
					EnterCriticalSection(&OrderManageSys::critSec_RtnOrderQue);
					CUstpFtdcOrderField* pRtnOrder = &OrderManageSys::RtnOrderQue2.front();
					LeaveCriticalSection(&OrderManageSys::critSec_RtnOrderQue);
					orderRefs = atoi(pRtnOrder->UserOrderLocalID);
					// 在orderTable中寻找orderRef相同的记录，倒着找比较快
					for (i = OrderManageSys::orderTable.size() - 1; i>= 0; i--)
					{
						pOrder = &(OrderManageSys::orderTable[i]);
						// pOrder对应的策略
						pStrategy = pOrder->pStrategy;
						if (orderRefs == pOrder->orderRef)
						{
							//// 将pRtnOrder添加到策略的RtnOrderQue，由策略进一步处理
							//EnterCriticalSection(&pStrategy->critSec_RtnOrderQue);
							//pStrategy->RtnOrderQue2.push(*pRtnOrder);
							//SetEvent(pStrategy->hEventOnRtnOrder);		// inform the Strategy
							//LeaveCriticalSection(&pStrategy->critSec_RtnOrderQue);
							if (0 == pOrder->responseTime)
							{
								// 记录第一次RtnOrder的时间
								QueryPerformanceCounter(&performanceCounter);
								pOrder->responseTime = performanceCounter.QuadPart;
							}
							// update order table
							pOrder->status = pRtnOrder->OrderStatus;
							pOrder->insertTime = pRtnOrder->InsertTime;

							if (pOrder->status == USTP_FTDC_OS_Canceled)
							{
								if (pRtnOrder->OffsetFlag == CONST_FUTURE_Exit)
								{
									// 更新持仓，平仓单撤单成功，恢复可平数量
									OrderManageSys::UpdatePosition(LocalMemory::instrumentMap[pRtnOrder->InstrumentID], pRtnOrder->Direction, 
										0, pRtnOrder->LimitPrice, pRtnOrder->Volume);
								}
							}
							break; // break for
						}
					}
					if (i == -1)
					{
						printf("OnRtnOrder: Order not found in the orderTable!\n");
					}
					// pop top and deal with rest (if any)
					EnterCriticalSection(&OrderManageSys::critSec_RtnOrderQue);
					OrderManageSys::RtnOrderQue2.pop();
					if (0 == OrderManageSys::RtnOrderQue2.size())
					{
						ResetEvent(OrderManageSys::hEventOnRtnOrder);
						LeaveCriticalSection(&OrderManageSys::critSec_RtnOrderQue);
						break; // break while
					}
					LeaveCriticalSection(&OrderManageSys::critSec_RtnOrderQue);
				}
			break; // break case
		case WAIT_OBJECT_0 + 3: // OnRtnTrade
			// record RtnTrade
			while(1)
				{
					// 获取队首的RtnTrade
					EnterCriticalSection(&OrderManageSys::critSec_RtnTradeQue);
					CUstpFtdcTradeField* pRtnTrade = &OrderManageSys::RtnTradeQue2.front();
					LeaveCriticalSection(&OrderManageSys::critSec_RtnTradeQue);
					orderRefs = atoi(pRtnTrade->UserOrderLocalID);
					// 在orderTable中寻找orderRef相同的记录，倒着找比较快
					for (i = OrderManageSys::orderTable.size() - 1; i>= 0; i--)
					{
						pOrder = &OrderManageSys::orderTable[i];
						// pOrder对应的策略
						pStrategy = pOrder->pStrategy;
						if (orderRefs == pOrder->orderRef)
						{
							//// 将pRtnTrade添加到策略的RtnTradeQue，由策略进一步处理
							//EnterCriticalSection(&pStrategy->critSec_RtnTradeQue);
							//pStrategy->RtnTradeQue2.push(*pRtnTrade);
							//SetEvent(pStrategy->hEventOnRtnTrade);		// inform the Strategy
							//LeaveCriticalSection(&pStrategy->critSec_RtnTradeQue);

							// 记录RtnTrade的时间
							QueryPerformanceCounter(&performanceCounter);
							// update order table
							pOrder->tradeTime = performanceCounter.QuadPart;
							pOrder->tradePrice = pRtnTrade->TradePrice;
							pOrder->tradedQuan += pRtnTrade->TradeVolume;
							pOrder->successTime = pRtnTrade->TradeTime;
							pOrder->tradeID = pRtnTrade->TradeID;
							// OnRtnTrade 不更新状态，由OnRtnOrder更新
								
							// 第三参数不为0
							// 第三参数判断开仓还是平仓，开仓的话，成交量加到持仓量上；平仓的话从持仓量中减掉成交量。
							// 第五参数判断开仓还是平仓，开仓的话，成交量加到可平量上；平仓的话不变，因为在委托平仓单的时候已经减掉了
							// 更新持仓
							OrderManageSys::UpdatePosition(LocalMemory::instrumentMap[pRtnTrade->InstrumentID], pRtnTrade->Direction, 
								pRtnTrade->OffsetFlag == CONST_FUTURE_Entry ? pRtnTrade->TradeVolume : - pRtnTrade->TradeVolume, 
								pRtnTrade->TradePrice, pRtnTrade->OffsetFlag == CONST_FUTURE_Entry ? pRtnTrade->TradeVolume : 0);

							break; // break for
						}
					}
					if ( i == -1)
					{
						printf("OnRtnTrade: Order not found in the orderTable!\n");
					}
					// it's not very good to let OrderManageSys do output, it costs too much time
					OrderManageSys::OutputTrade(pOrder);
					// pop top and deal with rest (if any)
					EnterCriticalSection(&OrderManageSys::critSec_RtnTradeQue);
					OrderManageSys::RtnTradeQue2.pop();
					if (0 == OrderManageSys::RtnTradeQue2.size())
					{
						ResetEvent(OrderManageSys::hEventOnRtnTrade);
						LeaveCriticalSection(&OrderManageSys::critSec_RtnTradeQue);
						break; // break while
					}
					LeaveCriticalSection(&OrderManageSys::critSec_RtnTradeQue);
					break;
				}
			break; // break case			
		default:;
		}
	}
	return 0;
}


// 输出交易信息到文件
void OrderManageSys::OutputTrade(pCMyOrder pOrder)
{
	/*
	// 计算两个时间差，timeDiff1为发单到收到第一个RtnOrder，timeDiff2为收到第一个RtnOrder到收到RtnTrade
	double timeDiff1 = (double) ((pOrder->responseTime - pOrder->entrustTime) * 1000000 / frequency);
	double timeDiff2 = (double) ((pOrder->tradeTime - pOrder->responseTime) * 1000000 / frequency);
	// 计算滑点
	double slip = formatPrice(pOrder->direction == CONST_DIRECTION_Long ? pOrder->theoPrice - pOrder->tradePrice : pOrder->tradePrice - pOrder->theoPrice);
	// 输出交易信息到文件
	printf("Ref= %d  ID= %s  Oper= %c  Dire= %c  RtnDelay= %g  TradeDelay= %g  Slip= %g\n", 
		pOrder->orderRef, pOrder->instrumentID, pOrder->operation, pOrder->direction, timeDiff1, timeDiff2, slip);
	OrderManageSys::entrustFile << "OrderRef= " << pOrder->orderRef << "  InstrumentID= " << pOrder->instrumentID << "  Operation= " << pOrder->operation 
		<< "  Direction= " << pOrder->direction << "  RtnDelay= " << timeDiff1 << "  TradeDelay= " << timeDiff2 << "  Slip= " << slip 
		<< "  TheoPrice= " << pOrder->theoPrice << "  TradePrice= " << pOrder->tradePrice << endl;
    */
}

// 下单函数封装，根据priceType分别执行MyOrderInsertAnyPrice、MyOrderInsertLimitPrice或MyOrderInsertFOK
bool OrderManageSys::MyOrderInsert(char priceType, char long_short, char entry_exit, const char* instrumentID, int volume, double price)
{
	// 检查仓位
	if (entry_exit == CONST_FUTURE_Exit)
	{
		int positionVolume;
		if (long_short == CONST_DIRECTION_Long)
		{
			EnterCriticalSection(&critSec_LongPos);
			positionVolume = longPosition[LocalMemory::instrumentMap[instrumentID]].volume;
			LeaveCriticalSection(&critSec_LongPos);
		}
		else
		{
			EnterCriticalSection(&critSec_ShortPos);
			positionVolume = shortPosition[LocalMemory::instrumentMap[instrumentID]].volume;
			LeaveCriticalSection(&critSec_ShortPos);
		}

		if (volume > positionVolume)
		{
			printf("MyOrderInsert: No enough position to close!\n");
		}
		else
		{
			UpdatePosition(LocalMemory::instrumentMap[instrumentID], long_short, 0, price, -volume);
		}
	}

	// 下具体类型的单子
	if (priceType == CONST_ORDER_TYPE_AnyPrice)
	{
		return MyOrderInsertAnyPrice(long_short, entry_exit, instrumentID, volume, price);
	}
	else if (priceType == CONST_ORDER_TYPE_LimitPrice)
	{
		return MyOrderInsertLimitPrice(long_short, entry_exit, instrumentID, volume, price);
	}
	else if (priceType = CONST_ORDER_TYPE_FOK)
	{
		return MyOrderInsertFOK(long_short, entry_exit, instrumentID, volume, price);
	}
	else if (priceType = CONST_ORDER_TYPE_FAK)
	{
		return MyOrderInsertFAK(long_short, entry_exit, instrumentID, volume, price);
	}
	else return 0;
}

// 下市价单
bool OrderManageSys::MyOrderInsertAnyPrice(char long_short, char entry_exit, const char* instrumentID, int volume, double price)
{
		///买卖方向: 
		basicOrder2.Direction = long_short;
		///组合开平标志: 开仓
		basicOrder2.OffsetFlag = entry_exit;
		///合约代码:
		strcpy(basicOrder2.InstrumentID, instrumentID);
		///数量: 
		basicOrder2.Volume = volume;
		///价格：
		basicOrder2.LimitPrice = price;
		///报单价格条件: 限价
		basicOrder2.OrderPriceType = USTP_FTDC_OPT_AnyPrice;
		///有效期类型：立即完成，否则撤销
		basicOrder2.TimeCondition = USTP_FTDC_TC_IOC;
		///成交量类型: 任何数量
		basicOrder2.VolumeCondition = USTP_FTDC_VC_AV;
		// increase LocalID
		sprintf(basicOrder2.UserOrderLocalID,"%012d",orderRef++);

		int iResult = pTraderApi2->ReqOrderInsert(&basicOrder2, ++iRequestID);
		printf("----->>> 报单录入请求: %s\n", ((iResult == 0) ? "成功" : "失败"));
		return (iResult == 0);
}

// 下限价单
bool OrderManageSys::MyOrderInsertLimitPrice(char long_short, char entry_exit,const char* instrumentID, int volume, double price)
{
		///买卖方向: 
		basicOrder2.Direction = long_short;
		///组合开平标志: 开仓
		basicOrder2.OffsetFlag = entry_exit;
		///合约代码:
		strcpy(basicOrder2.InstrumentID, instrumentID);
		///数量: 
		basicOrder2.Volume = volume;
		///价格：
		basicOrder2.LimitPrice = price;
		///报单价格条件: 限价
		basicOrder2.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
		///有效期类型：当日有效
		basicOrder2.TimeCondition = USTP_FTDC_TC_GFD;
		///成交量类型: 任何数量
		basicOrder2.VolumeCondition = USTP_FTDC_VC_AV;
		// increase LocalID
		sprintf(ORDER_REF,"%012d",orderRef++);
		strcpy(basicOrder2.UserOrderLocalID,ORDER_REF);
		int iResult = pTraderApi2->ReqOrderInsert(&basicOrder2, ++iRequestID);
		printf("----->>> 报单录入请求: %s\n", ((iResult == 0) ? "成功" : "失败"));
		return (iResult == 0);
}

// 下FOK单
bool OrderManageSys::MyOrderInsertFOK(char long_short, char entry_exit, const char* instrumentID, int volume, double price)
{
		///买卖方向: 
		basicOrder2.Direction = long_short;
		///组合开平标志: 开仓
		basicOrder2.OffsetFlag = entry_exit;
		///合约代码:
		strcpy(basicOrder2.InstrumentID, instrumentID);
		///数量: 
		basicOrder2.Volume = volume;
		///价格：
		basicOrder2.LimitPrice = price;
		///报单价格条件: 限价
		basicOrder2.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
		///有效期类型：当日有效
		basicOrder2.TimeCondition = USTP_FTDC_TC_IOC;
		///成交量类型: 任何数量
		basicOrder2.VolumeCondition = USTP_FTDC_VC_CV;
		sprintf(ORDER_REF,"%012d",orderRef++);
		strcpy(basicOrder2.UserOrderLocalID,ORDER_REF);
		int iResult = pTraderApi2->ReqOrderInsert(&basicOrder2, ++iRequestID);
		printf("----->>> 报单录入请求: %s\n", ((iResult == 0) ? "成功" : "失败"));
		return (iResult == 0);
}

// 下FAK单
bool OrderManageSys::MyOrderInsertFAK(char long_short, char entry_exit, const char* instrumentID, int volume, double price)
{
		///买卖方向: 
		basicOrder2.Direction = long_short;
		///组合开平标志: 开仓
		basicOrder2.OffsetFlag = entry_exit;
		///合约代码:
		strcpy(basicOrder2.InstrumentID, instrumentID);
		///数量: 
		basicOrder2.Volume = volume;
		///价格：
		basicOrder2.LimitPrice = price;
		///报单价格条件: 限价
		basicOrder2.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
		///有效期类型：当日有效
		basicOrder2.TimeCondition = USTP_FTDC_TC_IOC;
		///成交量类型: 任何数量
		basicOrder2.VolumeCondition = USTP_FTDC_VC_CV;
		// increase LocalID
		sprintf(basicOrder2.UserOrderLocalID,"%012d",orderRef++);

		int iResult = pTraderApi2->ReqOrderInsert(&basicOrder2, ++iRequestID);
		printf("----->>> 报单录入请求: %s\n", ((iResult == 0) ? "成功" : "失败"));
		return (iResult == 0);
}

// 撤单
bool OrderManageSys::MyOrderCancel(char* UserOrderLocalID)
{
		CUstpFtdcOrderActionField* pCancelOrder = new CUstpFtdcOrderActionField();
		memset(pCancelOrder, 0, sizeof(CUstpFtdcOrderActionField));

		///交易所代码
		strcpy(pCancelOrder->ExchangeID, EXCHANGEID) ;
		///报单编号
		//TUstpFtdcOrderSysIDType OrderSysID; // order唯一标示
		///经纪公司代码
		strcpy(pCancelOrder->BrokerID, BROKER_ID);
		//用户代码
		strcpy(pCancelOrder->UserID,USER_ID);
		///投资者代码
		strcpy(pCancelOrder->InvestorID, INVESTOR_ID);
		///本地报单编号
		strcpy(pCancelOrder->UserOrderLocalID, UserOrderLocalID); //被撤的订单本地报单编号
		///报单操作标志
		pCancelOrder->ActionFlag = '0'; // 0:删除
		///操作本地编号
		// increase LocalID
		sprintf(pCancelOrder->UserOrderActionLocalID, "%012d", orderRef++); //同一个序列

		int iResult = pTraderApi2->ReqOrderAction(pCancelOrder, ++iRequestID);
		printf("----->>> 报单操作请求: %s\n", ((iResult == 0) ? "成功" : "失败"));
		delete pCancelOrder;
		return (iResult == 0);
}

// Initialize femas basicOrder
void OrderManageSys::InitBasicOrder2(void)
{
	memset(&basicOrder2, 0, sizeof(basicOrder2));
	///经纪公司代码
	strcpy(basicOrder2.BrokerID, BROKER_ID);
	//用户代码
	strcpy(basicOrder2.UserID,USER_ID);
	//交易所编码
	strcpy(basicOrder2.ExchangeID,EXCHANGEID);
	///投资者代码
	strcpy(basicOrder2.InvestorID, INVESTOR_ID);
	///组合投机套保标志
	basicOrder2.HedgeFlag = '1'; // Speculation
	///最小成交量: 1
	basicOrder2.MinVolume = 1;
	///触发条件: 立即
	basicOrder2.TimeCondition = '1'; // Immediately;
	///强平原因: 非强平
	basicOrder2.ForceCloseReason = '0';
	///自动挂起标志: 否
	basicOrder2.IsAutoSuspend = 0;
}

//初始化持仓
void OrderManageSys::initPosition(int instrumentIndex, char direction, int Volume, double price, int deltaAvailableVolume)
{
	if (direction == CONST_DIRECTION_Long)
	{
		EnterCriticalSection(&critSec_LongPos);
		pCMyPosition pLongPosition = &longPosition[instrumentIndex];
		pLongPosition->volume= Volume;
		pLongPosition->availableVolume = deltaAvailableVolume;
		if (pLongPosition->volume == 0)
		{
			pLongPosition->aveCost = 0;
		}
		else
		{
			pLongPosition->aveCost = (price/pLongPosition->volume)/300;
		}
		LeaveCriticalSection(&critSec_LongPos);
	}
	else 
	{
		EnterCriticalSection(&critSec_ShortPos);
		pCMyPosition pShortPosition = &shortPosition[instrumentIndex];
		pShortPosition->volume = Volume;
		pShortPosition->availableVolume = deltaAvailableVolume;
		if (pShortPosition->volume == 0)
		{
			pShortPosition->aveCost = 0;
		}
		else
		{
			pShortPosition->aveCost = (price/pShortPosition->volume);//除以300？
		}
		LeaveCriticalSection(&critSec_ShortPos);
	}
}
// 更新持仓数组
void OrderManageSys::UpdatePosition(int instrumentIndex, char direction, int deltaVolume, double price, int deltaAvailableVolume)
{
	if (direction == CONST_DIRECTION_Long)
	{
		EnterCriticalSection(&critSec_LongPos);
		pCMyPosition pLongPosition = &longPosition[instrumentIndex];
		double cost = pLongPosition->aveCost * pLongPosition->volume;
		pLongPosition->volume += deltaVolume;
		pLongPosition->availableVolume += deltaAvailableVolume;
		if (pLongPosition->volume == 0)
		{
			pLongPosition->aveCost = 0;
		}
		else
		{
			pLongPosition->aveCost = (cost + price * deltaVolume) / pLongPosition->volume;
		}
		LeaveCriticalSection(&critSec_LongPos);
	}
	else 
	{
		EnterCriticalSection(&critSec_ShortPos);
		pCMyPosition pShortPosition = &shortPosition[instrumentIndex];
		double cost = pShortPosition->aveCost * pShortPosition->volume;
		pShortPosition->volume += deltaVolume;
		pShortPosition->availableVolume += deltaAvailableVolume;
		if (pShortPosition->volume == 0)
		{
			pShortPosition->aveCost = 0;
		}
		else
		{
			pShortPosition->aveCost = (cost + price * deltaVolume) / pShortPosition->volume;
		}
		LeaveCriticalSection(&critSec_ShortPos);
	}
}