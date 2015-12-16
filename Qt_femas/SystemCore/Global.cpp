#include "PublicFuncs.h"

//femasAPI对象
CUstpFtdcMduserApi* pUserApi;
femasMdSpi* pUserSpi;
CUstpFtdcTraderApi* pTraderApi2;
femasTraderSpi* pTraderSpi2;
CUstpFtdcRspInvestorAccountField *pInvestorAccount;

// 策略的全局变量，目前只使用一个策略。若使用多个，需定义多个
Strategy* pStrategyGlobal;

// 交易
char FRONT_ADDR[32] = "";								// 交易前置地址
TUstpFtdcBrokerIDType	BROKER_ID = "";					// 经纪公司代码
TUstpFtdcInvestorIDType INVESTOR_ID = "";				// 投资者代码
TUstpFtdcPasswordType  PASSWORD = "";					// 用户密码
TUstpFtdcUserIDType USER_ID = "";                       // 用户代码
TUstpFtdcExchangeIDType EXCHANGEID = "";                // 交易所ID
//int g_nOrdLocalID=0;                                    // 报单信息
// 行情
char FRONT_ADDR2[32] = "";								// 行情前置地址

// 此处可设置
char *ppInstrumentID[] = {  "IF1511", "IF1512", "IF1603","IF1606", "IC1511", "IC1512", "IC1603","IC1606", "IH1511", 
	"IH1512","IH1603","IH1606","HO1511-P-2300","HO1512-P-3100","HO1601-P-2450","HO1603-C-1700","HO1606-P-2600", "IO1511-P-3050","IO1512-C-4900","IO1603-P-5100"};			// 2个合约名，主力合约放前面
bool isTest;
bool isFemas;
double priceTick;
vector<string> instrumentVec;

// request ID
int iRequestID = 0;
bool TradebtnOK = false;
// 高精度时钟
LARGE_INTEGER performanceFrequency;
LARGE_INTEGER performanceCounter;
// 为了调用QueryPerformanceFrequency获得performanceFrequency，temp没有实质作用
BOOL temp = QueryPerformanceFrequency(&performanceFrequency);
LONGLONG frequency = performanceFrequency.QuadPart;