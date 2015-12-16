#include "PublicFuncs.h"

void femasTraderSpi::OnFrontConnected()
{
	printf("------->> %s\n", __FUNCTION__);
	CUstpFtdcReqUserLoginField reqUserLogin;
	memset(&reqUserLogin,0,sizeof(CUstpFtdcReqUserLoginField));	
	strcpy(reqUserLogin.BrokerID, BROKER_ID);
	strcpy(reqUserLogin.UserID, USER_ID);
	strcpy(reqUserLogin.Password, PASSWORD);
	int iResult = pTraderApi2->ReqUserLogin(&reqUserLogin, ++iRequestID);
	printf("------->> Request User Login: %s\n", ((iResult == 0) ? "success" : "fail"));
	Sleep(1000);
}

void femasTraderSpi::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin,
	CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("------->> %s\n", __FUNCTION__);
	if (bIsLast&&!IsErrorRspInfo(pRspInfo))
	{
		OrderManageSys::orderRef = atoi(pRspUserLogin->MaxOrderLocalID)+1;
		if (OrderManageSys::orderRef < 100000)
		{
			OrderManageSys::orderRef = 100001;
		}
		sprintf(OrderManageSys::ORDER_REF, "%012d", &OrderManageSys::orderRef);
		///��ȡ��ǰ������
		printf("------->> Trading Day = %s\n", pTraderApi2->GetTradingDay());
		///Ͷ���߽�����ȷ��
		//ReqSettlementInfoConfirm();

		//��ȡ���еĺ�Լ	
		CUstpFtdcQryInstrumentField pInstrument;
		memset(&pInstrument,0,sizeof(CUstpFtdcQryInstrumentField));
		pTraderApi2->ReqQryInstrument(&pInstrument,iRequestID++);
		
	}
	else
	{
       printf("-----------------------------\n");
		printf("��¼ʧ��...����ԭ��%s\n",pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
}

//traderSPI
//OnRspOrderInsert ����¼��Ӧ��
void femasTraderSpi::OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		printf("-----------------------------\n");
		printf("����ʧ�� ����ԭ��%s\n",pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if(pInputOrder==NULL)
	{
		printf("û�б�������\n");
		return;
	}
	printf("-----------------------------\n");
	printf("�����ɹ�\n");
	printf("-----------------------------\n");
	return ;
}

//�����ر�
void femasTraderSpi::OnRtnOrder(CUstpFtdcOrderField *pOrder)
{
	printf("-----------------------------\n");
	printf("�յ������ر�\n");
	Show(pOrder);
	EnterCriticalSection(&OrderManageSys::critSec_RtnOrderQue);
	OrderManageSys::RtnOrderQue2.push(*pOrder);
	SetEvent(OrderManageSys::hEventOnRtnOrder);		// inform the OMS
	LeaveCriticalSection(&OrderManageSys::critSec_RtnOrderQue);
	return ;
}

//�ɽ��ر�
void femasTraderSpi::OnRtnTrade(CUstpFtdcTradeField *pTrade)
{
	printf("-----------------------------\n");
	printf("�յ��ɽ��ر�\n");
	Show(pTrade);
	EnterCriticalSection(&OrderManageSys::critSec_RtnTradeQue);
	OrderManageSys::RtnTradeQue2.push(*pTrade);
	SetEvent(OrderManageSys::hEventOnRtnTrade);		// inform the OMS
	LeaveCriticalSection(&OrderManageSys::critSec_RtnTradeQue);
	return;
}

//�����ر�
void femasTraderSpi::OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		printf("-----------------------------\n");
		printf("����ʧ�� ����ԭ��%s\n",pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if(pOrderAction==NULL)
	{
		printf("û�г�������\n");
		return;
	}
	printf("-----------------------------\n");
	printf("�����ɹ�   �����������ҳ�����\n");
	printf("-----------------------------\n");
	return ;
}

//����¼�����ر�
void femasTraderSpi::OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		printf("-----------------------------\n");
		printf("��������ر�ʧ�� ����ԭ��%s\n",pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if(pInputOrder==NULL)
	{
		printf("û������\n");
		return;
	}
	printf("-----------------------------\n");
	printf("��������ر�\n");
	printf("-----------------------------\n");
	return ;
}

//������������ر�
void femasTraderSpi::OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		printf("-----------------------------\n");
		printf("��������ر�ʧ�� ����ԭ��%s\n",pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if(pOrderAction==NULL)
	{
		printf("û������\n");
		return;
	}
	printf("-----------------------------\n");
	printf("��������ر�\n");
	printf("-----------------------------\n");
	return ;
}

//������ѯӦ��
void femasTraderSpi::OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		printf("-----------------------------\n");
		printf("��ѯ����ʧ�� ����ԭ��%s\n",pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if(pOrder==NULL)
	{
		printf("û�в�ѯ����������\n");
		return;
	}
	Show(pOrder);
	return ;
}

//�ɽ�����ѯӦ��
void femasTraderSpi::OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
 if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		printf("-----------------------------\n");
		printf("��ѯ�ɽ�ʧ�� ����ԭ��%s\n",pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if(pTrade==NULL)
	{
		printf("û�в�ѯ���ɽ�����");
		return;
	}
	Show(pTrade);
	return ;
}

void femasTraderSpi::OnRspQryExchange(CUstpFtdcRspExchangeField *pRspExchange, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		printf("-----------------------------\n");
		printf("��ѯ������ʧ�� ����ԭ��%s\n",pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	if (pRspExchange==NULL)
	{
		printf("û�в�ѯ����������Ϣ\n");
		return ;
	}
	printf("-----------------------------\n");
	printf("[%s]\n",pRspExchange->ExchangeID);
	printf("[%s]\n",pRspExchange->ExchangeName);
	printf("-----------------------------\n");
	return;
}

void femasTraderSpi::OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		printf("-----------------------------\n");
		printf("��ѯͶ�����˻�ʧ�� ����ԭ��%s\n",pRspInfo->ErrorMsg);
		printf("-----------------------------\n");
		return;
	}
	
	if (pRspInvestorAccount==NULL)
	{
		printf("û�в�ѯ��Ͷ�����˻�\n");
		return ;
	}
	pInvestorAccount = pRspInvestorAccount;
	TradebtnOK = true;
	/*
	printf("-----------------------------\n");
	printf("Ͷ���߱��=[%s]\n",pRspInvestorAccount->InvestorID);
	printf("�ʽ��ʺ�=[%s]\n",pRspInvestorAccount->AccountID);
	printf("�ϴν���׼����=[%lf]\n",pRspInvestorAccount->PreBalance);
	printf("�����=[%lf]\n",pRspInvestorAccount->Deposit);
	printf("������=[%lf]\n",pRspInvestorAccount->Withdraw);
	printf("����ı�֤��=[%lf]\n",pRspInvestorAccount->FrozenMargin);
	printf("����������=[%lf]\n",pRspInvestorAccount->FrozenFee);
	printf("������=[%lf]\n",pRspInvestorAccount->Fee);
	printf("ƽ��ӯ��=[%lf]\n",pRspInvestorAccount->CloseProfit);
	printf("�ֲ�ӯ��=[%lf]\n",pRspInvestorAccount->PositionProfit);
	printf("�����ʽ�=[%lf]\n",pRspInvestorAccount->Available);
	printf("��ͷ����ı�֤��=[%lf]\n",pRspInvestorAccount->LongFrozenMargin);
	printf("��ͷ����ı�֤��=[%lf]\n",pRspInvestorAccount->ShortFrozenMargin);
	printf("��ͷ��֤��=[%lf]\n",pRspInvestorAccount->LongMargin);
	printf("��ͷ��֤��=[%lf]\n",pRspInvestorAccount->ShortMargin);
	printf("-----------------------------\n");
	*/
}
void femasTraderSpi::OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		printf("��ѯ���ױ���ʧ�� ����ԭ��%s\n",pRspInfo->ErrorMsg);
		return;
	}
	
	if (pRspInstrument==NULL)
	{
		printf("û�в�ѯ����Լ����\n");
		return ;
	}
	
	// ��¼��Լ��
	instrumentVec.push_back(pRspInstrument->InstrumentID);
	//��ʾ 
	//Show(pRspInstrument);

	if (bIsLast)
	{
		char fileName[128];
		sprintf(fileName, "instrumentIDs.txt");
		ofstream nameFile;
		nameFile.open(fileName);
		for (int i=0; i<instrumentVec.size(); i++)
		{
			nameFile << instrumentVec[i] << endl;
		}
	}
	return ;
}

