#include <QtWidgets/QApplication>
#include "qt_femas.h"
#include "PublicFuncs.h"
#include "qbuttongroup.h"
#include <qmessagebox.h>
#include <qtextcodec.h>
#include <QDesktopWidget>
#include <Qrect>
#include <qrect.h>

Qt_femas::Qt_femas(QWidget *parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);

	//����Ӧ��Ļ��С
	QDesktopWidget *desktop = QApplication::desktop();
    int screenWidth = desktop->screenGeometry().width();
    int screenHeight = desktop->screenGeometry().height();
	ui.widgetLA->resize(screenWidth,screenHeight);
	ui.widgetLB->resize(screenWidth,screenHeight);
	
	//����currentindexΪ�ֲ���ϸ��ʾ
	ui.myOrder->setCurrentIndex(2);
	//�ϲ���Ԫ��
	ui.MyFunds->setSpan(0,8,2,1);
	ui.MyFunds->setSpan(0,10,2,1);
	//��Ӱ�ť "��ʾ����"
	QWidget *widgetPush = new QWidget(ui.MyFunds);
	QHBoxLayout *hLayout = new QHBoxLayout();
	QPushButton *pushButton_test = new QPushButton(QString::fromLocal8Bit("��ʾ����")); 
	pushButton_test->resize(5,6);
	hLayout->addWidget(pushButton_test);
	hLayout->setMargin(0);
	hLayout->setAlignment(pushButton_test,Qt::AlignCenter);
	widgetPush->setLayout(hLayout);
	ui.MyFunds->setCellWidget(0, 8, widgetPush); 
	
	//��Ӱ�ť ����ȡ�ʽ𣬳ֲ� �ȿ�ʼ���ס�
	QWidget *widgetPush1 = new QWidget(ui.MyFunds);
	QHBoxLayout *hLayout1 = new QHBoxLayout();
	QPushButton *pushButton_test1 = new QPushButton(QString::fromLocal8Bit("���Խ���")); 
	pushButton_test1->resize(9,10);
	hLayout1->addWidget(pushButton_test1);
	hLayout1->setMargin(0);
	hLayout1->setAlignment(pushButton_test1,Qt::AlignCenter);
	widgetPush1->setLayout(hLayout1);
	ui.MyFunds->setCellWidget(0, 10, widgetPush1); 

	//��ʱ�����������ݵ��߳�mdThread����
	mdThread = new QThread;
	mdTime = new QTimer;
	mdTime->setSingleShot(true);
	mdTime->start(100);
	mdTime->moveToThread(mdThread);
	inAccountTime = new QTimer;
	inAccountTime->start(100);
	inAccountTime->moveToThread(mdThread);
	/*
	* ��ʱ���� �������� +������ί�е���ʾ + �ֲ���ʾ
	*/
	connect(mdTime,SIGNAL(timeout()),this,SLOT(updateMD()),Qt::DirectConnection);
	connect(mdTime,SIGNAL(timeout()),this,SLOT(updateEntrustOrder()),Qt::DirectConnection);
	connect(mdTime,SIGNAL(timeout()),this,SLOT(updatePositionTable()),Qt::DirectConnection);
	connect(inAccountTime,SIGNAL(timeout()),this,SLOT(initInvestorAccount()),Qt::DirectConnection);

	connect(pushButton_test,SIGNAL(clicked()),this,SLOT(connectMduser()));//������ʾ
	connect(pushButton_test1,SIGNAL(clicked()),this,SLOT(initTrader()));//������ʾ
	connect(pushButton_test,SIGNAL(clicked()),this,SLOT(QryInvestorPosition()));//��¼ʱ�ֲֲ�ѯ
	//�µ�
	connect(ui.btnPlaceOrder,SIGNAL(clicked()),this,SLOT(onPlaceOrder()));
	connect(ui.mduserTable,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(onMdSelect(int,int)));

	//��ʼ���µ��Ľ���
	ui.newOrder->setEditable(true);
	for(int j=0;j<instrumentNum;j++)
	{
		ui.newOrder->addItem(ppInstrumentID[j]);
	}
	//��ť�����ʼ��������ID
	btnBuy = new QButtonGroup(this);
	btnBuy->addButton(ui.order_buy,0);
	btnBuy->addButton(ui.order_sale,1);
	ui.order_buy->setChecked(true);

	btnKaiping = new QButtonGroup(this); 
	btnKaiping->addButton(ui.kaicang,0);
	btnKaiping->addButton(ui.pingcang,1);
	ui.kaicang->setChecked(true);

	btnPrice = new QButtonGroup(this);
	btnPrice->addButton(ui.limitprice,0);
	btnPrice->addButton(ui.anyprice,1);
	ui.limitprice->setChecked(true);

	connect(ui.reset,SIGNAL(clicked()),this,SLOT(onResetOrder()));

	//���ñ������
	ui.mduserTable->setRowCount(instrumentNum);
	ui.entrust_Order->setRowCount(instrumentNum*3);
	ui.finish_Order->setRowCount(instrumentNum*3);
	ui.position_detail->setRowCount(instrumentNum*2);
}

