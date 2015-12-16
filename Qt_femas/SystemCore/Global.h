#pragma once

#include "PublicFuncs.h"

// 常数列表

#define CONST_DIRECTION_Long		'0'
#define CONST_DIRECTION_Short		'1'

#define CONST_ORDER_TYPE_AnyPrice	'0'
#define CONST_ORDER_TYPE_LimitPrice '1'
#define CONST_ORDER_TYPE_FOK		'2'
#define CONST_ORDER_TYPE_FAK		'3'

#define CONST_FUTURE_Entry			'0'
#define CONST_FUTURE_Exit			'1'
#define CONST_FUTURE_ExitToday		'3'
#define CONST_FUTURE_ExitYesTD		'4'

// 全局变量

//femasAPI对象
extern class CUstpFtdcMduserApi* pUserApi;
extern class femasMdSpi* pUserSpi;
extern class CUstpFtdcTraderApi* pTraderApi2;
extern class femasTraderSpi* pTraderSpi2;

// 策略的全局变量，目前只使用一个策略。若使用多个，需定义多个
extern class Strategy* pStrategyGlobal;

// 连接API的参数，交易和行情分开
extern char FRONT_ADDR[32];							// 交易前置地址
extern TUstpFtdcBrokerIDType	BROKER_ID;			// 经纪公司代码
extern TUstpFtdcInvestorIDType INVESTOR_ID;		// 投资者代码
extern TUstpFtdcUserIDType USER_ID;                  // 用户代码
extern TUstpFtdcPasswordType	PASSWORD;			// 用户密码
extern TUstpFtdcExchangeIDType EXCHANGEID;         //交易所ID
extern char FRONT_ADDR2[32];						// 行情前置地址
extern CUstpFtdcRspInvestorAccountField *pInvestorAccount;

extern char *ppInstrumentID[];						// ppInstrumentID[] is in Global.cpp
// 此处可设置
const int instrumentNum = 20;						// 合约数量
const int marketDataListMaxLength = 600;					// 存储的MarketData的数量，300个为5分钟
const bool isOutputMdInfo = false;					// 是否输出运行时与MarketData相关的数据，主要用于测试


// request ID
extern int iRequestID;

//控制两个button只能点一次
extern bool TradebtnOK;

// 高精度时钟
extern LARGE_INTEGER performanceFrequency;
extern LARGE_INTEGER performanceCounter;
extern LONGLONG frequency;

// strategy parameters
// 预定义的windowLength最大值，实际策略中的window_length由SetParameter决定
#define windowLengthMax 200
extern bool isTest;
extern bool isFemas;
extern double priceTick;
extern vector<string> instrumentVec;

typedef struct myOrder
{
	//baoliu
	int userLocalID;
	int ordersysID;
	// order reference
	int orderRef;
	// instrument ID, in a canceling order it stores the order reference of target order
	TUstpFtdcInstrumentIDType instrumentID;
	// direction: '0' - long, '1' - short
	char direction;
	// operation: '0' - open, '1' - close, '2' - close today, '3' - close yesterday, '4' - cancel
	char operation;
	// quantity/volume
	int quantity;
	// traded quantity
	int tradedQuan;
	// theoretical price
	double theoPrice;
	// order price
	double orderPrice;
	// trade price
	double tradePrice;
	// order status: '0' - ready, '1' - sent, '2' - received, '3' - pending, '4' - traded
	char status;
	char* tradeID;
	char* insertTime;
	char* successTime;
	// entrust time
	LONGLONG entrustTime;
	// response time
	LONGLONG responseTime;
	// trade time
	LONGLONG tradeTime;
	// priceType
	TUstpFtdcPriceType orderPriceType;
	// Strategy
	class Strategy* pStrategy;
	
	// 默认构造函数 
	myOrder()
	{
		this->userLocalID = NULL;
		this->ordersysID = NULL;
		this->orderRef = NULL;
		strcpy(this->instrumentID, "");
		this->direction = NULL;
		this->operation = NULL;
		this->quantity = NULL;
		this->tradedQuan = NULL;
		this->theoPrice = NULL;
		this->orderPrice = NULL;
		this->tradePrice = NULL;
		this->status = NULL;
		this->tradeID = NULL;
		this->insertTime = NULL;
		this->successTime = NULL;
		this->entrustTime = NULL;
		this->responseTime = NULL;
		this->tradeTime = NULL;
		this->orderPriceType = NULL;
		this->pStrategy = NULL;
	}
	// 构造函数
	myOrder(int userLocalID,
	int ordersysID,
		int orderRef,
		TUstpFtdcInstrumentIDType instrumentID,
		char direction,
		char operation,
		int quantity,
		int tradedQuan,
		double theoPrice,
		double orderPrice,
		double tradePrice,
		char status,
		char* tradeID,
		char* insertTime,
		char* successTime,
		LONGLONG entrustTime,
		LONGLONG responseTime,
		LONGLONG tradeTime,
		TUstpFtdcPriceType orderPriceType,
		Strategy* pStrategy)
	{
		this->userLocalID = userLocalID;
		this->ordersysID = ordersysID;
		this->orderRef = orderRef;
		strcpy(this->instrumentID, instrumentID);
		this->direction = direction;
		this->operation = operation;
		this->quantity = quantity;
		this->tradedQuan = tradedQuan;
		this->theoPrice = theoPrice;
		this->orderPrice = orderPrice;
		this->tradePrice = tradePrice;
		this->status = status;
		this->tradeID = tradeID;
		this->insertTime = insertTime;
		this->successTime = successTime,
		this->entrustTime = entrustTime;
		this->responseTime = responseTime;
		this->tradeTime = tradeTime;
		this->pStrategy = pStrategy;
	}

} CMyOrder, *pCMyOrder;

typedef struct mySignal
{
	char orderPriceType;
	char direction;
	char operation;
	TUstpFtdcInstrumentIDType instrumentID;
	int volume;
	double price;

	// 默认构造函数 
	mySignal()
	{
		this->orderPriceType = NULL;
		this->direction = NULL;
		this->operation = NULL;
		strcpy(this->instrumentID, "");
		this->volume = NULL;
		this->price = NULL;
	}
	// 构造函数 
	mySignal(char orderPriceType, char direction, char operation, TUstpFtdcInstrumentIDType instrumentID, int volume, double price)
	{
		this->orderPriceType = orderPriceType;
		this->direction = direction;
		this->operation = operation;
		strcpy(this->instrumentID, instrumentID);
		this->volume = volume;
		this->price = price;
	}
} CMySignal, *pCMySignal;

typedef struct myFeed
{
	long feedTime;
	int feedTimeMillisec;
	int instrumentIndex;
	int cursor;
	// 没写构造函数
} CMyFeed, *pCMyFeed;

typedef struct myPosition
{
	int volume; // 持仓量
	int availableVolume; // 可平手数
	double aveCost; // 平均成本
} CMyPosition, *pCMyPosition;

typedef struct myAccount
{
	double totalCaptital; // 总资金
	double cash; // 可用现金
	double margin; // 保证金
	double charge; // 手续费
} CMyAccount, *pCMyAccount;
