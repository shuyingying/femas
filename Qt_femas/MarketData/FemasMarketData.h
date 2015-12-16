#pragma once
class FemasMarketData
{
public:
	FemasMarketData(void);
	~FemasMarketData(void);

	FemasMarketData(string instrumentID, bool isWriteDB = false);
	// ��pDepthCtpMarketData��ָ��MarketData�浽listCursor + 1��λ��
	void SaveData(CUstpFtdcDepthMarketDataField* pDepthFemasMarketData);
	// ���¼�hEventWriteDB��Ϊ���ź�
	void WriteData(void);
	// �����ǰlistCursor��ָ��MarketData���ļ�
	void WriteDB(void);
	// ���غ�Լ��
	string getInstrumentID();
	// ��ȡ��ǰlistCursor��ָ��MarketData
	CUstpFtdcDepthMarketDataField* getCurMarketData(void);
	// ��ȡindexλ�õ�MarketData����index���磬�򷵻ؿ�ָ��
	CUstpFtdcDepthMarketDataField* getMarketData(int index);
	// ���ص�ǰlistCursor
	int getListCursor(void);


public:
	HANDLE hEventWriteDB;				// �¼���д�ļ�
	ofstream mdFile;					// ����ļ�
	string instrumentID;				// ��Լ��

private:
	const int listMaxLength;			// �洢����MarketData
	HANDLE hThread_Md;					// ���MarketData���߳�handle
	CRITICAL_SECTION critSec_MarketData;								// MarketData���ٽ���
	CUstpFtdcDepthMarketDataField marketDataList[marketDataListMaxLength];	// �洢MarketData������
	int listCursor;						// ��ǰlistCursorλ��
	// trigger
	bool m_bIsWriteDB;					// �Ƿ�д�ļ�
};