void femasTraderSpi::OnRspQryTradingCode(CUstpFtdcRspTradingCodeField *pTradingCode, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		printf("��ѯ���ױ���ʧ�� ����ԭ��%s\n",pRspInfo->ErrorMsg);
		return;
	}
	
	if (pTradingCode==NULL)
	{
		printf("û�в�ѯ�����ױ���\n");
		return ;
	}
	printf("-----------------------------\n");
	printf("����������=[%s]\n",pTradingCode->ExchangeID);
	printf("���͹�˾���=[%s]\n",pTradingCode->BrokerID);
	printf("Ͷ���߱��=[%s]\n",pTradingCode->InvestorID);
	printf("�ͻ�����=[%s]\n",pTradingCode->ClientID);
	printf("�ͻ�����Ȩ��=[%d]\n",pTradingCode->ClientRight);
	printf("�Ƿ��Ծ=[%c]\n",pTradingCode->IsActive);
	printf("-----------------------------\n");
	return ;
}

void femasTraderSpi::OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		printf("��ѯͶ���ֲ߳� ����ԭ��%s\n",pRspInfo->ErrorMsg);
		return;
	}
	
	if (pRspInvestorPosition==NULL)
	{
		printf("û�в�ѯ��Ͷ���ֲ߳�\n");
		return ;
	}
	
	printf("-----------------------------\n");
	printf("����������=[%s]\n",pRspInvestorPosition->ExchangeID);
	printf("���͹�˾���=[%s]\n",pRspInvestorPosition->BrokerID);
	printf("Ͷ���߱��=[%s]\n",pRspInvestorPosition->InvestorID);
	printf("�ͻ�����=[%s]\n",pRspInvestorPosition->ClientID);
	printf("��Լ����=[%s]\n",pRspInvestorPosition->InstrumentID);
	printf("��������=[%c]\n",pRspInvestorPosition->Direction);
	printf("��ֲ���=[%d]\n",pRspInvestorPosition->Position);
	printf("-----------------------------\n");
	
	OrderManageSys::initPosition(LocalMemory::instrumentMap[pRspInvestorPosition->InstrumentID], pRspInvestorPosition->Direction, 
		pRspInvestorPosition->Position, pRspInvestorPosition->PositionCost, pRspInvestorPosition->Position);
	return ;
}

	///Ͷ�����������ʲ�ѯӦ��
