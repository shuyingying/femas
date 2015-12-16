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
	
	QButtonGroup *btnBuy;//����������ť��
	QButtonGroup *btnKaiping;//����ƽ�ְ�ť��
	QButtonGroup *btnPrice;//�޼��м۰�ť��

	QThread *mdThread;//��ʱ��������ҳ����߳�
    QTimer *mdTime;
	QTimer *inAccountTime;
	//static int order_Index;
signals:
	//void placeOrder(char* orderID);
	
public slots:
		void updateMD();
		void onMdSelect(int,int);//ѡ��������ʾ��һ��
		void onResetOrder(void);//�µ� ����
		void onPlaceOrder(void);//ȷ���µ�
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
