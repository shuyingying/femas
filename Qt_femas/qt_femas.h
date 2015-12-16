#ifndef QT_FEMAS_H
#define QT_FEMAS_H

#include <QtWidgets/QMainWindow>
#include "ui_qt_femas.h"
#include <qthread.h>
#include <qtimer.h>

class Qt_femas : public QMainWindow
{
	Q_OBJECT

public:
	Qt_femas(QWidget *parent = 0);
	~Qt_femas();
	
	QButtonGroup *btnBuy;//买入卖出按钮组
	QButtonGroup *btnKaiping;//开仓平仓按钮组
	QButtonGroup *btnPrice;//限价市价按钮组

	QThread *mdThread;//定时更新行情页面的线程
    QTimer *mdTime;
	QTimer *inAccountTime;
	//static int order_Index;
signals:
	//void placeOrder(char* orderID);
	
public slots:
		void updateMD();
		void onMdSelect(int,int);//选中行情显示的一行
		void onResetOrder(void);//下单 重置
		void onPlaceOrder(void);//确认下单
		void connectMduser(void);
		void initTrader(void);
		void initInvestorAccount(void);
		void updateEntrustOrder(void);
		void updatePositionTable(void);
		void QryInvestorPosition(void);
		 
private:
	Ui::Qt_femasClass ui;
};

#endif // QT_FEMAS_H
