#include "PublicFuncs.h"


StrategyTask::StrategyTask(void):Strategy()
{
}


StrategyTask::~StrategyTask(void)
{
}


// ���²����е�Feed���ݣ����ж��Ƿ����ִ�в��Ե�����
bool StrategyTask::UpdateMD4St(void)
{
	// ȡ��һ����Լ����ͣ��
	if(newFeed.instrumentIndex ==0)
	{
		upperLimitPrice = (LocalMemory::femasMarketData[0]->getCurMarketData()->UpperLimitPrice);
		lowerLimitPrice = (LocalMemory::femasMarketData[0]->getCurMarketData()->LowerLimitPrice);
		return true;
	}
	return false;
}

// �����㷨�����庯�����ж��µ����������ɶ����ź�
void StrategyTask::RunStrategy(void)
{
	FinishTask();
	OrderManageSys::m_bIsRunStrategy = false;
}

// �������صĶ�RtnOrder�Ķ��⴦����¼�����ɹ�ʱ�䣬����limitPosInd
void StrategyTask::StrategyOnRtnOrder(pCMyOrder pOrder, CUstpFtdcOrderField* pRtnOrder)
{

}

// �������صĶ�RtnTrade�Ķ��⴦����һ�������ɽ�ʱ���͵ڶ����������ڶ��������ɽ�ʱ����posInd
void StrategyTask::StrategyOnRtnTrade(pCMyOrder pOrder, CUstpFtdcTradeField* pRtnTrade)
{

}


