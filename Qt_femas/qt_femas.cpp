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

	//自适应屏幕大小
	QDesktopWidget *desktop = QApplication::desktop();
    int screenWidth = desktop->screenGeometry().width();
    int screenHeight = desktop->screenGeometry().height();
	ui.widgetLA->resize(screenWidth,screenHeight);
	ui.widgetLB->resize(screenWidth,screenHeight);
	
	//设置currentindex为持仓明细显示
	ui.myOrder->setCurrentIndex(2);
	//合并单元格
	ui.MyFunds->setSpan(0,8,2,1);
	ui.MyFunds->setSpan(0,10,2,1);
	//添加按钮 "显示行情"
	QWidget *widgetPush = new QWidget(ui.MyFunds);
	QHBoxLayout *hLayout = new QHBoxLayout();
	QPushButton *pushButton_test = new QPushButton(QString::fromLocal8Bit("显示行情")); 
	pushButton_test->resize(5,6);
	hLayout->addWidget(pushButton_test);
	hLayout->setMargin(0);
	hLayout->setAlignment(pushButton_test,Qt::AlignCenter);
	widgetPush->setLayout(hLayout);
	ui.MyFunds->setCellWidget(0, 8, widgetPush); 
	
	//添加按钮 “获取资金，持仓 等开始交易”
	QWidget *widgetPush1 = new QWidget(ui.MyFunds);
	QHBoxLayout *hLayout1 = new QHBoxLayout();
	QPushButton *pushButton_test1 = new QPushButton(QString::fromLocal8Bit("策略交易")); 
	pushButton_test1->resize(9,10);
	hLayout1->addWidget(pushButton_test1);
	hLayout1->setMargin(0);
	hLayout1->setAlignment(pushButton_test1,Qt::AlignCenter);
	widgetPush1->setLayout(hLayout1);
	ui.MyFunds->setCellWidget(0, 10, widgetPush1); 

	//定时更新行情数据的线程mdThread启动
	mdThread = new QThread;
	mdTime = new QTimer;
	mdTime->setSingleShot(true);
	mdTime->start(100);
	mdTime->moveToThread(mdThread);
	inAccountTime = new QTimer;
	inAccountTime->start(100);
	inAccountTime->moveToThread(mdThread);
	/*
	* 定时器绑定 跟新行情 +　更新委托单显示 + 持仓显示
	*/
	connect(mdTime,SIGNAL(timeout()),this,SLOT(updateMD()),Qt::DirectConnection);
	connect(mdTime,SIGNAL(timeout()),this,SLOT(updateEntrustOrder()),Qt::DirectConnection);
	connect(mdTime,SIGNAL(timeout()),this,SLOT(updatePositionTable()),Qt::DirectConnection);
	connect(inAccountTime,SIGNAL(timeout()),this,SLOT(initInvestorAccount()),Qt::DirectConnection);

	connect(pushButton_test,SIGNAL(clicked()),this,SLOT(connectMduser()));//行情显示
	connect(pushButton_test1,SIGNAL(clicked()),this,SLOT(initTrader()));//交易显示
	connect(pushButton_test,SIGNAL(clicked()),this,SLOT(QryInvestorPosition()));//登录时持仓查询
	//下单
	connect(ui.btnPlaceOrder,SIGNAL(clicked()),this,SLOT(onPlaceOrder()));
	connect(ui.mduserTable,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(onMdSelect(int,int)));

	//初始化下单的界面
	ui.newOrder->setEditable(true);
	for(int j=0;j<instrumentNum;j++)
	{
		ui.newOrder->addItem(ppInstrumentID[j]);
	}
	//按钮分组初始化，设置ID
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

	//设置表格行数
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
	ui.mduserTable->setItem(i,0,new QTableWidgetItem(ppInstrumentID[i]));//合约代码
	ui.mduserTable->setItem(i,1,new QTableWidgetItem(QString::number((pdata->LastPrice),'f',1)));//最新价
	ui.mduserTable->setItem(i,2,new QTableWidgetItem(QString::number((pdata->BidPrice1),'f',1)));//申买价
	ui.mduserTable->setItem(i,3,new QTableWidgetItem(QString::number(pdata->BidVolume1)));//买量
	ui.mduserTable->setItem(i,4,new QTableWidgetItem(QString::number((pdata->AskPrice1),'f',1)));//申卖价
	ui.mduserTable->setItem(i,5,new QTableWidgetItem(QString::number(pdata->AskVolume1)));//申卖量
	ui.mduserTable->setItem(i,6,new QTableWidgetItem(QString::number(pdata->Volume)));//成交量
	ui.mduserTable->setItem(i,7,new QTableWidgetItem(QString::number(pdata->OpenInterest)));//持仓量
	ui.mduserTable->setItem(i,8,new QTableWidgetItem(QString::number((pdata->UpperLimitPrice),'f',1)));//涨停板价
	ui.mduserTable->setItem(i,9,new QTableWidgetItem(QString::number((pdata->LowerLimitPrice),'f',1)));//跌停板价
	ui.mduserTable->setItem(i,10,new QTableWidgetItem(QString::number((pdata->OpenPrice),'f',1)));//今开盘
	ui.mduserTable->setItem(i,11,new QTableWidgetItem(QString::number((pdata->PreClosePrice),'f',1)));//昨开盘
	ui.mduserTable->setItem(i,12,new QTableWidgetItem(QString::number((pdata->HighestPrice),'f',1)));//最高价
	ui.mduserTable->setItem(i,13,new QTableWidgetItem(QString::number((pdata->LowestPrice),'f',1)));//最低价
	ui.mduserTable->setItem(i,14,new QTableWidgetItem(QString::number((pdata->PreSettlementPrice),'f',1)));//昨结算价
	ui.mduserTable->setItem(i,15,new QTableWidgetItem(QString::number((pdata->Turnover),'f',1)));//成交金额
	ui.mduserTable->setItem(i,16,new QTableWidgetItem(pdata->UpdateTime));//最后修改时间
	}
	mdTime->start(500);
}

