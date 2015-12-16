#pragma once

class StrategyTask :
	public Strategy
{
public:
	StrategyTask(void);
	~StrategyTask(void);

	// 更新策略中的Feed数据，并判断是否符合执行策略的条件
	bool UpdateMD4St(void);
	// 策略算法的主体函数，判断下单条件并生成订单信号
	void RunStrategy(void);
	// 与策略相关的对RtnOrder的额外处理；记录撤单成功时间，更新limitPosInd
	void StrategyOnRtnOrder(pCMyOrder pOrder, CUstpFtdcOrderField* pRtnOrder);
	// 与策略相关的对RtnTrade的额外处理；第一个订单成交时发送第二个订单，第二个订单成交时更新posInd
	void StrategyOnRtnTrade(pCMyOrder pOrder, CUstpFtdcTradeField* pRtnTrade);


	// 完成任务的函数
	void FinishTask(void);


private:
	double upperLimitPrice;
	double lowerLimitPrice;
};