// �������ĺ���
void StrategyTask::FinishTask(void)
{
	// ����1�������������100��ί�з��ͣ���3�������գ�20��
	// ����5���������շ���20��FAKί�У�20��FOKί�У���3�������գ�20��
	// ����6�������������һ������������10s�ڱ���50�ʶ���������3�������գ�30��
	// ����Ѹ�ٵ��Բ��ܳɽ��ļ۸���50��FOK������50��FAK����
	// ����ͣ�ۿ���100�ʣ�ÿ��1��
	// FOK 50�� 10��
	bool isRunTest1 = false;
	if(isRunTest1)
	{
	for (int i=0; i<10; i++)
	{
		for (int j=0; j<5; j++)
		{
			char long_short = '1';
			char entry_exit = '0';
			char* instrumentID = ppInstrumentID[0];
			int volume = 1;
			double price = upperLimitPrice;
			OrderManageSys::MyOrderInsert(CONST_ORDER_TYPE_FOK, long_short, entry_exit, instrumentID, volume, price);

			// ����orderTable
			// record time
			QueryPerformanceCounter(&performanceCounter);
			CMyOrder order(NULL,
				NULL,
				OrderManageSys::orderRef-1,
				instrumentID,
				long_short,
				entry_exit,
				volume,
				0,
				upperLimitPrice,
				upperLimitPrice,
				0,
				USTP_FTDC_OS_AcceptedNoReply,
				0,
				0,
				0,
				performanceCounter.QuadPart,
				0,
				0,
				CONST_ORDER_TYPE_FOK,
				this);
			// add to order table
			// even if order1 is not placed (it doesn't matter)
			OrderManageSys::orderTable.push_back(order);
		}
		Sleep(900);
	}
	// FAK 50�� 10����
	for (int i=0; i<10; i++)
	{
		for (int j=0; j<5; j++)
		{
			char long_short = '1';
			char entry_exit = '0';
			char* instrumentID = ppInstrumentID[0];
			int volume = 1;
			double price = upperLimitPrice;
			OrderManageSys::MyOrderInsert(CONST_ORDER_TYPE_FAK, long_short, entry_exit, instrumentID, volume, price);

			// ����orderTable
			// record time
			QueryPerformanceCounter(&performanceCounter);
			CMyOrder order(NULL,
				NULL,
				OrderManageSys::orderRef-1,
				instrumentID,
				long_short,
				entry_exit,
				volume,
				0,
				upperLimitPrice,
				upperLimitPrice,
				0,
				USTP_FTDC_OS_AcceptedNoReply,
				0,
				0,
				0,
				performanceCounter.QuadPart,
				0,
				0,
				CONST_ORDER_TYPE_FAK,
				this);
			// add to order table
			// even if order1 is not placed (it doesn't matter)
			OrderManageSys::orderTable.push_back(order);
		}
		Sleep(800); 
	}
	}
	
	// ����2�������������50�ʳɽ�(�������Գɽ�)����3�������գ� 30��
	// ����4���������շ���20��ƽ�ֵ�����3��������: 20��
	// ��50���м۵���ÿ��һ��ƽһ��
	// AnyPrice
	//����
	bool isRunTest2 = true;
	if(isRunTest2)
	{
	for (int i=0; i<50; i++)
	{
		// ����ͣ�ۿ���
		char long_short = '0';
		char entry_exit = '0';
		char* instrumentID = ppInstrumentID[0];
		int volume = 1;
		double price = upperLimitPrice;
		OrderManageSys::MyOrderInsert(CONST_ORDER_TYPE_AnyPrice, long_short, entry_exit, instrumentID, volume, price);

		// ����orderTable
		// record time
		QueryPerformanceCounter(&performanceCounter);
		CMyOrder order(NULL,
			NULL,
			OrderManageSys::orderRef-1,
			instrumentID,
			long_short,
			entry_exit,
			volume,
			0,
			upperLimitPrice,
			upperLimitPrice,
			0,
			USTP_FTDC_OS_AcceptedNoReply,
			0,
			0,
			0,
			performanceCounter.QuadPart,
			0,
			0,
			CONST_ORDER_TYPE_AnyPrice,
			this);
		// add to order table
		// even if order1 is not placed (it doesn't matter)
		OrderManageSys::orderTable.push_back(order);

		Sleep(5000);
		// �Ե�ͣ��ƽ��
		long_short = '1';
		entry_exit = '1';
		instrumentID = ppInstrumentID[0];
		volume = 1;
		price = lowerLimitPrice;
		OrderManageSys::MyOrderInsert(CONST_ORDER_TYPE_AnyPrice, long_short, entry_exit, instrumentID, volume, price);

		// ����orderTable
		// record time
		QueryPerformanceCounter(&performanceCounter);
		CMyOrder order2(NULL,
			NULL,
			OrderManageSys::orderRef-1,
			instrumentID,
			long_short,
			entry_exit,
			volume,
			0,
			lowerLimitPrice,
			lowerLimitPrice,
			0,
			USTP_FTDC_OS_AcceptedNoReply,
			0,
			0,
			0,
			performanceCounter.QuadPart,
			0,
			0,
			CONST_ORDER_TYPE_AnyPrice,
			this);
		// add to order table
		// even if order1 is not placed (it doesn't matter)
		OrderManageSys::orderTable.push_back(order2);

		Sleep(1000);
	}
	}
	
	// ����3�������������20���ֶ���������3��������: 20��
	// ��50���޷��ɽ����޼۵������ֶ�����
	// ����ͣ�ۿ���50�ʣ�ÿ��1��
	// LimitPrice

	//����
	bool isRunTest3 = false;
	if(isRunTest3)
	{
	for (int i=0; i<50; i++)
	{
		// ����ͣ�ۿ��գ��޼�
		char long_short = '1';
		char entry_exit = '0';
		char* instrumentID = ppInstrumentID[0];
		int volume = 1;
		double price = upperLimitPrice;
		OrderManageSys::MyOrderInsert(CONST_ORDER_TYPE_LimitPrice, long_short, entry_exit, instrumentID, volume, price);
		// ����orderTable
			// record time
			QueryPerformanceCounter(&performanceCounter);
			CMyOrder order(NULL,
				NULL,
				OrderManageSys::orderRef-1,
				instrumentID,
				long_short,
				entry_exit,
				volume,
				0,
				upperLimitPrice,
				upperLimitPrice,
				0,
				USTP_FTDC_OS_AcceptedNoReply,
				0,
				0,
				0,
				performanceCounter.QuadPart,
				0,
				0,
				CONST_ORDER_TYPE_LimitPrice,
				this);
			// add to order table
			// even if order1 is not placed (it doesn't matter)
			OrderManageSys::orderTable.push_back(order);

		//�ȴ�3��
		Sleep(3000);

		// ����
		TUstpFtdcUserOrderLocalIDType userOrderLocalID;
		sprintf(userOrderLocalID, "%012d", OrderManageSys::orderRef - 1);
		OrderManageSys::MyOrderCancel(userOrderLocalID);
		
		int orderRef = atoi(userOrderLocalID);
		// ��orderTable��Ѱ��userOrderLocalID��ͬ�ļ�¼�������ұȽϿ�
		for (i = OrderManageSys::orderTable.size() - 1; i>= 0; i--)
		{
			pCMyOrder pOrder = &OrderManageSys::orderTable[i];
			
			if (orderRef == pOrder->orderRef)
			{
					// �����ɹ����ڻر������״̬����
				pOrder->status = USTP_FTDC_OS_AcceptedNoReply;
				break;
			}
		}
		Sleep(1000);
	}
}
}