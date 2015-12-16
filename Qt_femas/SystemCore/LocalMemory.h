#pragma once

/*
* ����ȡ�����������ݴ��ڴ�
*/
#include "PublicFuncs.h"


class LocalMemory
{
public:
	static void Initialize(bool isWriteDB);
	static void Destroy(void);
	// ��ʼ��MarketData�Ĵ洢�ռ������ļ�
	static void InitFemasMarketData();

public:
	// ΪMarketData���ٵĴ洢�ռ�
	// ����femasMarketData[instrumentNum]�д洢����FemasMarketData���ָ�룬����ͨ����ָ����ʶ�Ӧ�Ĵ洢�ռ�
	static FemasMarketData* femasMarketData[instrumentNum];
	// MdSpi��OnRtnDepthMarketData���յ���MarketData������˶����У�Ȼ����LocalMemory�̺߳�������
	static queue<CUstpFtdcDepthMarketDataField> newFemasMarketDataQueue;
	// newMarketDataQueue���ٽ���
	static CRITICAL_SECTION critSec_MdQue;
	// �¼�����MarketData������MdSpi֪ͨLocalMemory
	static HANDLE hEventNewMd;
	// �Ƿ�洢MarketData���ļ������ڿɸ�Ϊ���ݿ⣩
	static bool m_bIsWriteDB;
	// LocalMemory���߳�handle
	static HANDLE hThread_LocalMemory;
	// ӳ�������Լ��ӳ�䵽���±꣬�������
	static map<string, int> instrumentMap;
};

// ��hh:mm:ss���͵�ʱ���ַ���ת��Ϊ����hhmmss
long timeToInt(char *t);
// LocalMemory���߳���ʼ����
DWORD WINAPI ThreadFunc_LocalMemory(LPVOID pParam);