void femasTraderSpi::OnRspQryInvestorFee(CUstpFtdcInvestorFeeField *pInvestorFee, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		printf("��ѯͶ������������ʧ�� ����ԭ��%s\n",pRspInfo->ErrorMsg);
		return;
	}
	
	if (pInvestorFee==NULL)
	{
		printf("û�в�ѯ��Ͷ������������\n");
		return ;
	}
	printf("-----------------------------\n");
	printf("���͹�˾���=[%s]\n",pInvestorFee->BrokerID);
	printf("��Լ����=[%s]\n",pInvestorFee->InstrumentID);
	printf("�ͻ�����=[%s]\n",pInvestorFee->ClientID);
	printf("���������Ѱ�����=[%f]\n",pInvestorFee->OpenFeeRate);
	printf("���������Ѱ�����=[%f]\n",pInvestorFee->OpenFeeAmt);
	printf("ƽ�������Ѱ�����=[%f]\n",pInvestorFee->OffsetFeeRate);
	printf("ƽ�������Ѱ�����=[%f]\n",pInvestorFee->OffsetFeeAmt);
	printf("ƽ��������Ѱ�����=[%f]\n",pInvestorFee->OTFeeRate);
	printf("ƽ��������Ѱ�����=[%f]\n",pInvestorFee->OTFeeAmt);
	printf("-----------------------------\n");
	return ;
}

	///Ͷ���߱�֤���ʲ�ѯӦ��
void femasTraderSpi::OnRspQryInvestorMargin(CUstpFtdcInvestorMarginField *pInvestorMargin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		printf("��ѯͶ���߱�֤����ʧ�� ����ԭ��%s\n",pRspInfo->ErrorMsg);
		return;
	}
	
	if (pInvestorMargin==NULL)
	{
		printf("û�в�ѯ��Ͷ���߱�֤����\n");
		return ;
	}
	printf("-----------------------------\n");
	printf("���͹�˾���=[%s]\n",pInvestorMargin->BrokerID);
	printf("��Լ����=[%s]\n",pInvestorMargin->InstrumentID);
	printf("�ͻ�����=[%s]\n",pInvestorMargin->ClientID);
	printf("��ͷռ�ñ�֤�𰴱���=[%f]\n",pInvestorMargin->LongMarginRate);
	printf("��ͷ��֤������=[%f]\n",pInvestorMargin->LongMarginAmt);
	printf("��ͷռ�ñ�֤�𰴱���=[%f]\n",pInvestorMargin->ShortMarginRate);
	printf("��ͷ��֤������=[%f]\n",pInvestorMargin->ShortMarginAmt);
	printf("-----------------------------\n");
	return ;

}


