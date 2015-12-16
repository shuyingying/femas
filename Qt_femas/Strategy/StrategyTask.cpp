#include "PublicFuncs.h"


StrategyTask::StrategyTask(void):Strategy()
{
}


StrategyTask::~StrategyTask(void)
{
}


// 更新策略中的Feed数据，并判断是否符合执行策略的条件
bool StrategyTask::UpdateMD4St(void)
{
	// 取第一个合约的涨停价
	if(newFeed.instrumentIndex ==0)
	{
		upperLimitPrice = (LocalMemory::femasMarketData[0]->getCurMarketData()->UpperLimitPrice);
		lowerLimitPrice = (LocalMemory::femasMarketData[0]->getCurMarketData()->LowerLimitPrice);
		return true;
	}
	return false;
}

// 策略算法的主体函数，判断下单条件并生成订单信号
void StrategyTask::RunStrategy(void)
{
	FinishTask();
	OrderManageSys::m_bIsRunStrategy = false;
}

// 与策略相关的对RtnOrder的额外处理；记录撤单成功时间，更新limitPosInd
void StrategyTask::StrategyOnRtnOrder(pCMyOrder pOrder, CUstpFtdcOrderField* pRtnOrder)
{

}

// 与策略相关的对RtnTrade的额外处理；第一个订单成交时发送第二个订单，第二个订单成交时更新posInd
void StrategyTask::StrategyOnRtnTrade(pCMyOrder pOrder, CUstpFtdcTradeField* pRtnTrade)
{

}


// 完成任务的函数
void StrategyTask::FinishTask(void)
{
	// 任务1：单交易日完成100笔委托发送，共3个交易日：20分
	// 任务5：单交易日发送20笔FAK委托，20笔FOK委托，共3个交易日：20分
	// 任务6：单交易日完成一次批量报单（10s内报入50笔订单），共3个交易日：30分
	// 这里迅速的以不能成交的价格下50笔FOK订单和50笔FAK订单
	// 以涨停价开空100笔，每笔1手
	// FOK 50笔 10秒
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

			// 加入orderTable
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
	// FAK 50笔 10秒内
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

			// 加入orderTable
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
	
	// 任务2：单交易日完成50笔成交(不包含自成交)，共3个交易日： 30分
	// 任务4：单交易日发送20笔平仓单，共3个交易日: 20分
	// 下50笔市价单，每下一笔平一笔
	// AnyPrice
	//开关
	bool isRunTest2 = true;
	if(isRunTest2)
	{
	for (int i=0; i<50; i++)
	{
		// 以涨停价开多
		char long_short = '0';
		char entry_exit = '0';
		char* instrumentID = ppInstrumentID[0];
		int volume = 1;
		double price = upperLimitPrice;
		OrderManageSys::MyOrderInsert(CONST_ORDER_TYPE_AnyPrice, long_short, entry_exit, instrumentID, volume, price);

		// 加入orderTable
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
		// 以跌停价平多
		long_short = '1';
		entry_exit = '1';
		instrumentID = ppInstrumentID[0];
		volume = 1;
		price = lowerLimitPrice;
		OrderManageSys::MyOrderInsert(CONST_ORDER_TYPE_AnyPrice, long_short, entry_exit, instrumentID, volume, price);

		// 加入orderTable
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
	
	// 任务3：单交易日完成20笔手动撤单，共3个交易日: 20分
	// 下50笔无法成交的限价单，再手动撤单
	// 以涨停价开空50笔，每笔1手
	// LimitPrice

	//开关
	bool isRunTest3 = false;
	if(isRunTest3)
	{
	for (int i=0; i<50; i++)
	{
		// 以涨停价开空，限价
		char long_short = '1';
		char entry_exit = '0';
		char* instrumentID = ppInstrumentID[0];
		int volume = 1;
		double price = upperLimitPrice;
		OrderManageSys::MyOrderInsert(CONST_ORDER_TYPE_LimitPrice, long_short, entry_exit, instrumentID, volume, price);
		// 加入orderTable
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

		//等待3秒
		Sleep(3000);

		// 撤单
		TUstpFtdcUserOrderLocalIDType userOrderLocalID;
		sprintf(userOrderLocalID, "%012d", OrderManageSys::orderRef - 1);
		OrderManageSys::MyOrderCancel(userOrderLocalID);
		
		int orderRef = atoi(userOrderLocalID);
		// 在orderTable中寻找userOrderLocalID相同的记录，倒着找比较快
		for (i = OrderManageSys::orderTable.size() - 1; i>= 0; i--)
		{
			pCMyOrder pOrder = &OrderManageSys::orderTable[i];
			
			if (orderRef == pOrder->orderRef)
			{
					// 撤单成功后在回报里会有状态更新
				pOrder->status = USTP_FTDC_OS_AcceptedNoReply;
				break;
			}
		}
		Sleep(1000);
	}
}
}