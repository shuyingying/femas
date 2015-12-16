#include "PublicFuncs.h"


FemasMarketData::FemasMarketData(void):listMaxLength(marketDataListMaxLength)
{
}


FemasMarketData::~FemasMarketData(void)
{
}


FemasMarketData::FemasMarketData(string _instrumentID, bool _isWriteDB):listMaxLength(marketDataListMaxLength)
{// list length = 600, 5mins data
	instrumentID = _instrumentID;
	m_bIsWriteDB = _isWriteDB;
	listCursor = -1;							// 注意listCursor是从-1开始的
	InitializeCriticalSection(&critSec_MarketData);
	for (int i=0; i<listMaxLength; i++)			// 预先分配MarketData的内存空间
	{
		CUstpFtdcDepthMarketDataField md;
		memset(&md, 0, sizeof(md));				// 初始化为默认值
		marketDataList[i] = md;
	}
}

// 返回合约名
string FemasMarketData::getInstrumentID()
{
	return instrumentID;
}

// 获取当前listCursor所指的MarketData
CUstpFtdcDepthMarketDataField* FemasMarketData::getCurMarketData(void)
{
	EnterCriticalSection(&critSec_MarketData);
	CUstpFtdcDepthMarketDataField* curMarketData = &marketDataList[listCursor==-1?0:listCursor];
	LeaveCriticalSection(&critSec_MarketData);
	return curMarketData;
}

// 获取index位置的MarketData，若index超界，则返回空指针
CUstpFtdcDepthMarketDataField* FemasMarketData::getMarketData(int index)
{
	if (index >= listMaxLength || index < 0)
	{
		printf("FemasMarketData::getMarketData: marketDataList index invalid!\n");
		return NULL;
	}
	EnterCriticalSection(&critSec_MarketData);
	CUstpFtdcDepthMarketDataField* marketData = &marketDataList[index];
	LeaveCriticalSection(&critSec_MarketData);
	return marketData;
}

// 返回当前listCursor
int FemasMarketData::getListCursor(void)
{
	EnterCriticalSection(&critSec_MarketData);
	int curosr = listCursor;
	LeaveCriticalSection(&critSec_MarketData);
	return curosr;
}

// 将pDepthFemasMarketData所指的MarketData存到listCursor + 1的位置
void FemasMarketData::SaveData(CUstpFtdcDepthMarketDataField* pDepthFemasMarketData)
{
	EnterCriticalSection(&critSec_MarketData);
	listCursor = (listCursor + 1) % listMaxLength;
	marketDataList[listCursor] = *pDepthFemasMarketData;
	LeaveCriticalSection(&critSec_MarketData);
}

// 将事件hEventWriteDB设为有信号
void FemasMarketData::WriteData(void)
{
	SetEvent(hEventWriteDB);
}

//// 输出MarketData的线程
//DWORD WINAPI ThreadFunc_Md(LPVOID pParam)
//{
//	FemasMarketData* md = (FemasMarketData*) pParam;
//	while (WAIT_OBJECT_0 == WaitForSingleObject(md->hEventWriteDB, INFINITE))
//	{
//		md->WriteDB();
//		ResetEvent(md->hEventWriteDB);
//	}
//	return 0;
//}

// 输出当前listCursor所指的MarketData到文件
void FemasMarketData::WriteDB(void)
{
	/*CUstpFtdcDepthMarketDataField* marketData = &marketDataList[listCursor];
	mdFile << "TradingDay=" << "\t" << marketData->TradingDay << "\t";
	mdFile << "InstrumentID=" << "\t" << marketData->InstrumentID << "\t";
	mdFile << "ExchangeID=" << "\t" << marketData->ExchangeID << "\t";
	mdFile << "ExchangeInstID=" << "\t" << marketData->ExchangeInstID << "\t";
	mdFile << "LastPrice=" << "\t" << marketData->LastPrice << "\t";
	mdFile << "PreSettlementPrice=" << "\t" << marketData->PreSettlementPrice << "\t";
	mdFile << "PreClosePrice=" << "\t" << marketData->PreClosePrice << "\t";
	mdFile << "PreOpenInterest=" << "\t" << marketData->PreOpenInterest << "\t";
	mdFile << "OpenPrice=" << "\t" << marketData->OpenPrice << "\t";
	mdFile << "HighestPrice=" << "\t" << marketData->HighestPrice << "\t";
	mdFile << "LowestPrice=" << "\t" << marketData->LowestPrice << "\t";
	mdFile << "Volume=" << "\t" << marketData->Volume << "\t";
	mdFile << "Turnover=" << "\t" << marketData->Turnover << "\t";
	mdFile << "OpenInterest=" << "\t" << marketData->OpenInterest << "\t";
	mdFile << "ClosePrice=" << "\t" << marketData->ClosePrice << "\t";
	mdFile << "SettlementPrice=" << "\t" << marketData->SettlementPrice << "\t";
	mdFile << "UpperLimitPrice=" << "\t" << marketData->UpperLimitPrice << "\t";
	mdFile << "LowerLimitPrice=" << "\t" << marketData->LowerLimitPrice << "\t";
	mdFile << "PreDelta=" << "\t" << marketData->PreDelta << "\t";
	mdFile << "CurrDelta=" << "\t" << marketData->CurrDelta << "\t";
	mdFile << "UpdateTime=" << "\t" << marketData->UpdateTime << "\t";
	mdFile << "UpdateMillisec=" << "\t" << marketData->UpdateMillisec << "\t";
	mdFile << "AskPrice1=" << "\t" << marketData->AskPrice1 << "\t";
	mdFile << "AskVolume1=" << "\t" << marketData->AskVolume1 << "\t";
	mdFile << "AskPrice2=" << "\t" << marketData->AskPrice2 << "\t";
	mdFile << "AskVolume2=" << "\t" << marketData->AskVolume2 << "\t";
	mdFile << "AskPrice3=" << "\t" << marketData->AskPrice3 << "\t";
	mdFile << "AskVolume3=" << "\t" << marketData->AskVolume3 << "\t";
	mdFile << "AskPrice4=" << "\t" << marketData->AskPrice4 << "\t";
	mdFile << "AskVolume4=" << "\t" << marketData->AskVolume4 << "\t";
	mdFile << "AskPrice5=" << "\t" << marketData->AskPrice5 << "\t";
	mdFile << "AskVolume5=" << "\t" << marketData->AskVolume5 << "\t";
	mdFile << "BidPrice1=" << "\t" << marketData->BidPrice1 << "\t";
	mdFile << "BidVolume1=" << "\t" << marketData->BidVolume1 << "\t";
	mdFile << "BidPrice2=" << "\t" << marketData->BidPrice2 << "\t";
	mdFile << "BidVolume2=" << "\t" << marketData->BidVolume2 << "\t";
	mdFile << "BidPrice3=" << "\t" << marketData->BidPrice3 << "\t";
	mdFile << "BidVolume3=" << "\t" << marketData->BidVolume3 << "\t";
	mdFile << "BidPrice4=" << "\t" << marketData->BidPrice4 << "\t";
	mdFile << "BidVolume4=" << "\t" << marketData->BidVolume4 << "\t";
	mdFile << "BidPrice5=" << "\t" << marketData->BidPrice5 << "\t";
	mdFile << "BidVolume5=" << "\t" << marketData->BidVolume5 << "\t";
	mdFile << "AveragePrice=" << "\t" << marketData->AveragePrice << endl;*/
}