void femasTraderSpi::OnRspQryComplianceParam(CUstpFtdcRspComplianceParamField *pRspComplianceParam, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (pRspInfo!=NULL&&pRspInfo->ErrorID!=0)
	{
		printf("��ѯ�Ϲ����ʧ�� ����ԭ��%s\n",pRspInfo->ErrorMsg);
		return;
	}
	
	if (pRspComplianceParam==NULL)
	{
		printf("û�в�ѯ���Ϲ����\n");
		return ;
	}
	printf("-----------------------------\n");
	printf("���͹�˾���=[%s]\n",pRspComplianceParam->BrokerID);
	printf("�ͻ�����=[%s]\n",pRspComplianceParam->ClientID);
	printf("ÿ����󱨵���=[%d]\n",pRspComplianceParam->DailyMaxOrder);
	printf("ÿ����󳷵���=[%d]\n",pRspComplianceParam->DailyMaxOrderAction);
	printf("ÿ��������=[%d]\n",pRspComplianceParam->DailyMaxErrorOrder);
	printf("ÿ����󱨵���=[%d]\n",pRspComplianceParam->DailyMaxOrderVolume);
	printf("ÿ����󳷵���=[%d]\n",pRspComplianceParam->DailyMaxOrderActionVolume);
	printf("-----------------------------\n");
	return ;


}

//��������ر�
void femasTraderSpi::OnRtnInvestorAccountDeposit(CUstpFtdcInvestorAccountDepositResField *pInvestorAccountDepositRes)
{
	if (pInvestorAccountDepositRes==NULL)
	{
		printf("û���ʽ�������Ϣ\n");
		return ;
	}

	printf("-----------------------------\n");
	printf("���͹�˾���=[%s]\n",pInvestorAccountDepositRes->BrokerID);
	printf("�û����룽[%s]\n",pInvestorAccountDepositRes->UserID);
	printf("Ͷ���߱��=[%s]\n",pInvestorAccountDepositRes->InvestorID);
	printf("�ʽ��˺�=[%s]\n",pInvestorAccountDepositRes->AccountID);
	printf("�ʽ���ˮ�ţ�[%s]\n",pInvestorAccountDepositRes->AccountSeqNo);
	printf("��[%s]\n",pInvestorAccountDepositRes->Amount);
	printf("�������[%s]\n",pInvestorAccountDepositRes->AmountDirection);
	printf("�����ʽ�[%s]\n",pInvestorAccountDepositRes->Available);
	printf("����׼����[%s]\n",pInvestorAccountDepositRes->Balance);
	printf("-----------------------------\n");
	return ;
}

//֮��ĳ���UI����ʾ��Ϣ����ʾ
void femasTraderSpi::Show(CUstpFtdcTradeField *pTrade)
{
	printf("-----------------------------\n");
	printf("����������=[%s]\n",pTrade->ExchangeID);
	printf("������=[%s]\n",pTrade->TradingDay);
	printf("��Ա���=[%s]\n",pTrade->ParticipantID);
	printf("�µ�ϯλ��=[%s]\n",pTrade->SeatID);
	printf("Ͷ���߱��=[%s]\n",pTrade->InvestorID);
	printf("�ͻ���=[%s]\n",pTrade->ClientID);
	printf("�ɽ����=[%s]\n",pTrade->TradeID);

	printf("�û����ر�����=[%s]\n",pTrade->UserOrderLocalID);
	printf("��Լ����=[%s]\n",pTrade->InstrumentID);
	printf("��������=[%c]\n",pTrade->Direction);
	printf("��ƽ��־=[%c]\n",pTrade->OffsetFlag);
	printf("Ͷ���ױ���־=[%c]\n",pTrade->HedgeFlag);
	printf("�ɽ��۸�=[%lf]\n",pTrade->TradePrice);
	printf("�ɽ�����=[%d]\n",pTrade->TradeVolume);
	printf("�����Ա���=[%s]\n",pTrade->ClearingPartID);
	
	printf("-----------------------------\n");
	return ;
}