Qt_femas::~Qt_femas()
{
	mdThread->quit();
	mdThread->wait();
	delete mdTime;
	delete mdThread;
	delete btnBuy;
	delete btnKaiping;
	//delete ui; 
}

void Qt_femas::connectMduser()
{
	mdThread->start();
}

void Qt_femas::updateMD()
{
	ui.mduserTable->clearContents();
	for(int i=0;i<instrumentNum;i++)
	{
	CUstpFtdcDepthMarketDataField *pdata = (LocalMemory::femasMarketData[i])->getCurMarketData();
	ui.mduserTable->setItem(i,0,new QTableWidgetItem(ppInstrumentID[i]));//��Լ����
	ui.mduserTable->setItem(i,1,new QTableWidgetItem(QString::number((pdata->LastPrice),'f',1)));//���¼�
	ui.mduserTable->setItem(i,2,new QTableWidgetItem(QString::number((pdata->BidPrice1),'f',1)));//�����
	ui.mduserTable->setItem(i,3,new QTableWidgetItem(QString::number(pdata->BidVolume1)));//����
	ui.mduserTable->setItem(i,4,new QTableWidgetItem(QString::number((pdata->AskPrice1),'f',1)));//������
	ui.mduserTable->setItem(i,5,new QTableWidgetItem(QString::number(pdata->AskVolume1)));//������
	ui.mduserTable->setItem(i,6,new QTableWidgetItem(QString::number(pdata->Volume)));//�ɽ���
	ui.mduserTable->setItem(i,7,new QTableWidgetItem(QString::number(pdata->OpenInterest)));//�ֲ���
	ui.mduserTable->setItem(i,8,new QTableWidgetItem(QString::number((pdata->UpperLimitPrice),'f',1)));//��ͣ���
	ui.mduserTable->setItem(i,9,new QTableWidgetItem(QString::number((pdata->LowerLimitPrice),'f',1)));//��ͣ���
	ui.mduserTable->setItem(i,10,new QTableWidgetItem(QString::number((pdata->OpenPrice),'f',1)));//����
	ui.mduserTable->setItem(i,11,new QTableWidgetItem(QString::number((pdata->PreClosePrice),'f',1)));//����
	ui.mduserTable->setItem(i,12,new QTableWidgetItem(QString::number((pdata->HighestPrice),'f',1)));//��߼�
	ui.mduserTable->setItem(i,13,new QTableWidgetItem(QString::number((pdata->LowestPrice),'f',1)));//��ͼ�
	ui.mduserTable->setItem(i,14,new QTableWidgetItem(QString::number((pdata->PreSettlementPrice),'f',1)));//������
	ui.mduserTable->setItem(i,15,new QTableWidgetItem(QString::number((pdata->Turnover),'f',1)));//�ɽ����
	ui.mduserTable->setItem(i,16,new QTableWidgetItem(pdata->UpdateTime));//����޸�ʱ��
	}
	mdTime->start(500);
}

