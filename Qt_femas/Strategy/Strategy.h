#pragma once

#include "PublicFuncs.h"

class Strategy
{
public:
	Strategy(void);
	~Strategy(void);

	// �趨priceTick���µ��ж���ӵļ۸�tick��
	void SetPriceTick(double _price_tick, int _add_units_immediate_deal, int _add_units_entry, int _add_units_exit, 
		int _add_units_stopLoss, int _add_units_forceStop);
	// �趨������صĲ���
	void SetStrategyParam(int _window_length, int UnitsCoefOpenPosi, int UnitsCoefStopLoss, int UnitsCoefStopLossProt, 
		int UnitsCoefForceStop, int Units_diff_thre1, int Units_diff_thre2, int _stopWaitTime, int _firstOrderWaitTime, int _virtualFeedWaitTime);
	// �趨�����������isTest��_instrumentID�Զ�ѡ����Բ���������runThread֮ǰ����SetParamters
	void SetParamters(bool isTest, char* _instrumentID, double _price_tick);
	// �����̵߳���ʼ����������Ϊһ�����������߳̽��Ӹú�����ʼ����
	// ��ͬ�Ĳ��Զ�����ԴӲ�ͬ�ĺ�����ʼ���У�Ҳ���Դ���ͬ�ĺ�����ʼ�������໥Ӱ��
	void runThread(LPTHREAD_START_ROUTINE pfnThreadProc);
	// -----------------------------------------------------------------------------------
	// �����¼�Feed���Ӻ��������಻�����أ�����ֻ������UpdateMD4St��RunStrategy
	void DealWithFeed(void);
	// ���²����е�Feed���ݣ����ж��Ƿ����ִ�в��Ե������������������
	virtual bool UpdateMD4St(void) = 0;
	// �����㷨�����庯�����ж��µ����������ɶ����źţ������������
	virtual void RunStrategy(void) = 0;
	// -----------------------------------------------------------------------------------
	// �����¼�OnRtnOrder���Ӻ��������಻������
	void DealWithRtnOrder(void);
	// �������صĶ�RtnOrder�Ķ��⴦�������������
	virtual void StrategyOnRtnOrder(pCMyOrder pOrder, CUstpFtdcOrderField* pRtnOrder) = 0;
	// -----------------------------------------------------------------------------------
	// �����¼�OnRtnTrade���Ӻ��������಻������
	void DealWithRtnTrade(void);
	// �������صĶ�RtnTrade�Ķ��⴦�������������
	virtual void StrategyOnRtnTrade(pCMyOrder pOrder, CUstpFtdcTradeField* pRtnTrade) = 0;
	// -----------------------------------------------------------------------------------
	
protected:
	// �������м۵Ķ������������඼�����õ�����˷����˻��ࣻ���໹���Զ������Ƶĸ��Ի����µ�����
	//void EntrustTwoAnyPirce(char direction1, char direction2, char operation);

public:
	// �Ƿ�ִ�в���
	bool isRunStrategy;
	// ��λ��¼
	int posInd;
	// Feed���е��ٽ�����֪ͨ�¼���FeedQue���ڴ��LocalMemory���͸�Strategy��Feed
	CRITICAL_SECTION critSec_FeedQue;
	queue<CMyFeed> FeedQue;
	HANDLE hEventFeed;
	// RtnOrder���е��ٽ�����֪ͨ�¼���RtnOrderQue���ڴ��OrderManageSys���͸�Strategy��RtnOrder
	CRITICAL_SECTION critSec_RtnOrderQue;
	queue<CUstpFtdcOrderField> RtnOrderQue2;
	HANDLE hEventOnRtnOrder;
	// RtnTrade���е��ٽ�����֪ͨ�¼���RtnTradeQue���ڴ��OrderManageSys���͸�Strategy��RtnTrade
	CRITICAL_SECTION critSec_RtnTradeQue;
	queue<CUstpFtdcTradeField> RtnTradeQue2;
	HANDLE hEventOnRtnTrade;
	// �洢�����¼������飬����WaitForMultipleObjects����
	HANDLE hObject[3];
	// ��һ���������ͺ�ĵȴ�ʱ��
	static int firstOrderWaitTime;
	// ��һ��Feed�ﵽ��ĵȴ�ʱ��
	static int virtualFeedWaitTime;
	// �Ƿ�ڶ���Feed����
	bool is2ndFeedArrived;
	// ����Feed
	CMyFeed virtualFeed;

protected:
	// ���ص�orderTable
	vector<struct myOrder> localOrderTable;
	// �����̵߳�handle
	HANDLE hThread_St;
	// ��¼feed��ʱ���
	long timeStamp;
	CMyFeed newFeed, feed1, feed2;
	// ������ز���
	int window_length;
	int diffCursor;
	double diffAsk[windowLengthMax];
	double diffBid[windowLengthMax];
	double diffAskCur, diffBidCur;
	double lastBid1, lastAsk1, lastBid2, lastAsk2;
	double sumAsk, sumBid, aveAsk, aveBid, midLine;
	double coefOpenPosi, coefStopLoss, coefStopLossProt, coefForceStop, diff_thre1, diff_thre2;
	int openInd, closeInd;
	double openLongPrice, openShortPrice;
	double price_tick;
	int add_units_immediate_deal/* anyPrice */, add_units_entry, add_units_exit, add_units_stopLoss, add_units_forceStop;
	int stopWaitTime;
};

// �����̵߳���ʼ������Ҳ����Ϊ�������Զ�����������ʼ������ֻҪ�����������ݸ�runThread����
DWORD WINAPI ThreadFunc_St(LPVOID pParam);
// �̺߳������ȴ�firstOrderWaitTime���룬����һ������δ�ɽ��򳷵�
DWORD WINAPI ThreadFunc_WaitAndCancel(LPVOID pParam);
// �̺߳������ȴ�virtualFeedWaitTime���룬���ڶ���feed��û��������ǰһ��tick��feed��Ϊ����feed���Դ�������
DWORD WINAPI ThreadFunc_WaitAndSendVirtualFeed(LPVOID pParam);

// ���
double sum(double num[], int n);
// ƽ��ֵ
double average(double num[], int n);
// ��׼��
double stdev(double num[], int n);
// ��ʽ���۸񣬱���һλС��
double formatPrice(double price);

//DWORD WINAPI ThreadFunc_MarketDataGenerator(LPVOID pParam);