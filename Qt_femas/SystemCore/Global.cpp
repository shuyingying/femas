#include "PublicFuncs.h"

//femasAPI����
CUstpFtdcMduserApi* pUserApi;
femasMdSpi* pUserSpi;
CUstpFtdcTraderApi* pTraderApi2;
femasTraderSpi* pTraderSpi2;
CUstpFtdcRspInvestorAccountField *pInvestorAccount;

// ���Ե�ȫ�ֱ�����Ŀǰֻʹ��һ�����ԡ���ʹ�ö�����趨����
Strategy* pStrategyGlobal;

// ����
char FRONT_ADDR[32] = "";								// ����ǰ�õ�ַ
TUstpFtdcBrokerIDType	BROKER_ID = "";					// ���͹�˾����
TUstpFtdcInvestorIDType INVESTOR_ID = "";				// Ͷ���ߴ���
TUstpFtdcPasswordType  PASSWORD = "";					// �û�����
TUstpFtdcUserIDType USER_ID = "";                       // �û�����
TUstpFtdcExchangeIDType EXCHANGEID = "";                // ������ID
//int g_nOrdLocalID=0;                                    // ������Ϣ
// ����
char FRONT_ADDR2[32] = "";								// ����ǰ�õ�ַ

// �˴�������
char *ppInstrumentID[] = {  "IF1511", "IF1512", "IF1603","IF1606", "IC1511", "IC1512", "IC1603","IC1606", "IH1511", 
	"IH1512","IH1603","IH1606","HO1511-P-2300","HO1512-P-3100","HO1601-P-2450","HO1603-C-1700","HO1606-P-2600", "IO1511-P-3050","IO1512-C-4900","IO1603-P-5100"};			// 2����Լ����������Լ��ǰ��
bool isTest;
bool isFemas;
double priceTick;
vector<string> instrumentVec;

// request ID
int iRequestID = 0;
bool TradebtnOK = false;
// �߾���ʱ��
LARGE_INTEGER performanceFrequency;
LARGE_INTEGER performanceCounter;
// Ϊ�˵���QueryPerformanceFrequency���performanceFrequency��tempû��ʵ������
BOOL temp = QueryPerformanceFrequency(&performanceFrequency);
LONGLONG frequency = performanceFrequency.QuadPart;