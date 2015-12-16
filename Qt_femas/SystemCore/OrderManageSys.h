#pragma once

class OrderManageSys
{
public:
	// orderTable���ٽ�����orderTable���ڼ�¼�ѷ���������״̬
	static CRITICAL_SECTION critSec_OrderTable;			
	static vector<CMyOrder> orderTable;
	// signal���е��ٽ�����֪ͨ�¼���signalQue���ڴ��Strategy���ɵĶ����źţ�OrderManageSys����ת��ΪCtp�������Ͳ�������״̬
	static CRITICAL_SECTION critSec_SignalQue;			
	static queue<CMySignal> signalQue;					
	static HANDLE hEventSignal;
	// RtnOrder���е��ٽ�����֪ͨ�¼���RtnOrderQue���ڴ��TraderSpi�յ���RtnOrder��OrderManageSys������������͸�Strategy
	static CRITICAL_SECTION critSec_RtnOrderQue;
	//femas RtnOrder
	static queue<CUstpFtdcOrderField> RtnOrderQue2;
	static HANDLE hEventOnRtnOrder;
	// RtnTrade���е��ٽ�����֪ͨ�¼���RtnTradeQue���ڴ��TraderSpi�յ���RtnTrade��OrderManageSys������������͸�Strategy
	static CRITICAL_SECTION critSec_RtnTradeQue;
	//femas RtnTrade
	static queue<CUstpFtdcTradeField> RtnTradeQue2;
	static HANDLE hEventOnRtnTrade;
	// ���������Ϣ���ļ����¼�����δ�õ���������Ҫ��������񽻸�����Ķ����̣߳�����õ�
	static HANDLE hEventOutput;
	// �洢�����¼������飬����WaitForMultipleObjects����
	static HANDLE hObject[4];
	// OrderManageSys���߳�handle
	static HANDLE hThread_OMS;
	// ����ļ�
	static ofstream entrustFile;
	// ������orderRef����������
	static int orderRef;
	// ������orderRef���ַ�������
	static TUstpFtdcUserOrderLocalIDType ORDER_REF;
	// �Ƿ����в���
	static bool m_bIsRunStrategy;
	// ����������������಻��ı�������ã��������Ч��
	static CUstpFtdcInputOrderField basicOrder2;
	// �ֲ�����
	// ��ͷ
	static vector<CMyPosition> longPosition;
	static CRITICAL_SECTION critSec_LongPos;
	// ��ͷ
	static vector<CMyPosition> shortPosition;
	static CRITICAL_SECTION critSec_ShortPos;
	// �ʽ�
	static CMyAccount myAccount;

public:
	// ���������Ϣ���ļ�
	static void OutputTrade(pCMyOrder pOrder);
	// �µ�������װ������priceType�ֱ�ִ��MyOrderInsertAnyPrice��MyOrderInsertLimitPrice��MyOrderInsertFOK
	static bool MyOrderInsert(char priceType, char long_short, char entry_exit, const char* instrumentID, int volume, double price);
	// ���м۵�
	static bool MyOrderInsertAnyPrice(char long_short, char entry_exit, const char* instrumentID, int volume, double price);
	// ���޼۵�
	static bool MyOrderInsertLimitPrice(char long_short, char entry_exit, const char* instrumentID, int volume, double price);
	// ��FOK��
	static bool MyOrderInsertFOK(char long_short, char entry_exit,const char* instrumentID, int volume, double price);
	// ��FAK��
	static bool MyOrderInsertFAK(char long_short, char entry_exit,const char* instrumentID, int volume, double price);
	// ����
	static bool MyOrderCancel(char* UserOrderLocalID);
	// ��ʼ����������
	static void InitBasicOrder2(void);
	// ��ʼ���ֲ�����
	static void initPosition(int instrumentIndex, char direction, int Volume, double price, int deltaAvailableVolume);
	// ���³ֲ�����
	static void UpdatePosition(int instrumentIndex, char direction, int deltaVolume, double price, int deltaAvailableVolume);

};

DWORD WINAPI ThreadFunc_OMS(LPVOID pParam);