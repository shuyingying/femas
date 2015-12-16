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
	//获取三个输入 暂时假定有输入即为正确，进行登录，不能做验证
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

	   //初始化API
	  // 初始化LocalMemory
	LocalMemory::Initialize(0);

	// 是否使用Femas API
	isFemas = true;

	if (isFemas)
	{
			// 前置地址
			strcpy(FRONT_ADDR, "tcp://117.185.125.4:17198");		// 交易前置地址
			strcpy(BROKER_ID, "1004");								// 经纪公司代码
			strcpy(EXCHANGEID,"CFFEX");
			strcpy(FRONT_ADDR2, "tcp://117.185.125.4:17199");		// 行情前置地址

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
		// 使客户端开始与行情发布服务器建立连接
	    /*
		int a = 0;
	    int b = 0;
	    printf(pUserApi->GetVersion(a,b));  //获取当前使用的femas版本号
	    */
	    pUserApi->SetHeartbeatTimeout(300);// connect
		pUserApi->Init();
		
		// 控制是否运行策略
		//OrderManageSys::m_bIsRunStrategy = true;

		pTraderApi2 = CUstpFtdcTraderApi::CreateFtdcTraderApi();			// 创建TreaderApi
		pTraderSpi2 = new femasTraderSpi();
		pTraderApi2->RegisterFront(FRONT_ADDR);// connect		
		pTraderApi2->RegisterSpi((CUstpFtdcTraderSpi*)pTraderSpi2);			// 注册事件类
		pTraderApi2->SubscribePublicTopic(USTP_TERT_QUICK);				// 注册公有流
		pTraderApi2->SubscribePrivateTopic(USTP_TERT_QUICK);				// 注册私有流						
		pTraderApi2->Init();
		OrderManageSys::InitBasicOrder2();
	}
		//初始化获取投资者资金情况
	CUstpFtdcQryInvestorAccountField QryInvestorAcc;
	memset(&QryInvestorAcc,0,sizeof(CUstpFtdcQryInvestorAccountField));
	strcpy(QryInvestorAcc.BrokerID,BROKER_ID);
	pTraderApi2->ReqQryInvestorAccount(&QryInvestorAcc,iRequestID++);
	
	//持仓查询
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
		QMessageBox::warning(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请输入登录信息并确保正确！"),QMessageBox::Yes);
		ui->textUserID->setFocus();
	}
}