//˫��ѡ�������л�ö�Ӧ��Լ �µ���Ϣ
void Qt_femas::onMdSelect(int row,int column)
{
	ui.newOrder->setCurrentText(ui.mduserTable->item(row,0)->text());
	//��ָ���۵�QSpinbox��ʾ���¼�
	ui.order_price->setValue((ui.mduserTable->item(row,1)->text()).toDouble());
	ui.trader_hand->setValue(1);
}

//���ö���������Ϣ
void Qt_femas::onResetOrder()
{
	ui.order_price->setValue(0.0);
	ui.trader_hand->setValue(1);
}

//�¶�������
void Qt_femas::onPlaceOrder()
{
	string str = ui.newOrder->currentText().toStdString();
	/*
	* �ж�������Ƿ�Ϊ�ɽ��׺�Լ
	*/
	if(str.empty())
	{
		QMessageBox::about(NULL,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("������Ҫ���׵ĺ�Լ����"));
	   return;
	}
	
	int temp;
	for(temp = 0;temp<instrumentNum;temp++)
	{
		if(strcmp(str.c_str(),ppInstrumentID[temp]))
		{
			break;
		}
	}
	if(temp==instrumentNum)
	{
	  QMessageBox::about(NULL,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("�������ѡ��Ϸ��ĺ�Լ"));
	  return;
	}

	//��ȡ�������򡢿�ƽ��
	char long_short = USTP_FTDC_D_Buy;
	char entry_exit = USTP_FTDC_OF_Open;
	QString str1,str2;
	switch(btnBuy->checkedId())
	{
	case 0:
		long_short = USTP_FTDC_D_Buy;//����
		str1 = QString::fromLocal8Bit("����");
		break;
	case 1:
		long_short = USTP_FTDC_D_Sell;//����
		str1 = QString::fromLocal8Bit("����");
		break;
	default:break;
	}

	switch(btnKaiping->checkedId())
	{
	case 0://ƽ��
		entry_exit = USTP_FTDC_OF_Open;
		str2 = QString::fromLocal8Bit("����");
		break;
	case 1:
		entry_exit = USTP_FTDC_OF_Close;
		str2 = QString::fromLocal8Bit("ƽ��");
		break;
	default:break;
	}

	char* instrumentID = ppInstrumentID[LocalMemory::instrumentMap[str]];//��һ�����Ϊɶ��Ҫ

	//����µ������ͼ۸�
	int volume = ui.trader_hand->value();
	double price = ui.order_price->value();
	//�ж�ѡ��ĺ�Լ�����������һ������Ӧ�������ȡ��۸�����
	int index_t = LocalMemory::instrumentMap[str];
	if(price>(ui.mduserTable->item(index_t,8)->text()).toDouble())
	     {
				 QMessageBox::about(NULL,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("���۳�����ͣ��ۣ�"));
				 return;
		  }
			else if(price<(ui.mduserTable->item(index_t,9)->text()).toDouble())
		 {
				QMessageBox::about(NULL,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("���۵��ڵ�ͣ��ۣ�"));
				return;
		 }
	
	QString orderMessage = QString::fromLocal8Bit("�µ���")+str2+str1+QString::fromStdString(str)+QString("  ")+ QString::number(volume)+QString::fromLocal8Bit(" �� �ڼ۸� ")+QString::number(price,'f',1);
	switch(QMessageBox::question(NULL,QString::fromLocal8Bit("ȷ�϶���"),orderMessage,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
	{
	case QMessageBox::Yes:
	{
		if(btnPrice->checkedId()==0)//���޼۵�
		{
		OrderManageSys::MyOrderInsertLimitPrice(long_short, entry_exit, instrumentID, volume, price);
		// ����orderTable
		// record time
		QueryPerformanceCounter(&performanceCounter);
		CMyOrder order(NULL,
			NULL,
			(OrderManageSys::orderRef)-1,
			instrumentID,
			long_short,
			entry_exit,
			volume,
			0,
			price,
			price,
			0,
			USTP_FTDC_OS_AcceptedNoReply,
			0,
			0,
			0,
			performanceCounter.QuadPart,
			0,
			0,
			CONST_ORDER_TYPE_LimitPrice,
			pStrategyGlobal);
		OrderManageSys::orderTable.push_back(order);
		}
		else
			if(btnPrice->checkedId()==1)//���м۵�
		{
		OrderManageSys::MyOrderInsertLimitPrice(long_short, entry_exit, instrumentID, volume, price);
		// ����orderTable
		// record time
		QueryPerformanceCounter(&performanceCounter);
		CMyOrder order(NULL,
			NULL,
			(OrderManageSys::orderRef)-1,
			instrumentID,
			long_short,
			entry_exit,
			volume,
			0,
			price,
			price,
			0,
			USTP_FTDC_OS_AcceptedNoReply,
			0,
			0,
			0,
			performanceCounter.QuadPart,
			0,
			0,
			CONST_ORDER_TYPE_LimitPrice,
			pStrategyGlobal);
		OrderManageSys::orderTable.push_back(order);
		}
		//QMessageBox::about(NULL,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("�µ��ɹ���"));
		break;
	}
	case QMessageBox::No:break;
	default:break;
	}
}

//ʵʱ�����ʽ�״��
void Qt_femas::initInvestorAccount()
{
	if(TradebtnOK){
		//���������û��ʽ������
	ui.MyFunds->setItem(1,0,new QTableWidgetItem(QString(pInvestorAccount->InvestorID)));
	ui.MyFunds->setItem(1,1,new QTableWidgetItem(QString::number((pInvestorAccount->Available),'f',1)));
	ui.MyFunds->setItem(1,2,new QTableWidgetItem(QString::number((pInvestorAccount->CloseProfit),'f',1)));
	ui.MyFunds->setItem(1,3,new QTableWidgetItem(QString::number((pInvestorAccount->PositionProfit),'f',1)));
	//ui.MyFunds->setItem(1,4,new QTableWidgetItem(QString::number((pInvestorAccount->DynamicRights),'f',1)));//��̬Ȩ��	
	ui.MyFunds->setItem(1,4,new QTableWidgetItem(QString::number((pInvestorAccount->Margin),'f',1)));//��֤��
	ui.MyFunds->setItem(1,5,new QTableWidgetItem(QString::number((pInvestorAccount->Fee),'f',1)));//������
	ui.MyFunds->setItem(1,6,new QTableWidgetItem(QString::number((pInvestorAccount->Premium),'f',1)));//Ȩ����
	ui.MyFunds->item(1,0)->setTextAlignment(Qt::AlignCenter);
	ui.MyFunds->item(1,1)->setTextAlignment(Qt::AlignCenter);
	ui.MyFunds->item(1,2)->setTextAlignment(Qt::AlignCenter);
	ui.MyFunds->item(1,3)->setTextAlignment(Qt::AlignCenter);
	ui.MyFunds->item(1,4)->setTextAlignment(Qt::AlignCenter);
	ui.MyFunds->item(1,5)->setTextAlignment(Qt::AlignCenter);
	ui.MyFunds->item(1,6)->setTextAlignment(Qt::AlignCenter);
	//ui.MyFunds->item(1,7)->setTextAlignment(Qt::AlignCenter);
	TradebtnOK=false;//�ر� �Ȳ�ѯ�������ʱ����
	}
	//�ʽ��ѯ
	CUstpFtdcQryInvestorAccountField QryInvestorAcc;
	memset(&QryInvestorAcc,0,sizeof(CUstpFtdcQryInvestorAccountField));
	strcpy(QryInvestorAcc.BrokerID,BROKER_ID);
	pTraderApi2->ReqQryInvestorAccount(&QryInvestorAcc,iRequestID++);
	inAccountTime->start(2000);
	return;
}

//ִ�в����µ�
void Qt_femas::initTrader()
{
	//ִ�в����µ�
	StrategyTask* pStTask = new StrategyTask();
	pStrategyGlobal = pStTask; // pStrategyGlobal is global, can be used by others
	pStTask->runThread(ThreadFunc_St);
	QMessageBox::about(NULL,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("���ڽ��в��Խ��ף�"));
}

//ʵʱ����ί�е���Ϣ
void Qt_femas::updateEntrustOrder()
{
	//����ǰ��ȫ�����
	ui.finish_Order->clearContents();
	ui.entrust_Order->clearContents();
	CMyOrder orderTemp;
	int tableSize = OrderManageSys::orderTable.size();
	for(int i = 0,j=0,k=0;i<tableSize;i++)
	{
		// order status: '0' - ready, '1' - sent, '2' - received, '3' - pending, '4' - traded
		orderTemp = OrderManageSys::orderTable[i];
		//�ж϶�����״̬���ֱ�������
		switch(orderTemp.status)
		{
	    //��ʾ�ڳɽ���setItem(1,6,new QTableWidgetItem(QString::number((pInvestorAccount->FrozenMargin),'f',1)));
		case USTP_FTDC_OS_AllTraded://ȫ���ɽ�
			{
				ui.finish_Order->setItem(j,0,new QTableWidgetItem(orderTemp.tradeID));//�ɽ����
				ui.finish_Order->setItem(j,1,new QTableWidgetItem(orderTemp.instrumentID));//��Լ
				ui.finish_Order->setItem(j,2,new QTableWidgetItem((orderTemp.direction==USTP_FTDC_D_Buy)?(QString::fromLocal8Bit("����")):(QString::fromLocal8Bit("����"))));//����
			    ui.finish_Order->setItem(j,3,new QTableWidgetItem((orderTemp.operation==USTP_FTDC_OF_Open)?(QString::fromLocal8Bit("����")):(QString::fromLocal8Bit("ƽ��"))));//��Լ
				ui.finish_Order->setItem(j,4,new QTableWidgetItem(QString::number(orderTemp.tradePrice,'f',1)));//�ɽ���
				ui.finish_Order->setItem(j,5,new QTableWidgetItem(QString::number(orderTemp.tradedQuan)));//�ɽ�����
				ui.finish_Order->setItem(j,6,new QTableWidgetItem(orderTemp.successTime));//�ɽ�ʱ��
				ui.finish_Order->setItem(j,7,new QTableWidgetItem(QString::fromLocal8Bit("ȫ���ɽ�")));//�ɽ�����
				ui.finish_Order->setItem(j,8,new QTableWidgetItem(QString::number(orderTemp.orderRef)));//�������
				ui.finish_Order->setItem(j,9,new QTableWidgetItem(QString(QLatin1String(orderTemp.insertTime))));//����ʱ��
				ui.finish_Order->setItem(j,10,new QTableWidgetItem("CFFEX"));//������
				j++;//�����ݾͽ��±����
			}
			break;
		case USTP_FTDC_OS_PartTradedNotQueueing://���ֳɽ����ڶ�����
			{
				ui.finish_Order->setItem(j,0,new QTableWidgetItem(orderTemp.tradeID));//�ɽ����
				ui.finish_Order->setItem(j,1,new QTableWidgetItem(orderTemp.instrumentID));//��Լ
				ui.finish_Order->setItem(j,2,new QTableWidgetItem((orderTemp.direction==USTP_FTDC_D_Buy)?(QString::fromLocal8Bit("����")):(QString::fromLocal8Bit("����"))));//����
			    ui.finish_Order->setItem(j,3,new QTableWidgetItem((orderTemp.operation==USTP_FTDC_OF_Open)?(QString::fromLocal8Bit("����")):(QString::fromLocal8Bit("ƽ��"))));//��Լ
				ui.finish_Order->setItem(j,4,new QTableWidgetItem(QString::number(orderTemp.tradePrice,'f',1)));//�ɽ���
				ui.finish_Order->setItem(j,5,new QTableWidgetItem(QString::number(orderTemp.tradedQuan)));//�ɽ�����
				ui.finish_Order->setItem(j,6,new QTableWidgetItem(orderTemp.successTime));//�ɽ�ʱ��
				ui.finish_Order->setItem(j,7,new QTableWidgetItem(QString::fromLocal8Bit("���ֳɽ����ڶ�����")));//�ɽ�����
				ui.finish_Order->setItem(j,8,new QTableWidgetItem(QString::number(orderTemp.orderRef)));//�������
				ui.finish_Order->setItem(j,9,new QTableWidgetItem(QString(QLatin1String(orderTemp.insertTime))));//����ʱ��
				ui.finish_Order->setItem(j,10,new QTableWidgetItem("CFFEX"));//������
				j++;
			}
			break;
		//��ʾ��ί�е�
		case USTP_FTDC_OS_PartTradedQueueing://���ֳɽ����ڶ�����
			{
				ui.entrust_Order->setItem(k,0,new QTableWidgetItem(QString::number(orderTemp.orderRef)));//�������
				ui.entrust_Order->setItem(k,1,new QTableWidgetItem(orderTemp.instrumentID));//��Լ
				ui.entrust_Order->setItem(k,2,new QTableWidgetItem((orderTemp.direction==USTP_FTDC_D_Buy)?(QString::fromLocal8Bit("����")):(QString::fromLocal8Bit("����"))));//����
			    ui.entrust_Order->setItem(k,3,new QTableWidgetItem((orderTemp.operation==USTP_FTDC_OF_Open)?(QString::fromLocal8Bit("����")):(QString::fromLocal8Bit("ƽ��"))));//��Լ
				ui.entrust_Order->setItem(k,4,new QTableWidgetItem(QString::fromLocal8Bit("���ֳɽ����ڶ�����")));//״̬
				ui.entrust_Order->setItem(k,5,new QTableWidgetItem(QString::number(orderTemp.orderPrice,'f',1)));//������
				ui.entrust_Order->setItem(k,6,new QTableWidgetItem(QString::number(orderTemp.quantity-orderTemp.tradedQuan)));//δ�ɽ�����
				ui.entrust_Order->setItem(k,7,new QTableWidgetItem(QString::number(orderTemp.quantity)));//��������
				ui.entrust_Order->setItem(k,8,new QTableWidgetItem(QString::number(orderTemp.tradePrice,'f',1)));//�ɽ�����
				ui.entrust_Order->setItem(k,9,new QTableWidgetItem(QString(QLatin1String(orderTemp.insertTime))));//����ʱ��
				ui.entrust_Order->setItem(k,10,new QTableWidgetItem("CFFEX"));//������
				k++;
			}
			break;
		case USTP_FTDC_OS_NoTradeQueueing://δ�ɽ����ڶ�����
			{
				ui.entrust_Order->setItem(k,0,new QTableWidgetItem(QString::number(orderTemp.orderRef)));//�������
				ui.entrust_Order->setItem(k,1,new QTableWidgetItem(orderTemp.instrumentID));//��Լ
				ui.entrust_Order->setItem(k,2,new QTableWidgetItem((orderTemp.direction==USTP_FTDC_D_Buy)?(QString::fromLocal8Bit("����")):(QString::fromLocal8Bit("����"))));//����
			    ui.entrust_Order->setItem(k,3,new QTableWidgetItem((orderTemp.operation==USTP_FTDC_OF_Open)?(QString::fromLocal8Bit("����")):(QString::fromLocal8Bit("ƽ��"))));//��Լ
				ui.entrust_Order->setItem(k,4,new QTableWidgetItem(QString::fromLocal8Bit("δ�ɽ����ڶ�����")));//״̬
				ui.entrust_Order->setItem(k,5,new QTableWidgetItem(QString::number(orderTemp.orderPrice,'f',1)));//������
				ui.entrust_Order->setItem(k,6,new QTableWidgetItem(QString::number(orderTemp.quantity-orderTemp.tradedQuan)));//δ�ɽ�����
				ui.entrust_Order->setItem(k,7,new QTableWidgetItem(QString::number(orderTemp.quantity)));//��������
				ui.entrust_Order->setItem(k,8,new QTableWidgetItem(QString::number(orderTemp.tradePrice,'f',1)));//�ɽ�����
				ui.entrust_Order->setItem(k,9,new QTableWidgetItem(QString(QLatin1String(orderTemp.insertTime))));//����ʱ��
				ui.entrust_Order->setItem(k,10,new QTableWidgetItem("CFFEX"));//������
				k++;
			}
			break;
		case USTP_FTDC_OS_NoTradeNotQueueing://δ�ɽ����ڶ�����
			{
				ui.entrust_Order->setItem(k,0,new QTableWidgetItem(QString::number(orderTemp.orderRef)));//�������
				ui.entrust_Order->setItem(k,1,new QTableWidgetItem(orderTemp.instrumentID));//��Լ
				ui.entrust_Order->setItem(k,2,new QTableWidgetItem((orderTemp.direction==USTP_FTDC_D_Buy)?(QString::fromLocal8Bit("����")):(QString::fromLocal8Bit("����"))));//����
			    ui.entrust_Order->setItem(k,3,new QTableWidgetItem((orderTemp.operation==USTP_FTDC_OF_Open)?(QString::fromLocal8Bit("����")):(QString::fromLocal8Bit("ƽ��"))));//��Լ
				ui.entrust_Order->setItem(k,4,new QTableWidgetItem(QString::fromLocal8Bit("δ�ɽ����ڶ�����")));//״̬
				ui.entrust_Order->setItem(k,5,new QTableWidgetItem(QString::number(orderTemp.orderPrice,'f',1)));//������
				ui.entrust_Order->setItem(k,6,new QTableWidgetItem(QString::number(orderTemp.quantity-orderTemp.tradedQuan)));//δ�ɽ�����
				ui.entrust_Order->setItem(k,7,new QTableWidgetItem(QString::number(orderTemp.quantity)));//��������
				ui.entrust_Order->setItem(k,8,new QTableWidgetItem(QString::number(orderTemp.tradePrice,'f',1)));//�ɽ�����
				ui.entrust_Order->setItem(k,9,new QTableWidgetItem(QString(QLatin1String(orderTemp.insertTime))));//����ʱ��
				ui.entrust_Order->setItem(k,10,new QTableWidgetItem("CFFEX"));//������
				k++;
			}
			break;
		case USTP_FTDC_OS_Canceled://����
			{
				ui.entrust_Order->setItem(k,0,new QTableWidgetItem(QString::number(orderTemp.orderRef)));//�������
				ui.entrust_Order->setItem(k,1,new QTableWidgetItem(orderTemp.instrumentID));//��Լ
				ui.entrust_Order->setItem(k,2,new QTableWidgetItem((orderTemp.direction==USTP_FTDC_D_Buy)?(QString::fromLocal8Bit("����")):(QString::fromLocal8Bit("����"))));//����
			    ui.entrust_Order->setItem(k,3,new QTableWidgetItem((orderTemp.operation==USTP_FTDC_OF_Open)?(QString::fromLocal8Bit("����")):(QString::fromLocal8Bit("ƽ��"))));//��Լ
				ui.entrust_Order->setItem(k,4,new QTableWidgetItem(QString::fromLocal8Bit("��������")));//״̬
				ui.entrust_Order->setItem(k,5,new QTableWidgetItem(QString::number(orderTemp.orderPrice,'f',1)));//������
				ui.entrust_Order->setItem(k,6,new QTableWidgetItem(QString::number(orderTemp.quantity-orderTemp.tradedQuan)));//δ�ɽ�����
				ui.entrust_Order->setItem(k,7,new QTableWidgetItem(QString::number(orderTemp.quantity)));//��������
				ui.entrust_Order->setItem(k,8,new QTableWidgetItem(QString::number(orderTemp.tradePrice,'f',1)));//�ɽ�����
				ui.entrust_Order->setItem(k,9,new QTableWidgetItem(QString(QLatin1String(orderTemp.insertTime))));//����ʱ��
				ui.entrust_Order->setItem(k,10,new QTableWidgetItem("CFFEX"));//������
				k++;
			}
			break;
		case USTP_FTDC_OS_AcceptedNoReply://�����ѱ��뽻����δӦ��
			{
				ui.entrust_Order->setItem(k,0,new QTableWidgetItem(QString::number(orderTemp.orderRef)));//�������
				ui.entrust_Order->setItem(k,1,new QTableWidgetItem(orderTemp.instrumentID));//��Լ
				ui.entrust_Order->setItem(k,2,new QTableWidgetItem((orderTemp.direction==USTP_FTDC_D_Buy)?(QString::fromLocal8Bit("����")):(QString::fromLocal8Bit("����"))));//����
			    ui.entrust_Order->setItem(k,3,new QTableWidgetItem((orderTemp.operation==USTP_FTDC_OF_Open)?(QString::fromLocal8Bit("����")):(QString::fromLocal8Bit("ƽ��"))));//��Լ
				ui.entrust_Order->setItem(k,4,new QTableWidgetItem(QString::fromLocal8Bit("�ѱ��� ������δӦ��")));//״̬
				ui.entrust_Order->setItem(k,5,new QTableWidgetItem(QString::number(orderTemp.orderPrice,'f',1)));//������
				ui.entrust_Order->setItem(k,6,new QTableWidgetItem(QString::number(orderTemp.quantity-orderTemp.tradedQuan)));//δ�ɽ�����
				ui.entrust_Order->setItem(k,7,new QTableWidgetItem(QString::number(orderTemp.quantity)));//��������
				ui.entrust_Order->setItem(k,8,new QTableWidgetItem(QString::number(orderTemp.tradePrice,'f',1)));//�ɽ�����
				ui.entrust_Order->setItem(k,9,new QTableWidgetItem(QString(QLatin1String(orderTemp.insertTime))));//����ʱ��
				ui.entrust_Order->setItem(k,10,new QTableWidgetItem("CFFEX"));//������
				k++;
			}
			break;
		default:;
			break;
		}
	}
}

//ʵʱ���³ֲ���Ϣ
void Qt_femas::updatePositionTable()
{
	//����ǰ ��ȫ�����
	ui.position_detail->clearContents();
	//�������� �ֲ�����
	for(int i = 0,j = 0;i<instrumentNum;i++)
	{
		CMyPosition lposition= OrderManageSys::longPosition[i];
		CMyPosition sposition= OrderManageSys::shortPosition[i];
		if(lposition.volume>0)
		{
		 ui.position_detail->setItem(j,0,new QTableWidgetItem(ppInstrumentID[i]));
		 ui.position_detail->setItem(j,1,new QTableWidgetItem(QString::fromLocal8Bit("���� ����")));
		 ui.position_detail->setItem(j,2,new QTableWidgetItem(QString::number(lposition.volume)));//�ֲ���
		 ui.position_detail->setItem(j,3,new QTableWidgetItem(QString::number((lposition.aveCost),'f',1)));//ƽ���ɱ�
		 ui.position_detail->setItem(j,4,new QTableWidgetItem(QString::number(lposition.availableVolume)));//��ƽ��
		 ui.position_detail->setItem(j,5,new QTableWidgetItem("CFFEX"));//������
		 j++;
		}
		if(sposition.volume>0)
		{
		 ui.position_detail->setItem(j,0,new QTableWidgetItem(ppInstrumentID[i]));
		 ui.position_detail->setItem(j,1,new QTableWidgetItem(QString::fromLocal8Bit("���� ����")));
		 ui.position_detail->setItem(j,2,new QTableWidgetItem(QString::number(sposition.volume)));//�ֲ���
		 ui.position_detail->setItem(j,3,new QTableWidgetItem(QString::number(sposition.aveCost,'f',1)));//ƽ���ɱ�
		 ui.position_detail->setItem(j,4,new QTableWidgetItem(QString::number(sposition.availableVolume)));//��ƽ��
		 ui.position_detail->setItem(j,5,new QTableWidgetItem("CFFEX"));//������
		 j++;
		}
	}
}

//��ѯ�ֲ�
void Qt_femas::QryInvestorPosition()
{
	CUstpFtdcQryInvestorPositionField QryInvestorPosition;
	memset(&QryInvestorPosition,0,sizeof(CUstpFtdcQryInvestorPositionField));
	strcpy(QryInvestorPosition.ExchangeID,"CFFEX");
	strcpy(QryInvestorPosition.BrokerID,BROKER_ID);
	strcpy(QryInvestorPosition.InvestorID,INVESTOR_ID);
	pTraderApi2->ReqQryInvestorPosition(&QryInvestorPosition,iRequestID++);
}
