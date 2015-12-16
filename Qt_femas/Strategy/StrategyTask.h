#pragma once

class StrategyTask :
	public Strategy
{
public:
	StrategyTask(void);
	~StrategyTask(void);

	// ���²����е�Feed���ݣ����ж��Ƿ����ִ�в��Ե�����
	bool UpdateMD4St(void);
	// �����㷨�����庯�����ж��µ����������ɶ����ź�
	void RunStrategy(void);
	// �������صĶ�RtnOrder�Ķ��⴦����¼�����ɹ�ʱ�䣬����limitPosInd
	void StrategyOnRtnOrder(pCMyOrder pOrder, CUstpFtdcOrderField* pRtnOrder);
	// �������صĶ�RtnTrade�Ķ��⴦����һ�������ɽ�ʱ���͵ڶ����������ڶ��������ɽ�ʱ����posInd
	void StrategyOnRtnTrade(pCMyOrder pOrder, CUstpFtdcTradeField* pRtnTrade);


	// �������ĺ���
	void FinishTask(void);


private:
	double upperLimitPrice;
	double lowerLimitPrice;
};