//双击选择行情行获得对应合约 下单信息
void Qt_femas::onMdSelect(int row,int column)
{
	ui.newOrder->setCurrentText(ui.mduserTable->item(row,0)->text());
	//让指定价的QSpinbox显示最新价
	ui.order_price->setValue((ui.mduserTable->item(row,1)->text()).toDouble());
	ui.trader_hand->setValue(1);
}

//重置订单界面信息
void Qt_femas::onResetOrder()
{
	ui.order_price->setValue(0.0);
	ui.trader_hand->setValue(1);
}

//下订单操作
void Qt_femas::onPlaceOrder()
{
	string str = ui.newOrder->currentText().toStdString();
	/*
	* 判断输入的是否为可交易合约
	*/
	if(str.empty())
	{
		QMessageBox::about(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请输入要交易的合约代码"));
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
	  QMessageBox::about(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请输入或选择合法的合约"));
	  return;
	}

	//获取买卖方向、开平仓
	char long_short = USTP_FTDC_D_Buy;
	char entry_exit = USTP_FTDC_OF_Open;
	QString str1,str2;
	switch(btnBuy->checkedId())
	{
	case 0:
		long_short = USTP_FTDC_D_Buy;//买入
		str1 = QString::fromLocal8Bit("买入");
		break;
	case 1:
		long_short = USTP_FTDC_D_Sell;//卖出
		str1 = QString::fromLocal8Bit("卖出");
		break;
	default:break;
	}

	switch(btnKaiping->checkedId())
	{
	case 0://平仓
		entry_exit = USTP_FTDC_OF_Open;
		str2 = QString::fromLocal8Bit("开仓");
		break;
	case 1:
		entry_exit = USTP_FTDC_OF_Close;
		str2 = QString::fromLocal8Bit("平仓");
		break;
	default:break;
	}

	char* instrumentID = ppInstrumentID[LocalMemory::instrumentMap[str]];//问一下这句为啥需要

	//获得下单手数和价格
	int volume = ui.trader_hand->value();
	double price = ui.order_price->value();
	//判断选择的合约是数组里的哪一个，对应行情界面取其价格限制
	int index_t = LocalMemory::instrumentMap[str];
	if(price>(ui.mduserTable->item(index_t,8)->text()).toDouble())
	     {
				 QMessageBox::about(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("报价超过涨停板价！"));
				 return;
		  }
			else if(price<(ui.mduserTable->item(index_t,9)->text()).toDouble())
		 {
				QMessageBox::about(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("报价低于跌停板价！"));
				return;
		 }
	
	QString orderMessage = QString::fromLocal8Bit("下单：")+str2+str1+QString::fromStdString(str)+QString("  ")+ QString::number(volume)+QString::fromLocal8Bit(" 手 于价格 ")+QString::number(price,'f',1);
	switch(QMessageBox::question(NULL,QString::fromLocal8Bit("确认订单"),orderMessage,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
	{
	case QMessageBox::Yes:
	{
		if(btnPrice->checkedId()==0)//下限价单
		{
		OrderManageSys::MyOrderInsertLimitPrice(long_short, entry_exit, instrumentID, volume, price);
		// 加入orderTable
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
			if(btnPrice->checkedId()==1)//下市价单
		{
		OrderManageSys::MyOrderInsertLimitPrice(long_short, entry_exit, instrumentID, volume, price);
		// 加入orderTable
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
		//QMessageBox::about(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("下单成功！"));
		break;
	}
	case QMessageBox::No:break;
	default:break;
	}
}

//实时更新资金状况
void Qt_femas::initInvestorAccount()
{
	if(TradebtnOK){
		//测试设置用户资金的数据
	ui.MyFunds->setItem(1,0,new QTableWidgetItem(QString(pInvestorAccount->InvestorID)));
	ui.MyFunds->setItem(1,1,new QTableWidgetItem(QString::number((pInvestorAccount->Available),'f',1)));
	ui.MyFunds->setItem(1,2,new QTableWidgetItem(QString::number((pInvestorAccount->CloseProfit),'f',1)));
	ui.MyFunds->setItem(1,3,new QTableWidgetItem(QString::number((pInvestorAccount->PositionProfit),'f',1)));
	//ui.MyFunds->setItem(1,4,new QTableWidgetItem(QString::number((pInvestorAccount->DynamicRights),'f',1)));//动态权益	
	ui.MyFunds->setItem(1,4,new QTableWidgetItem(QString::number((pInvestorAccount->Margin),'f',1)));//保证金
	ui.MyFunds->setItem(1,5,new QTableWidgetItem(QString::number((pInvestorAccount->Fee),'f',1)));//手续费
	ui.MyFunds->setItem(1,6,new QTableWidgetItem(QString::number((pInvestorAccount->Premium),'f',1)));//权利金
	ui.MyFunds->item(1,0)->setTextAlignment(Qt::AlignCenter);
	ui.MyFunds->item(1,1)->setTextAlignment(Qt::AlignCenter);
	ui.MyFunds->item(1,2)->setTextAlignment(Qt::AlignCenter);
	ui.MyFunds->item(1,3)->setTextAlignment(Qt::AlignCenter);
	ui.MyFunds->item(1,4)->setTextAlignment(Qt::AlignCenter);
	ui.MyFunds->item(1,5)->setTextAlignment(Qt::AlignCenter);
	ui.MyFunds->item(1,6)->setTextAlignment(Qt::AlignCenter);
	//ui.MyFunds->item(1,7)->setTextAlignment(Qt::AlignCenter);
	TradebtnOK=false;//关闭 等查询结果返回时开启
	}
	//资金查询
	CUstpFtdcQryInvestorAccountField QryInvestorAcc;
	memset(&QryInvestorAcc,0,sizeof(CUstpFtdcQryInvestorAccountField));
	strcpy(QryInvestorAcc.BrokerID,BROKER_ID);
	pTraderApi2->ReqQryInvestorAccount(&QryInvestorAcc,iRequestID++);
	inAccountTime->start(2000);
	return;
}

//执行策略下单
void Qt_femas::initTrader()
{
	//执行策略下单
	StrategyTask* pStTask = new StrategyTask();
	pStrategyGlobal = pStTask; // pStrategyGlobal is global, can be used by others
	pStTask->runThread(ThreadFunc_St);
	QMessageBox::about(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("正在进行策略交易！"));
}

//实时更新委托单信息
void Qt_femas::updateEntrustOrder()
{
	//更新前先全部清空
	ui.finish_Order->clearContents();
	ui.entrust_Order->clearContents();
	CMyOrder orderTemp;
	int tableSize = OrderManageSys::orderTable.size();
	for(int i = 0,j=0,k=0;i<tableSize;i++)
	{
		// order status: '0' - ready, '1' - sent, '2' - received, '3' - pending, '4' - traded
		orderTemp = OrderManageSys::orderTable[i];
		//判断订单的状态，分别做处理
		switch(orderTemp.status)
		{
	    //显示于成交单setItem(1,6,new QTableWidgetItem(QString::number((pInvestorAccount->FrozenMargin),'f',1)));
		case USTP_FTDC_OS_AllTraded://全部成交
			{
				ui.finish_Order->setItem(j,0,new QTableWidgetItem(orderTemp.tradeID));//成交编号
				ui.finish_Order->setItem(j,1,new QTableWidgetItem(orderTemp.instrumentID));//合约
				ui.finish_Order->setItem(j,2,new QTableWidgetItem((orderTemp.direction==USTP_FTDC_D_Buy)?(QString::fromLocal8Bit("买入")):(QString::fromLocal8Bit("卖出"))));//买卖
			    ui.finish_Order->setItem(j,3,new QTableWidgetItem((orderTemp.operation==USTP_FTDC_OF_Open)?(QString::fromLocal8Bit("开仓")):(QString::fromLocal8Bit("平仓"))));//合约
				ui.finish_Order->setItem(j,4,new QTableWidgetItem(QString::number(orderTemp.tradePrice,'f',1)));//成交价
				ui.finish_Order->setItem(j,5,new QTableWidgetItem(QString::number(orderTemp.tradedQuan)));//成交手数
				ui.finish_Order->setItem(j,6,new QTableWidgetItem(orderTemp.successTime));//成交时间
				ui.finish_Order->setItem(j,7,new QTableWidgetItem(QString::fromLocal8Bit("全部成交")));//成交类型
				ui.finish_Order->setItem(j,8,new QTableWidgetItem(QString::number(orderTemp.orderRef)));//报单编号
				ui.finish_Order->setItem(j,9,new QTableWidgetItem(QString(QLatin1String(orderTemp.insertTime))));//报单时间
				ui.finish_Order->setItem(j,10,new QTableWidgetItem("CFFEX"));//交易所
				j++;//有数据就将下标递增
			}
			break;
		case USTP_FTDC_OS_PartTradedNotQueueing://部分成交不在队列中
			{
				ui.finish_Order->setItem(j,0,new QTableWidgetItem(orderTemp.tradeID));//成交编号
				ui.finish_Order->setItem(j,1,new QTableWidgetItem(orderTemp.instrumentID));//合约
				ui.finish_Order->setItem(j,2,new QTableWidgetItem((orderTemp.direction==USTP_FTDC_D_Buy)?(QString::fromLocal8Bit("买入")):(QString::fromLocal8Bit("卖出"))));//买卖
			    ui.finish_Order->setItem(j,3,new QTableWidgetItem((orderTemp.operation==USTP_FTDC_OF_Open)?(QString::fromLocal8Bit("开仓")):(QString::fromLocal8Bit("平仓"))));//合约
				ui.finish_Order->setItem(j,4,new QTableWidgetItem(QString::number(orderTemp.tradePrice,'f',1)));//成交价
				ui.finish_Order->setItem(j,5,new QTableWidgetItem(QString::number(orderTemp.tradedQuan)));//成交手数
				ui.finish_Order->setItem(j,6,new QTableWidgetItem(orderTemp.successTime));//成交时间
				ui.finish_Order->setItem(j,7,new QTableWidgetItem(QString::fromLocal8Bit("部分成交不在队列中")));//成交类型
				ui.finish_Order->setItem(j,8,new QTableWidgetItem(QString::number(orderTemp.orderRef)));//报单编号
				ui.finish_Order->setItem(j,9,new QTableWidgetItem(QString(QLatin1String(orderTemp.insertTime))));//报单时间
				ui.finish_Order->setItem(j,10,new QTableWidgetItem("CFFEX"));//交易所
				j++;
			}
			break;
		//显示于委托单
		case USTP_FTDC_OS_PartTradedQueueing://部分成交还在队列中
			{
				ui.entrust_Order->setItem(k,0,new QTableWidgetItem(QString::number(orderTemp.orderRef)));//报单编号
				ui.entrust_Order->setItem(k,1,new QTableWidgetItem(orderTemp.instrumentID));//合约
				ui.entrust_Order->setItem(k,2,new QTableWidgetItem((orderTemp.direction==USTP_FTDC_D_Buy)?(QString::fromLocal8Bit("买入")):(QString::fromLocal8Bit("卖出"))));//买卖
			    ui.entrust_Order->setItem(k,3,new QTableWidgetItem((orderTemp.operation==USTP_FTDC_OF_Open)?(QString::fromLocal8Bit("开仓")):(QString::fromLocal8Bit("平仓"))));//合约
				ui.entrust_Order->setItem(k,4,new QTableWidgetItem(QString::fromLocal8Bit("部分成交还在队列中")));//状态
				ui.entrust_Order->setItem(k,5,new QTableWidgetItem(QString::number(orderTemp.orderPrice,'f',1)));//报单价
				ui.entrust_Order->setItem(k,6,new QTableWidgetItem(QString::number(orderTemp.quantity-orderTemp.tradedQuan)));//未成交手数
				ui.entrust_Order->setItem(k,7,new QTableWidgetItem(QString::number(orderTemp.quantity)));//报单手数
				ui.entrust_Order->setItem(k,8,new QTableWidgetItem(QString::number(orderTemp.tradePrice,'f',1)));//成交均价
				ui.entrust_Order->setItem(k,9,new QTableWidgetItem(QString(QLatin1String(orderTemp.insertTime))));//报单时间
				ui.entrust_Order->setItem(k,10,new QTableWidgetItem("CFFEX"));//交易所
				k++;
			}
			break;
		case USTP_FTDC_OS_NoTradeQueueing://未成交还在队列中
			{
				ui.entrust_Order->setItem(k,0,new QTableWidgetItem(QString::number(orderTemp.orderRef)));//报单编号
				ui.entrust_Order->setItem(k,1,new QTableWidgetItem(orderTemp.instrumentID));//合约
				ui.entrust_Order->setItem(k,2,new QTableWidgetItem((orderTemp.direction==USTP_FTDC_D_Buy)?(QString::fromLocal8Bit("买入")):(QString::fromLocal8Bit("卖出"))));//买卖
			    ui.entrust_Order->setItem(k,3,new QTableWidgetItem((orderTemp.operation==USTP_FTDC_OF_Open)?(QString::fromLocal8Bit("开仓")):(QString::fromLocal8Bit("平仓"))));//合约
				ui.entrust_Order->setItem(k,4,new QTableWidgetItem(QString::fromLocal8Bit("未成交还在队列中")));//状态
				ui.entrust_Order->setItem(k,5,new QTableWidgetItem(QString::number(orderTemp.orderPrice,'f',1)));//报单价
				ui.entrust_Order->setItem(k,6,new QTableWidgetItem(QString::number(orderTemp.quantity-orderTemp.tradedQuan)));//未成交手数
				ui.entrust_Order->setItem(k,7,new QTableWidgetItem(QString::number(orderTemp.quantity)));//报单手数
				ui.entrust_Order->setItem(k,8,new QTableWidgetItem(QString::number(orderTemp.tradePrice,'f',1)));//成交均价
				ui.entrust_Order->setItem(k,9,new QTableWidgetItem(QString(QLatin1String(orderTemp.insertTime))));//报单时间
				ui.entrust_Order->setItem(k,10,new QTableWidgetItem("CFFEX"));//交易所
				k++;
			}
			break;
		case USTP_FTDC_OS_NoTradeNotQueueing://未成交不在队列中
			{
				ui.entrust_Order->setItem(k,0,new QTableWidgetItem(QString::number(orderTemp.orderRef)));//报单编号
				ui.entrust_Order->setItem(k,1,new QTableWidgetItem(orderTemp.instrumentID));//合约
				ui.entrust_Order->setItem(k,2,new QTableWidgetItem((orderTemp.direction==USTP_FTDC_D_Buy)?(QString::fromLocal8Bit("买入")):(QString::fromLocal8Bit("卖出"))));//买卖
			    ui.entrust_Order->setItem(k,3,new QTableWidgetItem((orderTemp.operation==USTP_FTDC_OF_Open)?(QString::fromLocal8Bit("开仓")):(QString::fromLocal8Bit("平仓"))));//合约
				ui.entrust_Order->setItem(k,4,new QTableWidgetItem(QString::fromLocal8Bit("未成交不在队列中")));//状态
				ui.entrust_Order->setItem(k,5,new QTableWidgetItem(QString::number(orderTemp.orderPrice,'f',1)));//报单价
				ui.entrust_Order->setItem(k,6,new QTableWidgetItem(QString::number(orderTemp.quantity-orderTemp.tradedQuan)));//未成交手数
				ui.entrust_Order->setItem(k,7,new QTableWidgetItem(QString::number(orderTemp.quantity)));//报单手数
				ui.entrust_Order->setItem(k,8,new QTableWidgetItem(QString::number(orderTemp.tradePrice,'f',1)));//成交均价
				ui.entrust_Order->setItem(k,9,new QTableWidgetItem(QString(QLatin1String(orderTemp.insertTime))));//报单时间
				ui.entrust_Order->setItem(k,10,new QTableWidgetItem("CFFEX"));//交易所
				k++;
			}
			break;
		case USTP_FTDC_OS_Canceled://撤单
			{
				ui.entrust_Order->setItem(k,0,new QTableWidgetItem(QString::number(orderTemp.orderRef)));//报单编号
				ui.entrust_Order->setItem(k,1,new QTableWidgetItem(orderTemp.instrumentID));//合约
				ui.entrust_Order->setItem(k,2,new QTableWidgetItem((orderTemp.direction==USTP_FTDC_D_Buy)?(QString::fromLocal8Bit("买入")):(QString::fromLocal8Bit("卖出"))));//买卖
			    ui.entrust_Order->setItem(k,3,new QTableWidgetItem((orderTemp.operation==USTP_FTDC_OF_Open)?(QString::fromLocal8Bit("开仓")):(QString::fromLocal8Bit("平仓"))));//合约
				ui.entrust_Order->setItem(k,4,new QTableWidgetItem(QString::fromLocal8Bit("订单撤销")));//状态
				ui.entrust_Order->setItem(k,5,new QTableWidgetItem(QString::number(orderTemp.orderPrice,'f',1)));//报单价
				ui.entrust_Order->setItem(k,6,new QTableWidgetItem(QString::number(orderTemp.quantity-orderTemp.tradedQuan)));//未成交手数
				ui.entrust_Order->setItem(k,7,new QTableWidgetItem(QString::number(orderTemp.quantity)));//报单手数
				ui.entrust_Order->setItem(k,8,new QTableWidgetItem(QString::number(orderTemp.tradePrice,'f',1)));//成交均价
				ui.entrust_Order->setItem(k,9,new QTableWidgetItem(QString(QLatin1String(orderTemp.insertTime))));//报单时间
				ui.entrust_Order->setItem(k,10,new QTableWidgetItem("CFFEX"));//交易所
				k++;
			}
			break;
		case USTP_FTDC_OS_AcceptedNoReply://订单已报入交易所未应答
			{
				ui.entrust_Order->setItem(k,0,new QTableWidgetItem(QString::number(orderTemp.orderRef)));//报单编号
				ui.entrust_Order->setItem(k,1,new QTableWidgetItem(orderTemp.instrumentID));//合约
				ui.entrust_Order->setItem(k,2,new QTableWidgetItem((orderTemp.direction==USTP_FTDC_D_Buy)?(QString::fromLocal8Bit("买入")):(QString::fromLocal8Bit("卖出"))));//买卖
			    ui.entrust_Order->setItem(k,3,new QTableWidgetItem((orderTemp.operation==USTP_FTDC_OF_Open)?(QString::fromLocal8Bit("开仓")):(QString::fromLocal8Bit("平仓"))));//合约
				ui.entrust_Order->setItem(k,4,new QTableWidgetItem(QString::fromLocal8Bit("已报入 交易所未应答")));//状态
				ui.entrust_Order->setItem(k,5,new QTableWidgetItem(QString::number(orderTemp.orderPrice,'f',1)));//报单价
				ui.entrust_Order->setItem(k,6,new QTableWidgetItem(QString::number(orderTemp.quantity-orderTemp.tradedQuan)));//未成交手数
				ui.entrust_Order->setItem(k,7,new QTableWidgetItem(QString::number(orderTemp.quantity)));//报单手数
				ui.entrust_Order->setItem(k,8,new QTableWidgetItem(QString::number(orderTemp.tradePrice,'f',1)));//成交均价
				ui.entrust_Order->setItem(k,9,new QTableWidgetItem(QString(QLatin1String(orderTemp.insertTime))));//报单时间
				ui.entrust_Order->setItem(k,10,new QTableWidgetItem("CFFEX"));//交易所
				k++;
			}
			break;
		default:;
			break;
		}
	}
}

//实时更新持仓信息
void Qt_femas::updatePositionTable()
{
	//更新前 先全部清空
	ui.position_detail->clearContents();
	//遍历买卖 持仓数组
	for(int i = 0,j = 0;i<instrumentNum;i++)
	{
		CMyPosition lposition= OrderManageSys::longPosition[i];
		CMyPosition sposition= OrderManageSys::shortPosition[i];
		if(lposition.volume>0)
		{
		 ui.position_detail->setItem(j,0,new QTableWidgetItem(ppInstrumentID[i]));
		 ui.position_detail->setItem(j,1,new QTableWidgetItem(QString::fromLocal8Bit("买入 做多")));
		 ui.position_detail->setItem(j,2,new QTableWidgetItem(QString::number(lposition.volume)));//持仓量
		 ui.position_detail->setItem(j,3,new QTableWidgetItem(QString::number((lposition.aveCost),'f',1)));//平均成本
		 ui.position_detail->setItem(j,4,new QTableWidgetItem(QString::number(lposition.availableVolume)));//可平量
		 ui.position_detail->setItem(j,5,new QTableWidgetItem("CFFEX"));//交易所
		 j++;
		}
		if(sposition.volume>0)
		{
		 ui.position_detail->setItem(j,0,new QTableWidgetItem(ppInstrumentID[i]));
		 ui.position_detail->setItem(j,1,new QTableWidgetItem(QString::fromLocal8Bit("卖出 做空")));
		 ui.position_detail->setItem(j,2,new QTableWidgetItem(QString::number(sposition.volume)));//持仓量
		 ui.position_detail->setItem(j,3,new QTableWidgetItem(QString::number(sposition.aveCost,'f',1)));//平均成本
		 ui.position_detail->setItem(j,4,new QTableWidgetItem(QString::number(sposition.availableVolume)));//可平量
		 ui.position_detail->setItem(j,5,new QTableWidgetItem("CFFEX"));//交易所
		 j++;
		}
	}
}

//查询持仓
void Qt_femas::QryInvestorPosition()
{
	CUstpFtdcQryInvestorPositionField QryInvestorPosition;
	memset(&QryInvestorPosition,0,sizeof(CUstpFtdcQryInvestorPositionField));
	strcpy(QryInvestorPosition.ExchangeID,"CFFEX");
	strcpy(QryInvestorPosition.BrokerID,BROKER_ID);
	strcpy(QryInvestorPosition.InvestorID,INVESTOR_ID);
	pTraderApi2->ReqQryInvestorPosition(&QryInvestorPosition,iRequestID++);
}
