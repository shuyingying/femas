#pragma once

#include "PublicFuncs.h"

class Strategy
{
public:
	Strategy(void);
	~Strategy(void);

	// 设定priceTick和下单中额外加的价格tick数
	void SetPriceTick(double _price_tick, int _add_units_immediate_deal, int _add_units_entry, int _add_units_exit, 
		int _add_units_stopLoss, int _add_units_forceStop);
	// 设定策略相关的参数
	void SetStrategyParam(int _window_length, int UnitsCoefOpenPosi, int UnitsCoefStopLoss, int UnitsCoefStopLossProt, 
		int UnitsCoefForceStop, int Units_diff_thre1, int Units_diff_thre2, int _stopWaitTime, int _firstOrderWaitTime, int _virtualFeedWaitTime);
	// 设定参数，会根据isTest和_instrumentID自动选择策略参数；运行runThread之前需先SetParamters
	void SetParamters(bool isTest, char* _instrumentID, double _price_tick);
	// 策略线程的起始函数，输入为一个函数名，线程将从该函数开始运行
	// 不同的策略对象可以从不同的函数开始运行，也可以从相同的函数开始，不会相互影响
	void runThread(LPTHREAD_START_ROUTINE pfnThreadProc);
	// -----------------------------------------------------------------------------------
	// 处理事件Feed的子函数，子类不必重载；子类只需重载UpdateMD4St和RunStrategy
	void DealWithFeed(void);
	// 更新策略中的Feed数据，并判断是否符合执行策略的条件；子类必须重载
	virtual bool UpdateMD4St(void) = 0;
	// 策略算法的主体函数，判断下单条件并生成订单信号；子类必须重载
	virtual void RunStrategy(void) = 0;
	// -----------------------------------------------------------------------------------
	// 处理事件OnRtnOrder的子函数，子类不必重载
	void DealWithRtnOrder(void);
	// 与策略相关的对RtnOrder的额外处理；子类必须重载
	virtual void StrategyOnRtnOrder(pCMyOrder pOrder, CUstpFtdcOrderField* pRtnOrder) = 0;
	// -----------------------------------------------------------------------------------
	// 处理事件OnRtnTrade的子函数，子类不必重载
	void DealWithRtnTrade(void);
	// 与策略相关的对RtnTrade的额外处理；子类必须重载
	virtual void StrategyOnRtnTrade(pCMyOrder pOrder, CUstpFtdcTradeField* pRtnTrade) = 0;
	// -----------------------------------------------------------------------------------
	
protected:
	// 下两个市价的订单，由于子类都可能用到，因此放在了基类；子类还可以定义相似的个性化的下单函数
	//void EntrustTwoAnyPirce(char direction1, char direction2, char operation);

public:
	// 是否执行策略
	bool isRunStrategy;
	// 仓位记录
	int posInd;
	// Feed队列的临界区，通知事件，FeedQue用于存放LocalMemory发送给Strategy的Feed
	CRITICAL_SECTION critSec_FeedQue;
	queue<CMyFeed> FeedQue;
	HANDLE hEventFeed;
	// RtnOrder队列的临界区，通知事件，RtnOrderQue用于存放OrderManageSys发送给Strategy的RtnOrder
	CRITICAL_SECTION critSec_RtnOrderQue;
	queue<CUstpFtdcOrderField> RtnOrderQue2;
	HANDLE hEventOnRtnOrder;
	// RtnTrade队列的临界区，通知事件，RtnTradeQue用于存放OrderManageSys发送给Strategy的RtnTrade
	CRITICAL_SECTION critSec_RtnTradeQue;
	queue<CUstpFtdcTradeField> RtnTradeQue2;
	HANDLE hEventOnRtnTrade;
	// 存储所有事件的数组，用于WaitForMultipleObjects调用
	HANDLE hObject[3];
	// 第一个订单发送后的等待时间
	static int firstOrderWaitTime;
	// 第一个Feed达到后的等待时间
	static int virtualFeedWaitTime;
	// 是否第二个Feed来过
	bool is2ndFeedArrived;
	// 虚拟Feed
	CMyFeed virtualFeed;

protected:
	// 本地的orderTable
	vector<struct myOrder> localOrderTable;
	// 策略线程的handle
	HANDLE hThread_St;
	// 记录feed的时间戳
	long timeStamp;
	CMyFeed newFeed, feed1, feed2;
	// 策略相关参数
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

// 策略线程的起始函数，也可以为其它策略定义其它的起始函数，只要将函数名传递给runThread即可
DWORD WINAPI ThreadFunc_St(LPVOID pParam);
// 线程函数，等待firstOrderWaitTime毫秒，若第一个订单未成交则撤单
DWORD WINAPI ThreadFunc_WaitAndCancel(LPVOID pParam);
// 线程函数，等待virtualFeedWaitTime毫秒，若第二个feed还没到，则发送前一个tick的feed作为虚拟feed，以触发策略
DWORD WINAPI ThreadFunc_WaitAndSendVirtualFeed(LPVOID pParam);

// 求和
double sum(double num[], int n);
// 平均值
double average(double num[], int n);
// 标准差
double stdev(double num[], int n);
// 格式化价格，保留一位小数
double formatPrice(double price);

//DWORD WINAPI ThreadFunc_MarketDataGenerator(LPVOID pParam);