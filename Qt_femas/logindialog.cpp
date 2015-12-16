#include <qmessagebox.h>
#include "logindialog.h"
#include "PublicFuncs.h"

LoginDialog::LoginDialog(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::Dialog)
{
	ui->setupUi(this);
	connect(ui->LoginIn,SIGNAL(clicked()),this,SLOT(on_button_clickd_LoginIn()));
}

LoginDialog::~LoginDialog()
{
	delete ui;
}

void LoginDialog::on_button_clickd_LoginIn()
{
	QString str0,str1,str2;
	//��ȡ�������� ��ʱ�ٶ������뼴Ϊ��ȷ�����е�¼����������֤
	str0 = ui->textInvestorID->text();
	str1 = ui->textUserID->text();
	str2 = ui->textPassword->text();
	
	if((!str0.isEmpty())&&(!str1.isEmpty())&&(!str2.isEmpty()))
	{
	  QByteArray textInvestorID = str0.toLatin1();
	  QByteArray textUserID = str1.toLatin1();
	  QByteArray textPassword = str2.toLatin1();
	  strcpy(INVESTOR_ID,textInvestorID.data());
	  strcpy(USER_ID,textUserID.data());
	  strcpy(PASSWORD,textPassword.data());	

	   //��ʼ��API
	  // ��ʼ��LocalMemory
	LocalMemory::Initialize(0);

	// �Ƿ�ʹ��Femas API
	isFemas = true;

	if (isFemas)
	{
			// ǰ�õ�ַ
			strcpy(FRONT_ADDR, "tcp://117.185.125.4:17198");		// ����ǰ�õ�ַ
			strcpy(BROKER_ID, "1004");								// ���͹�˾����
			strcpy(EXCHANGEID,"CFFEX");
			strcpy(FRONT_ADDR2, "tcp://117.185.125.4:17199");		// ����ǰ�õ�ַ

		LocalMemory::InitFemasMarketData();
		Sleep(500);

		// Initialize Market data Api
		printf("------->> Initialize Market data Api\n");
		pUserApi = CUstpFtdcMduserApi::CreateFtdcMduserApi();			// create MdApi
		pUserSpi = new femasMdSpi();
		pUserApi->RegisterSpi((CUstpFtdcMduserSpi*)pUserSpi);// register MdSpi

		int topicid = 100;
		pUserApi->SubscribeMarketDataTopic(topicid,USTP_TERT_RESTART);
		pUserApi->RegisterFront(FRONT_ADDR2);
		// ʹ�ͻ��˿�ʼ�����鷢����������������
	    /*
		int a = 0;
	    int b = 0;
	    printf(pUserApi->GetVersion(a,b));  //��ȡ��ǰʹ�õ�femas�汾��
	    */
	    pUserApi->SetHeartbeatTimeout(300);// connect
		pUserApi->Init();
		
		// �����Ƿ����в���
		//OrderManageSys::m_bIsRunStrategy = true;

		pTraderApi2 = CUstpFtdcTraderApi::CreateFtdcTraderApi();			// ����TreaderApi
		pTraderSpi2 = new femasTraderSpi();
		pTraderApi2->RegisterFront(FRONT_ADDR);// connect		
		pTraderApi2->RegisterSpi((CUstpFtdcTraderSpi*)pTraderSpi2);			// ע���¼���
		pTraderApi2->SubscribePublicTopic(USTP_TERT_QUICK);				// ע�ṫ����
		pTraderApi2->SubscribePrivateTopic(USTP_TERT_QUICK);				// ע��˽����						
		pTraderApi2->Init();
		OrderManageSys::InitBasicOrder2();
	}
		//��ʼ����ȡͶ�����ʽ����
	CUstpFtdcQryInvestorAccountField QryInvestorAcc;
	memset(&QryInvestorAcc,0,sizeof(CUstpFtdcQryInvestorAccountField));
	strcpy(QryInvestorAcc.BrokerID,BROKER_ID);
	pTraderApi2->ReqQryInvestorAccount(&QryInvestorAcc,iRequestID++);
	
	//�ֲֲ�ѯ
	CUstpFtdcQryInvestorPositionField QryInvestorPosition;
	memset(&QryInvestorPosition,0,sizeof(CUstpFtdcQryInvestorPositionField));
	strcpy(QryInvestorPosition.ExchangeID,"CFFEX");
	strcpy(QryInvestorPosition.BrokerID,BROKER_ID);
	strcpy(QryInvestorPosition.InvestorID,INVESTOR_ID);
	pTraderApi2->ReqQryInvestorPosition(&QryInvestorPosition,iRequestID++);
	
	accept();
	}
	else
	{
		QMessageBox::warning(this,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("�������¼��Ϣ��ȷ����ȷ��"),QMessageBox::Yes);
		ui->textUserID->setFocus();
	}
}