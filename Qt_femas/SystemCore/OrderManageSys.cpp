#include "PublicFuncs.h"

// ��ʼ������static����
int OrderManageSys::orderRef = 0;
TUstpFtdcUserOrderLocalIDType OrderManageSys::ORDER_REF = "";	//��������
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

// �ֲ�����
vector<CMyPosition> OrderManageSys::longPosition;
CRITICAL_SECTION  OrderManageSys::critSec_LongPos;
vector<CMyPosition> OrderManageSys::shortPosition;
CRITICAL_SECTION  OrderManageSys::critSec_ShortPos;

// �ʽ��˻�
CMyAccount OrderManageSys::myAccount = {10000000, 0, 0, 0};

HANDLE OrderManageSys::hEventSignal = CreateEvent(NULL,							// ��ȫ���ԣ�Ĭ��
												  TRUE,							// ��λ��ʽ���˹���λ
												  FALSE,						// ��ʼ״̬�����ź�
												  NULL);	 
HANDLE OrderManageSys::hEventOutput = CreateEvent(NULL,							// ��ȫ���ԣ�Ĭ��
												  TRUE,							// ��λ��ʽ���˹���λ
												  FALSE,						// ��ʼ״̬�����ź�
												  NULL);
HANDLE OrderManageSys::hEventOnRtnOrder = CreateEvent(NULL,						// ��ȫ���ԣ�Ĭ��
												  TRUE,							// ��λ��ʽ���˹���λ
												  FALSE,						// ��ʼ״̬�����ź�
												  NULL);	
HANDLE OrderManageSys::hEventOnRtnTrade = CreateEvent(NULL,						// ��ȫ���ԣ�Ĭ��
												  TRUE,							// ��λ��ʽ���˹���λ
												  FALSE,						// ��ʼ״̬�����ź�
												  NULL);	
// Handle ��Ҫclose
HANDLE OrderManageSys::hThread_OMS = CreateThread(NULL,  
	0,
	ThreadFunc_OMS,
	NULL,
	0,								// ����ִ��
	NULL);


