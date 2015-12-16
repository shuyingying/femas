#pragma once
class FemasMarketData
{
public:
	FemasMarketData(void);
	~FemasMarketData(void);

	FemasMarketData(string instrumentID, bool isWriteDB = false);
	// 将pDepthCtpMarketData所指的MarketData存到listCursor + 1的位置
	void SaveData(CUstpFtdcDepthMarketDataField* pDepthFemasMarketData);
	// 将事件hEventWriteDB设为有信号
	void WriteData(void);
	// 输出当前listCursor所指的MarketData到文件
	void WriteDB(void);
	// 返回合约名
	string getInstrumentID();
	// 获取当前listCursor所指的MarketData
	CUstpFtdcDepthMarketDataField* getCurMarketData(void);
	// 获取index位置的MarketData，若index超界，则返回空指针
	CUstpFtdcDepthMarketDataField* getMarketData(int index);
	// 返回当前listCursor
	int getListCursor(void);


public:
	HANDLE hEventWriteDB;				// 事件：写文件
	ofstream mdFile;					// 输出文件
	string instrumentID;				// 合约名

private:
	const int listMaxLength;			// 存储多少MarketData
	HANDLE hThread_Md;					// 输出MarketData的线程handle
	CRITICAL_SECTION critSec_MarketData;								// MarketData的临界区
	CUstpFtdcDepthMarketDataField marketDataList[marketDataListMaxLength];	// 存储MarketData的数组
	int listCursor;						// 当前listCursor位置
	// trigger
	bool m_bIsWriteDB;					// 是否写文件
};

