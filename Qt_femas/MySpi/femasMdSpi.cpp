#include "PublicFuncs.h"

LONGLONG f_timeLast = 0;
LONGLONG f_timeCur = 0;

void femasMdSpi::OnFrontConnected()
{
	printf("------->> %s\n", __FUNCTION__);
	CUstpFtdcReqUserLoginField reqUserLogin;
    memset(&reqUserLogin, 0, sizeof(reqUserLogin));
	strcpy(reqUserLogin.BrokerID, BROKER_ID);
	strcpy(reqUserLogin.UserID, USER_ID);
	strcpy(reqUserLogin.Password, PASSWORD);
	strcpy(reqUserLogin.TradingDay, pUserApi->GetTradingDay());
    int iResult = pUserApi->ReqUserLogin(&reqUserLogin, ++iRequestID);
	printf("------->> Request User Login: %s\n", ((iResult == 0) ? "success" : "fail"));
}

void femasMdSpi::OnFrontDisconnected()
{
	printf("------->> %s\n", __FUNCTION__);
	//printf("------->> Reason = %d\n", ErrorID);
	printf("OnFrontDisconnected.\n");
}

void femasMdSpi::OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("------->> %s\n", __FUNCTION__);
	printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
   // IsErrorRspInfo(pRspInfo);
}

void femasMdSpi::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("------->> %s\n", __FUNCTION__);
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///获取当前交易日
	//	printf("------->> Trading Day = %s\n", pUserApi->GetTradingDay());
		// 请求订阅行情
		SubscribeMarketData();	
	}
}

void femasMdSpi::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("------->> %s\n", __FUNCTION__);
	printf("UserID:[%s] \n",pRspUserLogout->UserID);
	printf("ParticipantID:[%s] \n",pRspUserLogout->BrokerID);
	printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
}

void femasMdSpi::OnRtnDepthMarketData(CUstpFtdcDepthMarketDataField *pMarketData)
{
//  printf("------->> %s\n", __FUNCTION__);
//	printf("%s,%s,%d,",pMarketData->InstrumentID,pMarketData->UpdateTime,pMarketData->UpdateMillisec);
//	printf("tradingDay=%s\n",pMarketData->TradingDay);
	QueryPerformanceCounter(&performanceCounter);
	f_timeLast = f_timeCur;
	if (isOutputMdInfo)
	{
		f_timeCur = performanceCounter.QuadPart;
	}

	EnterCriticalSection(&LocalMemory::critSec_MdQue);
	LocalMemory::newFemasMarketDataQueue.push(*pMarketData);
	SetEvent(LocalMemory::hEventNewMd);
	LeaveCriticalSection(&LocalMemory::critSec_MdQue);

	if (isOutputMdInfo)
	{
		QueryPerformanceCounter(&performanceCounter);
		double timeInterval = (double) ((f_timeCur - f_timeLast) * 1000000 / frequency);
		printf("time =%d, interval = %7g us, ID = %s\n", timeToInt(pMarketData->UpdateTime) * 1000 + pMarketData->UpdateMillisec, 
			timeInterval, pMarketData->InstrumentID);
	}
}

bool femasMdSpi::IsErrorRspInfo(CUstpFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
	{
		printf("------->> ErrorID = %d, ErrorMsg = %s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	return bResult;
}

void femasMdSpi::SubscribeMarketData()
{
	int iResult = pUserApi->SubMarketData(ppInstrumentID, instrumentNum);
	cerr << "--->>> 发送行情订阅请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void femasMdSpi::OnRspQryTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printf("OnRspQryTopic: %d ,%d\n",pDissemination->SequenceSeries,pDissemination->SequenceNo);
}

void femasMdSpi::OnRspSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("Sub 返回订阅合约：%s \n",pSpecificInstrument->InstrumentID);
}

void femasMdSpi::OnRspUnSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("UnSub 返回退订合约：%s \n",pSpecificInstrument->InstrumentID);
}