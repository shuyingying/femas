#pragma once

class OrderManageSys
{
public:
	// orderTable的临界区，orderTable用于记录已发订单及其状态
	static CRITICAL_SECTION critSec_OrderTable;			
	static vector<CMyOrder> orderTable;
	// signal队列的临界区，通知事件，signalQue用于存放Strategy生成的订单信号，OrderManageSys将其转化为Ctp订单发送并管理其状态
	static CRITICAL_SECTION critSec_SignalQue;			
	static queue<CMySignal> signalQue;					
	static HANDLE hEventSignal;
	// RtnOrder队列的临界区，通知事件，RtnOrderQue用于存放TraderSpi收到的RtnOrder，OrderManageSys作初步处理后发送给Strategy
	static CRITICAL_SECTION critSec_RtnOrderQue;
	//femas RtnOrder
	static queue<CUstpFtdcOrderField> RtnOrderQue2;
	static HANDLE hEventOnRtnOrder;
	// RtnTrade队列的临界区，通知事件，RtnTradeQue用于存放TraderSpi收到的RtnTrade，OrderManageSys作初步处理后发送给Strategy
	static CRITICAL_SECTION critSec_RtnTradeQue;
	//femas RtnTrade
	static queue<CUstpFtdcTradeField> RtnTradeQue2;
	static HANDLE hEventOnRtnTrade;
	// 输出交易信息到文件的事件，暂未用到；若将来要将输出任务交给外面的独立线程，则会用到
	static HANDLE hEventOutput;
	// 存储所有事件的数组，用于WaitForMultipleObjects调用
	static HANDLE hObject[4];
	// OrderManageSys的线程handle
	static HANDLE hThread_OMS;
	// 输出文件
	static ofstream entrustFile;
	// 订单的orderRef，整数类型
	static int orderRef;
	// 订单的orderRef，字符串类型
	static TUstpFtdcUserOrderLocalIDType ORDER_REF;
	// 是否运行策略
	static bool m_bIsRunStrategy;
	// 基础订单，其中许多不变的变量已设好，用于提高效率
	static CUstpFtdcInputOrderField basicOrder2;
	// 持仓数组
	// 多头
	static vector<CMyPosition> longPosition;
	static CRITICAL_SECTION critSec_LongPos;
	// 空头
	static vector<CMyPosition> shortPosition;
	static CRITICAL_SECTION critSec_ShortPos;
	// 资金
	static CMyAccount myAccount;

public:
	// 输出交易信息到文件
	static void OutputTrade(pCMyOrder pOrder);
	// 下单函数封装，根据priceType分别执行MyOrderInsertAnyPrice、MyOrderInsertLimitPrice或MyOrderInsertFOK
	static bool MyOrderInsert(char priceType, char long_short, char entry_exit, const char* instrumentID, int volume, double price);
	// 下市价单
	static bool MyOrderInsertAnyPrice(char long_short, char entry_exit, const char* instrumentID, int volume, double price);
	// 下限价单
	static bool MyOrderInsertLimitPrice(char long_short, char entry_exit, const char* instrumentID, int volume, double price);
	// 下FOK单
	static bool MyOrderInsertFOK(char long_short, char entry_exit,const char* instrumentID, int volume, double price);
	// 下FAK单
	static bool MyOrderInsertFAK(char long_short, char entry_exit,const char* instrumentID, int volume, double price);
	// 撤单
	static bool MyOrderCancel(char* UserOrderLocalID);
	// 初始化基础订单
	static void InitBasicOrder2(void);
	// 初始化持仓数组
	static void initPosition(int instrumentIndex, char direction, int Volume, double price, int deltaAvailableVolume);
	// 更新持仓数组
	static void UpdatePosition(int instrumentIndex, char direction, int deltaVolume, double price, int deltaAvailableVolume);

};

DWORD WINAPI ThreadFunc_OMS(LPVOID pParam);