// OrderManageSystem���߳���ʼ����
DWORD WINAPI ThreadFunc_OMS(LPVOID pParam)
{
	// ��ʼ�����׵�����ļ�
	struct tm *local;
	time_t t = time(NULL);
	local = localtime(&t);
	//char fileName[128];

	//sprintf(fileName, "Entrust_%d%02d%02d_%02d%02d%02d.txt\0", local->tm_year-100, local->tm_mon+1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
	//OrderManageSys::entrustFile.open(fileName);

	// ����WaitForMultipleObjects���ȴ����¼�
	OrderManageSys::hObject[0] = OrderManageSys::hEventSignal;
	OrderManageSys::hObject[1] = OrderManageSys::hEventOutput;
	OrderManageSys::hObject[2] = OrderManageSys::hEventOnRtnOrder;
	OrderManageSys::hObject[3] = OrderManageSys::hEventOnRtnTrade;

	// ��ʼ��Critical section
	InitializeCriticalSection(&OrderManageSys::critSec_OrderTable);
	InitializeCriticalSection(&OrderManageSys::critSec_SignalQue);
	InitializeCriticalSection(&OrderManageSys::critSec_RtnOrderQue);
	InitializeCriticalSection(&OrderManageSys::critSec_RtnTradeQue);
	InitializeCriticalSection(&OrderManageSys::critSec_LongPos);
	InitializeCriticalSection(&OrderManageSys::critSec_ShortPos);


	// ��ʼ���ֲ�����
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
				// ��ȡ���׵�signal
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
					// ��ȡ���׵�RtnOrder
					EnterCriticalSection(&OrderManageSys::critSec_RtnOrderQue);
					CUstpFtdcOrderField* pRtnOrder = &OrderManageSys::RtnOrderQue2.front();
					LeaveCriticalSection(&OrderManageSys::critSec_RtnOrderQue);
					orderRefs = atoi(pRtnOrder->UserOrderLocalID);
					// ��orderTable��Ѱ��orderRef��ͬ�ļ�¼�������ұȽϿ�
					for (i = OrderManageSys::orderTable.size() - 1; i>= 0; i--)
					{
						pOrder = &(OrderManageSys::orderTable[i]);
						// pOrder��Ӧ�Ĳ���
						pStrategy = pOrder->pStrategy;
						if (orderRefs == pOrder->orderRef)
						{
							//// ��pRtnOrder��ӵ����Ե�RtnOrderQue���ɲ��Խ�һ������
							//EnterCriticalSection(&pStrategy->critSec_RtnOrderQue);
							//pStrategy->RtnOrderQue2.push(*pRtnOrder);
							//SetEvent(pStrategy->hEventOnRtnOrder);		// inform the Strategy
							//LeaveCriticalSection(&pStrategy->critSec_RtnOrderQue);
							if (0 == pOrder->responseTime)
							{
								// ��¼��һ��RtnOrder��ʱ��
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
									// ���³ֲ֣�ƽ�ֵ������ɹ����ָ���ƽ����
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
					// ��ȡ���׵�RtnTrade
					EnterCriticalSection(&OrderManageSys::critSec_RtnTradeQue);
					CUstpFtdcTradeField* pRtnTrade = &OrderManageSys::RtnTradeQue2.front();
					LeaveCriticalSection(&OrderManageSys::critSec_RtnTradeQue);
					orderRefs = atoi(pRtnTrade->UserOrderLocalID);
					// ��orderTable��Ѱ��orderRef��ͬ�ļ�¼�������ұȽϿ�
					for (i = OrderManageSys::orderTable.size() - 1; i>= 0; i--)
					{
						pOrder = &OrderManageSys::orderTable[i];
						// pOrder��Ӧ�Ĳ���
						pStrategy = pOrder->pStrategy;
						if (orderRefs == pOrder->orderRef)
						{
							//// ��pRtnTrade��ӵ����Ե�RtnTradeQue���ɲ��Խ�һ������
							//EnterCriticalSection(&pStrategy->critSec_RtnTradeQue);
							//pStrategy->RtnTradeQue2.push(*pRtnTrade);
							//SetEvent(pStrategy->hEventOnRtnTrade);		// inform the Strategy
							//LeaveCriticalSection(&pStrategy->critSec_RtnTradeQue);

							// ��¼RtnTrade��ʱ��
							QueryPerformanceCounter(&performanceCounter);
							// update order table
							pOrder->tradeTime = performanceCounter.QuadPart;
							pOrder->tradePrice = pRtnTrade->TradePrice;
							pOrder->tradedQuan += pRtnTrade->TradeVolume;
							pOrder->successTime = pRtnTrade->TradeTime;
							pOrder->tradeID = pRtnTrade->TradeID;
							// OnRtnTrade ������״̬����OnRtnOrder����
								
							// ����������Ϊ0
							// ���������жϿ��ֻ���ƽ�֣����ֵĻ����ɽ����ӵ��ֲ����ϣ�ƽ�ֵĻ��ӳֲ����м����ɽ�����
							// ��������жϿ��ֻ���ƽ�֣����ֵĻ����ɽ����ӵ���ƽ���ϣ�ƽ�ֵĻ����䣬��Ϊ��ί��ƽ�ֵ���ʱ���Ѿ�������
							// ���³ֲ�
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


// ���������Ϣ���ļ�
void OrderManageSys::OutputTrade(pCMyOrder pOrder)
{
	/*
	// ��������ʱ��timeDiff1Ϊ�������յ���һ��RtnOrder��timeDiff2Ϊ�յ���һ��RtnOrder���յ�RtnTrade
	double timeDiff1 = (double) ((pOrder->responseTime - pOrder->entrustTime) * 1000000 / frequency);
	double timeDiff2 = (double) ((pOrder->tradeTime - pOrder->responseTime) * 1000000 / frequency);
	// ���㻬��
	double slip = formatPrice(pOrder->direction == CONST_DIRECTION_Long ? pOrder->theoPrice - pOrder->tradePrice : pOrder->tradePrice - pOrder->theoPrice);
	// ���������Ϣ���ļ�
	printf("Ref= %d  ID= %s  Oper= %c  Dire= %c  RtnDelay= %g  TradeDelay= %g  Slip= %g\n", 
		pOrder->orderRef, pOrder->instrumentID, pOrder->operation, pOrder->direction, timeDiff1, timeDiff2, slip);
	OrderManageSys::entrustFile << "OrderRef= " << pOrder->orderRef << "  InstrumentID= " << pOrder->instrumentID << "  Operation= " << pOrder->operation 
		<< "  Direction= " << pOrder->direction << "  RtnDelay= " << timeDiff1 << "  TradeDelay= " << timeDiff2 << "  Slip= " << slip 
		<< "  TheoPrice= " << pOrder->theoPrice << "  TradePrice= " << pOrder->tradePrice << endl;
    */
}

// �µ�������װ������priceType�ֱ�ִ��MyOrderInsertAnyPrice��MyOrderInsertLimitPrice��MyOrderInsertFOK
bool OrderManageSys::MyOrderInsert(char priceType, char long_short, char entry_exit, const char* instrumentID, int volume, double price)
{
	// ����λ
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

	// �¾������͵ĵ���
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

// ���м۵�
bool OrderManageSys::MyOrderInsertAnyPrice(char long_short, char entry_exit, const char* instrumentID, int volume, double price)
{
		///��������: 
		basicOrder2.Direction = long_short;
		///��Ͽ�ƽ��־: ����
		basicOrder2.OffsetFlag = entry_exit;
		///��Լ����:
		strcpy(basicOrder2.InstrumentID, instrumentID);
		///����: 
		basicOrder2.Volume = volume;
		///�۸�
		basicOrder2.LimitPrice = price;
		///�����۸�����: �޼�
		basicOrder2.OrderPriceType = USTP_FTDC_OPT_AnyPrice;
		///��Ч�����ͣ�������ɣ�������
		basicOrder2.TimeCondition = USTP_FTDC_TC_IOC;
		///�ɽ�������: �κ�����
		basicOrder2.VolumeCondition = USTP_FTDC_VC_AV;
		// increase LocalID
		sprintf(basicOrder2.UserOrderLocalID,"%012d",orderRef++);

		int iResult = pTraderApi2->ReqOrderInsert(&basicOrder2, ++iRequestID);
		printf("----->>> ����¼������: %s\n", ((iResult == 0) ? "�ɹ�" : "ʧ��"));
		return (iResult == 0);
}

// ���޼۵�
bool OrderManageSys::MyOrderInsertLimitPrice(char long_short, char entry_exit,const char* instrumentID, int volume, double price)
{
		///��������: 
		basicOrder2.Direction = long_short;
		///��Ͽ�ƽ��־: ����
		basicOrder2.OffsetFlag = entry_exit;
		///��Լ����:
		strcpy(basicOrder2.InstrumentID, instrumentID);
		///����: 
		basicOrder2.Volume = volume;
		///�۸�
		basicOrder2.LimitPrice = price;
		///�����۸�����: �޼�
		basicOrder2.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
		///��Ч�����ͣ�������Ч
		basicOrder2.TimeCondition = USTP_FTDC_TC_GFD;
		///�ɽ�������: �κ�����
		basicOrder2.VolumeCondition = USTP_FTDC_VC_AV;
		// increase LocalID
		sprintf(ORDER_REF,"%012d",orderRef++);
		strcpy(basicOrder2.UserOrderLocalID,ORDER_REF);
		int iResult = pTraderApi2->ReqOrderInsert(&basicOrder2, ++iRequestID);
		printf("----->>> ����¼������: %s\n", ((iResult == 0) ? "�ɹ�" : "ʧ��"));
		return (iResult == 0);
}

// ��FOK��
bool OrderManageSys::MyOrderInsertFOK(char long_short, char entry_exit, const char* instrumentID, int volume, double price)
{
		///��������: 
		basicOrder2.Direction = long_short;
		///��Ͽ�ƽ��־: ����
		basicOrder2.OffsetFlag = entry_exit;
		///��Լ����:
		strcpy(basicOrder2.InstrumentID, instrumentID);
		///����: 
		basicOrder2.Volume = volume;
		///�۸�
		basicOrder2.LimitPrice = price;
		///�����۸�����: �޼�
		basicOrder2.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
		///��Ч�����ͣ�������Ч
		basicOrder2.TimeCondition = USTP_FTDC_TC_IOC;
		///�ɽ�������: �κ�����
		basicOrder2.VolumeCondition = USTP_FTDC_VC_CV;
		sprintf(ORDER_REF,"%012d",orderRef++);
		strcpy(basicOrder2.UserOrderLocalID,ORDER_REF);
		int iResult = pTraderApi2->ReqOrderInsert(&basicOrder2, ++iRequestID);
		printf("----->>> ����¼������: %s\n", ((iResult == 0) ? "�ɹ�" : "ʧ��"));
		return (iResult == 0);
}

// ��FAK��
bool OrderManageSys::MyOrderInsertFAK(char long_short, char entry_exit, const char* instrumentID, int volume, double price)
{
		///��������: 
		basicOrder2.Direction = long_short;
		///��Ͽ�ƽ��־: ����
		basicOrder2.OffsetFlag = entry_exit;
		///��Լ����:
		strcpy(basicOrder2.InstrumentID, instrumentID);
		///����: 
		basicOrder2.Volume = volume;
		///�۸�
		basicOrder2.LimitPrice = price;
		///�����۸�����: �޼�
		basicOrder2.OrderPriceType = USTP_FTDC_OPT_LimitPrice;
		///��Ч�����ͣ�������Ч
		basicOrder2.TimeCondition = USTP_FTDC_TC_IOC;
		///�ɽ�������: �κ�����
		basicOrder2.VolumeCondition = USTP_FTDC_VC_CV;
		// increase LocalID
		sprintf(basicOrder2.UserOrderLocalID,"%012d",orderRef++);

		int iResult = pTraderApi2->ReqOrderInsert(&basicOrder2, ++iRequestID);
		printf("----->>> ����¼������: %s\n", ((iResult == 0) ? "�ɹ�" : "ʧ��"));
		return (iResult == 0);
}

// ����
bool OrderManageSys::MyOrderCancel(char* UserOrderLocalID)
{
		CUstpFtdcOrderActionField* pCancelOrder = new CUstpFtdcOrderActionField();
		memset(pCancelOrder, 0, sizeof(CUstpFtdcOrderActionField));

		///����������
		strcpy(pCancelOrder->ExchangeID, EXCHANGEID) ;
		///�������
		//TUstpFtdcOrderSysIDType OrderSysID; // orderΨһ��ʾ
		///���͹�˾����
		strcpy(pCancelOrder->BrokerID, BROKER_ID);
		//�û�����
		strcpy(pCancelOrder->UserID,USER_ID);
		///Ͷ���ߴ���
		strcpy(pCancelOrder->InvestorID, INVESTOR_ID);
		///���ر������
		strcpy(pCancelOrder->UserOrderLocalID, UserOrderLocalID); //�����Ķ������ر������
		///����������־
		pCancelOrder->ActionFlag = '0'; // 0:ɾ��
		///�������ر��
		// increase LocalID
		sprintf(pCancelOrder->UserOrderActionLocalID, "%012d", orderRef++); //ͬһ������

		int iResult = pTraderApi2->ReqOrderAction(pCancelOrder, ++iRequestID);
		printf("----->>> ������������: %s\n", ((iResult == 0) ? "�ɹ�" : "ʧ��"));
		delete pCancelOrder;
		return (iResult == 0);
}

// Initialize femas basicOrder
void OrderManageSys::InitBasicOrder2(void)
{
	memset(&basicOrder2, 0, sizeof(basicOrder2));
	///���͹�˾����
	strcpy(basicOrder2.BrokerID, BROKER_ID);
	//�û�����
	strcpy(basicOrder2.UserID,USER_ID);
	//����������
	strcpy(basicOrder2.ExchangeID,EXCHANGEID);
	///Ͷ���ߴ���
	strcpy(basicOrder2.InvestorID, INVESTOR_ID);
	///���Ͷ���ױ���־
	basicOrder2.HedgeFlag = '1'; // Speculation
	///��С�ɽ���: 1
	basicOrder2.MinVolume = 1;
	///��������: ����
	basicOrder2.TimeCondition = '1'; // Immediately;
	///ǿƽԭ��: ��ǿƽ
	basicOrder2.ForceCloseReason = '0';
	///�Զ������־: ��
	basicOrder2.IsAutoSuspend = 0;
}

//��ʼ���ֲ�
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
			pShortPosition->aveCost = (price/pShortPosition->volume);//����300��
		}
		LeaveCriticalSection(&critSec_ShortPos);
	}
}
// ���³ֲ�����
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