#pragma once

#include "PublicFuncs.h"

// �����б�

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

// ȫ�ֱ���

//femasAPI����
extern class CUstpFtdcMduserApi* pUserApi;
extern class femasMdSpi* pUserSpi;
extern class CUstpFtdcTraderApi* pTraderApi2;
extern class femasTraderSpi* pTraderSpi2;

// ���Ե�ȫ�ֱ�����Ŀǰֻʹ��һ�����ԡ���ʹ�ö�����趨����
extern class Strategy* pStrategyGlobal;

// ����API�Ĳ��������׺�����ֿ�
extern char FRONT_ADDR[32];							// ����ǰ�õ�ַ
extern TUstpFtdcBrokerIDType	BROKER_ID;			// ���͹�˾����
extern TUstpFtdcInvestorIDType INVESTOR_ID;		// Ͷ���ߴ���
extern TUstpFtdcUserIDType USER_ID;                  // �û�����
extern TUstpFtdcPasswordType	PASSWORD;			// �û�����
extern TUstpFtdcExchangeIDType EXCHANGEID;         //������ID
extern char FRONT_ADDR2[32];						// ����ǰ�õ�ַ
extern CUstpFtdcRspInvestorAccountField *pInvestorAccount;

extern char *ppInstrumentID[];						// ppInstrumentID[] is in Global.cpp
// �˴�������
const int instrumentNum = 20;						// ��Լ����
const int marketDataListMaxLength = 600;					// �洢��MarketData��������300��Ϊ5����
const bool isOutputMdInfo = false;					// �Ƿ��������ʱ��MarketData��ص����ݣ���Ҫ���ڲ���


// request ID
extern int iRequestID;

//��������buttonֻ�ܵ�һ��
extern bool TradebtnOK;

// �߾���ʱ��
extern LARGE_INTEGER performanceFrequency;
extern LARGE_INTEGER performanceCounter;
extern LONGLONG frequency;

// strategy parameters
// Ԥ�����windowLength���ֵ��ʵ�ʲ����е�window_length��SetParameter����
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
	
	// Ĭ�Ϲ��캯�� 
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
	// ���캯��
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

	// Ĭ�Ϲ��캯�� 
	mySignal()
	{
		this->orderPriceType = NULL;
		this->direction = NULL;
		this->operation = NULL;
		strcpy(this->instrumentID, "");
		this->volume = NULL;
		this->price = NULL;
	}
	// ���캯�� 
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
	// ûд���캯��
} CMyFeed, *pCMyFeed;

typedef struct myPosition
{
	int volume; // �ֲ���
	int availableVolume; // ��ƽ����
	double aveCost; // ƽ���ɱ�
} CMyPosition, *pCMyPosition;

typedef struct myAccount
{
	double totalCaptital; // ���ʽ�
	double cash; // �����ֽ�
	double margin; // ��֤��
	double charge; // ������
} CMyAccount, *pCMyAccount;