void femasTraderSpi::Show(CUstpFtdcOrderField *pOrder)
{
	printf("-----------------------------\n");
	printf("����������=[%s]\n",pOrder->ExchangeID);
	printf("������=[%s]\n",pOrder->TradingDay);
	printf("��Ա���=[%s]\n",pOrder->ParticipantID);
	printf("�µ�ϯλ��=[%s]\n",pOrder->SeatID);
	printf("Ͷ���߱��=[%s]\n",pOrder->InvestorID);
	printf("�ͻ���=[%s]\n",pOrder->ClientID);
	printf("ϵͳ�������=[%s]\n",pOrder->OrderSysID);
	printf("���ر������=[%s]\n",pOrder->OrderLocalID);
	printf("�û����ر�����=[%s]\n",pOrder->UserOrderLocalID);
	printf("��Լ����=[%s]\n",pOrder->InstrumentID);
	printf("�����۸�����=[%c]\n",pOrder->OrderPriceType);
	printf("��������=[%c]\n",pOrder->Direction);
	printf("��ƽ��־=[%c]\n",pOrder->OffsetFlag);
	printf("Ͷ���ױ���־=[%c]\n",pOrder->HedgeFlag);
	printf("�۸�=[%lf]\n",pOrder->LimitPrice);
	printf("����=[%d]\n",pOrder->Volume);
	printf("������Դ=[%c]\n",pOrder->OrderSource);
	printf("����״̬=[%c]\n",pOrder->OrderStatus);
	printf("����ʱ��=[%s]\n",pOrder->InsertTime);
	printf("����ʱ��=[%s]\n",pOrder->CancelTime);
	printf("��Ч������=[%c]\n",pOrder->TimeCondition);
	printf("GTD����=[%s]\n",pOrder->GTDDate);
	printf("��С�ɽ���=[%d]\n",pOrder->MinVolume);
	printf("ֹ���=[%lf]\n",pOrder->StopPrice);
	printf("ǿƽԭ��=[%c]\n",pOrder->ForceCloseReason);
	printf("�Զ������־=[%d]\n",pOrder->IsAutoSuspend);
	printf("-----------------------------\n");
	return ;
}

void femasTraderSpi::Show(CUstpFtdcRspInstrumentField *pRspInstrument)
{
	printf("-----------------------------\n");
	printf("����������=[%s]\n",pRspInstrument->ExchangeID);
	printf("Ʒ�ִ���=[%s]\n",pRspInstrument->ProductID);
	printf("Ʒ������=[%s]\n",pRspInstrument->ProductName);
	printf("��Լ����=[%s]\n",pRspInstrument->InstrumentID);
	printf("��Լ����=[%s]\n",pRspInstrument->InstrumentName);
	printf("�������=[%d]\n",pRspInstrument->DeliveryYear);
	printf("������=[%d]\n",pRspInstrument->DeliveryMonth);
	printf("�޼۵�����µ���=[%d]\n",pRspInstrument->MaxLimitOrderVolume);
	printf("�޼۵���С�µ���=[%d]\n",pRspInstrument->MinLimitOrderVolume);
	printf("�м۵�����µ���=[%d]\n",pRspInstrument->MaxMarketOrderVolume);
	printf("�м۵���С�µ���=[%d]\n",pRspInstrument->MinMarketOrderVolume);
	
	printf("��������=[%d]\n",pRspInstrument->VolumeMultiple);
	printf("���۵�λ=[%lf]\n",pRspInstrument->PriceTick);
	printf("����=[%c]\n",pRspInstrument->Currency);
	printf("��ͷ�޲�=[%d]\n",pRspInstrument->LongPosLimit);
	printf("��ͷ�޲�=[%d]\n",pRspInstrument->ShortPosLimit);
	printf("��ͣ���=[%lf]\n",pRspInstrument->LowerLimitPrice);
	printf("��ͣ���=[%lf]\n",pRspInstrument->UpperLimitPrice);
	printf("�����=[%lf]\n",pRspInstrument->PreSettlementPrice);
	printf("��Լ����״̬=[%c]\n",pRspInstrument->InstrumentStatus);
	
	printf("������=[%s]\n",pRspInstrument->CreateDate);
	printf("������=[%s]\n",pRspInstrument->OpenDate);
	printf("������=[%s]\n",pRspInstrument->ExpireDate);
	printf("��ʼ������=[%s]\n",pRspInstrument->StartDelivDate);
	printf("��󽻸���=[%s]\n",pRspInstrument->EndDelivDate);
	printf("���ƻ�׼��=[%lf]\n",pRspInstrument->BasisPrice);
	printf("��ǰ�Ƿ���=[%d]\n",pRspInstrument->IsTrading);
	printf("������Ʒ����=[%s]\n",pRspInstrument->UnderlyingInstrID);
	printf("�ֲ�����=[%c]\n",pRspInstrument->PositionType);
	printf("ִ�м�=[%lf]\n",pRspInstrument->StrikePrice);
	printf("��Ȩ����=[%c]\n",pRspInstrument->OptionsType);
	printf("-----------------------------\n");
	
}
bool femasTraderSpi::IsErrorRspInfo(CUstpFtdcRspInfoField *pRspInfo)
{
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		printf("--->>> ErrorID= %d, pRspInfo->ErrorMsg= %s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	return bResult;
}