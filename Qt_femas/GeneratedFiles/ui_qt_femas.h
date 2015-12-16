/********************************************************************************
** Form generated from reading UI file 'qt_femas.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT_FEMAS_H
#define UI_QT_FEMAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Qt_femasClass
{
public:
    QAction *actionK;
    QAction *action;
    QAction *actionGuanyu;
    QAction *actionTuichu;
    QAction *action_2;
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_4;
    QWidget *widgetLA;
    QFormLayout *formLayout;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *MyFunds;
    QTableWidget *mduserTable;
    QWidget *widgetLB;
    QGridLayout *gridLayout;
    QGroupBox *placeOrder;
    QLabel *label;
    QComboBox *newOrder;
    QRadioButton *order_buy;
    QRadioButton *order_sale;
    QRadioButton *kaicang;
    QRadioButton *pingcang;
    QLabel *shoushu;
    QSpinBox *trader_hand;
    QLabel *zhidingjia;
    QDoubleSpinBox *order_price;
    QLabel *kaiping;
    QPushButton *btnPlaceOrder;
    QPushButton *reset;
    QRadioButton *limitprice;
    QLabel *kaiping_2;
    QRadioButton *anyprice;
    QTabWidget *myOrder;
    QWidget *entrustOrder;
    QTableWidget *entrust_Order;
    QWidget *finishOrder;
    QTableWidget *finish_Order;
    QWidget *position;
    QTableWidget *position_detail;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QToolBar *mainToolBar;
    QToolBar *toolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Qt_femasClass)
    {
        if (Qt_femasClass->objectName().isEmpty())
            Qt_femasClass->setObjectName(QStringLiteral("Qt_femasClass"));
        Qt_femasClass->resize(1282, 707);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/ femasImage"), QSize(), QIcon::Normal, QIcon::Off);
        Qt_femasClass->setWindowIcon(icon);
        actionK = new QAction(Qt_femasClass);
        actionK->setObjectName(QStringLiteral("actionK"));
        action = new QAction(Qt_femasClass);
        action->setObjectName(QStringLiteral("action"));
        actionGuanyu = new QAction(Qt_femasClass);
        actionGuanyu->setObjectName(QStringLiteral("actionGuanyu"));
        actionTuichu = new QAction(Qt_femasClass);
        actionTuichu->setObjectName(QStringLiteral("actionTuichu"));
        action_2 = new QAction(Qt_femasClass);
        action_2->setObjectName(QStringLiteral("action_2"));
        centralWidget = new QWidget(Qt_femasClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 2, 2));
        verticalLayout_4 = new QVBoxLayout(layoutWidget);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        widgetLA = new QWidget(centralWidget);
        widgetLA->setObjectName(QStringLiteral("widgetLA"));
        widgetLA->setGeometry(QRect(0, 0, 1280, 1000));
        sizePolicy.setHeightForWidth(widgetLA->sizePolicy().hasHeightForWidth());
        widgetLA->setSizePolicy(sizePolicy);
        formLayout = new QFormLayout(widgetLA);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setSizeConstraint(QLayout::SetNoConstraint);
        formLayout->setHorizontalSpacing(0);
        formLayout->setVerticalSpacing(0);
        formLayout->setContentsMargins(2, 2, 2, 2);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));

        formLayout->setLayout(0, QFormLayout::LabelRole, verticalLayout_3);

        MyFunds = new QTableWidget(widgetLA);
        if (MyFunds->columnCount() < 12)
            MyFunds->setColumnCount(12);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        MyFunds->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        MyFunds->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        MyFunds->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        MyFunds->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        MyFunds->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        MyFunds->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        MyFunds->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        MyFunds->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        MyFunds->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        MyFunds->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        MyFunds->setHorizontalHeaderItem(10, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        MyFunds->setHorizontalHeaderItem(11, __qtablewidgetitem11);
        if (MyFunds->rowCount() < 2)
            MyFunds->setRowCount(2);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        __qtablewidgetitem12->setTextAlignment(Qt::AlignCenter);
        MyFunds->setItem(0, 0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        __qtablewidgetitem13->setTextAlignment(Qt::AlignCenter);
        MyFunds->setItem(0, 1, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        __qtablewidgetitem14->setTextAlignment(Qt::AlignCenter);
        MyFunds->setItem(0, 2, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        __qtablewidgetitem15->setTextAlignment(Qt::AlignCenter);
        MyFunds->setItem(0, 3, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        __qtablewidgetitem16->setTextAlignment(Qt::AlignCenter);
        MyFunds->setItem(0, 4, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        __qtablewidgetitem17->setTextAlignment(Qt::AlignCenter);
        MyFunds->setItem(0, 5, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        __qtablewidgetitem18->setTextAlignment(Qt::AlignCenter);
        MyFunds->setItem(0, 6, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        __qtablewidgetitem19->setTextAlignment(Qt::AlignCenter);
        MyFunds->setItem(0, 7, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        __qtablewidgetitem20->setTextAlignment(Qt::AlignCenter);
        MyFunds->setItem(1, 1, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        __qtablewidgetitem21->setTextAlignment(Qt::AlignCenter);
        MyFunds->setItem(1, 2, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        __qtablewidgetitem22->setTextAlignment(Qt::AlignCenter);
        MyFunds->setItem(1, 3, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        __qtablewidgetitem23->setTextAlignment(Qt::AlignCenter);
        MyFunds->setItem(1, 4, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        __qtablewidgetitem24->setTextAlignment(Qt::AlignCenter);
        MyFunds->setItem(1, 5, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        __qtablewidgetitem25->setTextAlignment(Qt::AlignCenter);
        MyFunds->setItem(1, 6, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        __qtablewidgetitem26->setTextAlignment(Qt::AlignCenter);
        MyFunds->setItem(1, 7, __qtablewidgetitem26);
        MyFunds->setObjectName(QStringLiteral("MyFunds"));
        MyFunds->setMaximumSize(QSize(16777215, 63));
        MyFunds->setFocusPolicy(Qt::WheelFocus);
        MyFunds->setFrameShape(QFrame::Panel);
        MyFunds->setEditTriggers(QAbstractItemView::NoEditTriggers);
        MyFunds->setGridStyle(Qt::DotLine);
        MyFunds->setRowCount(2);
        MyFunds->horizontalHeader()->setVisible(false);
        MyFunds->horizontalHeader()->setCascadingSectionResizes(true);
        MyFunds->verticalHeader()->setVisible(false);

        formLayout->setWidget(1, QFormLayout::FieldRole, MyFunds);

        mduserTable = new QTableWidget(widgetLA);
        if (mduserTable->columnCount() < 17)
            mduserTable->setColumnCount(17);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(0, __qtablewidgetitem27);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(1, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(2, __qtablewidgetitem29);
        QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(3, __qtablewidgetitem30);
        QTableWidgetItem *__qtablewidgetitem31 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(4, __qtablewidgetitem31);
        QTableWidgetItem *__qtablewidgetitem32 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(5, __qtablewidgetitem32);
        QTableWidgetItem *__qtablewidgetitem33 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(6, __qtablewidgetitem33);
        QTableWidgetItem *__qtablewidgetitem34 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(7, __qtablewidgetitem34);
        QTableWidgetItem *__qtablewidgetitem35 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(8, __qtablewidgetitem35);
        QTableWidgetItem *__qtablewidgetitem36 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(9, __qtablewidgetitem36);
        QTableWidgetItem *__qtablewidgetitem37 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(10, __qtablewidgetitem37);
        QTableWidgetItem *__qtablewidgetitem38 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(11, __qtablewidgetitem38);
        QTableWidgetItem *__qtablewidgetitem39 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(12, __qtablewidgetitem39);
        QTableWidgetItem *__qtablewidgetitem40 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(13, __qtablewidgetitem40);
        QTableWidgetItem *__qtablewidgetitem41 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(14, __qtablewidgetitem41);
        QTableWidgetItem *__qtablewidgetitem42 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(15, __qtablewidgetitem42);
        QTableWidgetItem *__qtablewidgetitem43 = new QTableWidgetItem();
        mduserTable->setHorizontalHeaderItem(16, __qtablewidgetitem43);
        if (mduserTable->rowCount() < 17)
            mduserTable->setRowCount(17);
        mduserTable->setObjectName(QStringLiteral("mduserTable"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(mduserTable->sizePolicy().hasHeightForWidth());
        mduserTable->setSizePolicy(sizePolicy1);
        mduserTable->setMinimumSize(QSize(0, 300));
        mduserTable->setMaximumSize(QSize(1380, 300));
        mduserTable->setMouseTracking(false);
        mduserTable->setToolTipDuration(0);
        mduserTable->setLayoutDirection(Qt::LeftToRight);
        mduserTable->setStyleSheet(QStringLiteral(""));
        mduserTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        mduserTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        mduserTable->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        mduserTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        mduserTable->setAlternatingRowColors(true);
        mduserTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        mduserTable->setIconSize(QSize(0, 0));
        mduserTable->setTextElideMode(Qt::ElideNone);
        mduserTable->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
        mduserTable->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
        mduserTable->setGridStyle(Qt::SolidLine);
        mduserTable->setSortingEnabled(true);
        mduserTable->setRowCount(17);
        mduserTable->verticalHeader()->setVisible(false);

        formLayout->setWidget(2, QFormLayout::FieldRole, mduserTable);

        widgetLB = new QWidget(widgetLA);
        widgetLB->setObjectName(QStringLiteral("widgetLB"));
        sizePolicy.setHeightForWidth(widgetLB->sizePolicy().hasHeightForWidth());
        widgetLB->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(widgetLB);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetNoConstraint);
        gridLayout->setContentsMargins(2, 2, 2, 2);
        placeOrder = new QGroupBox(widgetLB);
        placeOrder->setObjectName(QStringLiteral("placeOrder"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(placeOrder->sizePolicy().hasHeightForWidth());
        placeOrder->setSizePolicy(sizePolicy2);
        placeOrder->setMinimumSize(QSize(350, 280));
        placeOrder->setMaximumSize(QSize(350, 280));
        placeOrder->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        label = new QLabel(placeOrder);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 30, 51, 21));
        label->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        newOrder = new QComboBox(placeOrder);
        newOrder->setObjectName(QStringLiteral("newOrder"));
        newOrder->setGeometry(QRect(70, 30, 81, 22));
        order_buy = new QRadioButton(placeOrder);
        order_buy->setObjectName(QStringLiteral("order_buy"));
        order_buy->setGeometry(QRect(160, 30, 61, 16));
        order_buy->setChecked(true);
        order_sale = new QRadioButton(placeOrder);
        order_sale->setObjectName(QStringLiteral("order_sale"));
        order_sale->setGeometry(QRect(220, 30, 61, 16));
        kaicang = new QRadioButton(placeOrder);
        kaicang->setObjectName(QStringLiteral("kaicang"));
        kaicang->setGeometry(QRect(80, 60, 61, 20));
        kaicang->setChecked(false);
        pingcang = new QRadioButton(placeOrder);
        pingcang->setObjectName(QStringLiteral("pingcang"));
        pingcang->setGeometry(QRect(150, 60, 61, 21));
        shoushu = new QLabel(placeOrder);
        shoushu->setObjectName(QStringLiteral("shoushu"));
        shoushu->setGeometry(QRect(20, 100, 41, 31));
        shoushu->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        trader_hand = new QSpinBox(placeOrder);
        trader_hand->setObjectName(QStringLiteral("trader_hand"));
        trader_hand->setGeometry(QRect(80, 100, 161, 31));
        trader_hand->setMinimum(1);
        trader_hand->setMaximum(50);
        trader_hand->setValue(1);
        zhidingjia = new QLabel(placeOrder);
        zhidingjia->setObjectName(QStringLiteral("zhidingjia"));
        zhidingjia->setGeometry(QRect(20, 150, 51, 31));
        zhidingjia->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        order_price = new QDoubleSpinBox(placeOrder);
        order_price->setObjectName(QStringLiteral("order_price"));
        order_price->setGeometry(QRect(80, 150, 161, 31));
        order_price->setDecimals(1);
        order_price->setMaximum(100000);
        order_price->setSingleStep(0.2);
        kaiping = new QLabel(placeOrder);
        kaiping->setObjectName(QStringLiteral("kaiping"));
        kaiping->setGeometry(QRect(10, 60, 51, 21));
        kaiping->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        btnPlaceOrder = new QPushButton(placeOrder);
        btnPlaceOrder->setObjectName(QStringLiteral("btnPlaceOrder"));
        btnPlaceOrder->setGeometry(QRect(30, 200, 151, 51));
        reset = new QPushButton(placeOrder);
        reset->setObjectName(QStringLiteral("reset"));
        reset->setGeometry(QRect(200, 200, 101, 51));
        limitprice = new QRadioButton(placeOrder);
        limitprice->setObjectName(QStringLiteral("limitprice"));
        limitprice->setGeometry(QRect(270, 90, 61, 20));
        limitprice->setChecked(false);
        kaiping_2 = new QLabel(placeOrder);
        kaiping_2->setObjectName(QStringLiteral("kaiping_2"));
        kaiping_2->setGeometry(QRect(270, 60, 51, 21));
        kaiping_2->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        anyprice = new QRadioButton(placeOrder);
        anyprice->setObjectName(QStringLiteral("anyprice"));
        anyprice->setGeometry(QRect(270, 120, 61, 20));
        anyprice->setChecked(false);

        gridLayout->addWidget(placeOrder, 0, 1, 1, 1);

        myOrder = new QTabWidget(widgetLB);
        myOrder->setObjectName(QStringLiteral("myOrder"));
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(myOrder->sizePolicy().hasHeightForWidth());
        myOrder->setSizePolicy(sizePolicy3);
        myOrder->setMaximumSize(QSize(1000, 500));
        myOrder->setTabPosition(QTabWidget::West);
        myOrder->setTabShape(QTabWidget::Triangular);
        myOrder->setIconSize(QSize(18, 18));
        entrustOrder = new QWidget();
        entrustOrder->setObjectName(QStringLiteral("entrustOrder"));
        entrust_Order = new QTableWidget(entrustOrder);
        if (entrust_Order->columnCount() < 11)
            entrust_Order->setColumnCount(11);
        QTableWidgetItem *__qtablewidgetitem44 = new QTableWidgetItem();
        entrust_Order->setHorizontalHeaderItem(0, __qtablewidgetitem44);
        QTableWidgetItem *__qtablewidgetitem45 = new QTableWidgetItem();
        entrust_Order->setHorizontalHeaderItem(1, __qtablewidgetitem45);
        QTableWidgetItem *__qtablewidgetitem46 = new QTableWidgetItem();
        entrust_Order->setHorizontalHeaderItem(2, __qtablewidgetitem46);
        QTableWidgetItem *__qtablewidgetitem47 = new QTableWidgetItem();
        entrust_Order->setHorizontalHeaderItem(3, __qtablewidgetitem47);
        QTableWidgetItem *__qtablewidgetitem48 = new QTableWidgetItem();
        entrust_Order->setHorizontalHeaderItem(4, __qtablewidgetitem48);
        QTableWidgetItem *__qtablewidgetitem49 = new QTableWidgetItem();
        entrust_Order->setHorizontalHeaderItem(5, __qtablewidgetitem49);
        QTableWidgetItem *__qtablewidgetitem50 = new QTableWidgetItem();
        entrust_Order->setHorizontalHeaderItem(6, __qtablewidgetitem50);
        QTableWidgetItem *__qtablewidgetitem51 = new QTableWidgetItem();
        entrust_Order->setHorizontalHeaderItem(7, __qtablewidgetitem51);
        QTableWidgetItem *__qtablewidgetitem52 = new QTableWidgetItem();
        entrust_Order->setHorizontalHeaderItem(8, __qtablewidgetitem52);
        QTableWidgetItem *__qtablewidgetitem53 = new QTableWidgetItem();
        entrust_Order->setHorizontalHeaderItem(9, __qtablewidgetitem53);
        QTableWidgetItem *__qtablewidgetitem54 = new QTableWidgetItem();
        entrust_Order->setHorizontalHeaderItem(10, __qtablewidgetitem54);
        if (entrust_Order->rowCount() < 30)
            entrust_Order->setRowCount(30);
        entrust_Order->setObjectName(QStringLiteral("entrust_Order"));
        entrust_Order->setGeometry(QRect(0, 0, 971, 271));
        sizePolicy.setHeightForWidth(entrust_Order->sizePolicy().hasHeightForWidth());
        entrust_Order->setSizePolicy(sizePolicy);
        entrust_Order->setMinimumSize(QSize(0, 0));
        entrust_Order->setMaximumSize(QSize(1380, 16777215));
        entrust_Order->setMouseTracking(false);
        entrust_Order->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        entrust_Order->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        entrust_Order->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        entrust_Order->setEditTriggers(QAbstractItemView::NoEditTriggers);
        entrust_Order->setSelectionBehavior(QAbstractItemView::SelectRows);
        entrust_Order->setTextElideMode(Qt::ElideRight);
        entrust_Order->setRowCount(30);
        entrust_Order->verticalHeader()->setVisible(false);
        myOrder->addTab(entrustOrder, QString());
        finishOrder = new QWidget();
        finishOrder->setObjectName(QStringLiteral("finishOrder"));
        finish_Order = new QTableWidget(finishOrder);
        if (finish_Order->columnCount() < 11)
            finish_Order->setColumnCount(11);
        QTableWidgetItem *__qtablewidgetitem55 = new QTableWidgetItem();
        finish_Order->setHorizontalHeaderItem(0, __qtablewidgetitem55);
        QTableWidgetItem *__qtablewidgetitem56 = new QTableWidgetItem();
        finish_Order->setHorizontalHeaderItem(1, __qtablewidgetitem56);
        QTableWidgetItem *__qtablewidgetitem57 = new QTableWidgetItem();
        finish_Order->setHorizontalHeaderItem(2, __qtablewidgetitem57);
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        QTableWidgetItem *__qtablewidgetitem58 = new QTableWidgetItem();
        __qtablewidgetitem58->setFont(font);
        finish_Order->setHorizontalHeaderItem(3, __qtablewidgetitem58);
        QTableWidgetItem *__qtablewidgetitem59 = new QTableWidgetItem();
        __qtablewidgetitem59->setFont(font);
        finish_Order->setHorizontalHeaderItem(4, __qtablewidgetitem59);
        QTableWidgetItem *__qtablewidgetitem60 = new QTableWidgetItem();
        finish_Order->setHorizontalHeaderItem(5, __qtablewidgetitem60);
        QTableWidgetItem *__qtablewidgetitem61 = new QTableWidgetItem();
        finish_Order->setHorizontalHeaderItem(6, __qtablewidgetitem61);
        QTableWidgetItem *__qtablewidgetitem62 = new QTableWidgetItem();
        finish_Order->setHorizontalHeaderItem(7, __qtablewidgetitem62);
        QTableWidgetItem *__qtablewidgetitem63 = new QTableWidgetItem();
        finish_Order->setHorizontalHeaderItem(8, __qtablewidgetitem63);
        QTableWidgetItem *__qtablewidgetitem64 = new QTableWidgetItem();
        finish_Order->setHorizontalHeaderItem(9, __qtablewidgetitem64);
        QTableWidgetItem *__qtablewidgetitem65 = new QTableWidgetItem();
        finish_Order->setHorizontalHeaderItem(10, __qtablewidgetitem65);
        if (finish_Order->rowCount() < 30)
            finish_Order->setRowCount(30);
        finish_Order->setObjectName(QStringLiteral("finish_Order"));
        finish_Order->setGeometry(QRect(0, 0, 981, 271));
        sizePolicy.setHeightForWidth(finish_Order->sizePolicy().hasHeightForWidth());
        finish_Order->setSizePolicy(sizePolicy);
        finish_Order->setMinimumSize(QSize(0, 0));
        finish_Order->setMaximumSize(QSize(16777215, 16777215));
        finish_Order->setMouseTracking(false);
        finish_Order->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        finish_Order->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        finish_Order->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        finish_Order->setAutoScroll(true);
        finish_Order->setEditTriggers(QAbstractItemView::NoEditTriggers);
        finish_Order->setSelectionBehavior(QAbstractItemView::SelectRows);
        finish_Order->setTextElideMode(Qt::ElideRight);
        finish_Order->setRowCount(30);
        finish_Order->verticalHeader()->setVisible(false);
        myOrder->addTab(finishOrder, QString());
        position = new QWidget();
        position->setObjectName(QStringLiteral("position"));
        position_detail = new QTableWidget(position);
        if (position_detail->columnCount() < 6)
            position_detail->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem66 = new QTableWidgetItem();
        position_detail->setHorizontalHeaderItem(0, __qtablewidgetitem66);
        QTableWidgetItem *__qtablewidgetitem67 = new QTableWidgetItem();
        position_detail->setHorizontalHeaderItem(1, __qtablewidgetitem67);
        QTableWidgetItem *__qtablewidgetitem68 = new QTableWidgetItem();
        position_detail->setHorizontalHeaderItem(2, __qtablewidgetitem68);
        QTableWidgetItem *__qtablewidgetitem69 = new QTableWidgetItem();
        position_detail->setHorizontalHeaderItem(3, __qtablewidgetitem69);
        QTableWidgetItem *__qtablewidgetitem70 = new QTableWidgetItem();
        position_detail->setHorizontalHeaderItem(4, __qtablewidgetitem70);
        QTableWidgetItem *__qtablewidgetitem71 = new QTableWidgetItem();
        position_detail->setHorizontalHeaderItem(5, __qtablewidgetitem71);
        if (position_detail->rowCount() < 20)
            position_detail->setRowCount(20);
        position_detail->setObjectName(QStringLiteral("position_detail"));
        position_detail->setGeometry(QRect(0, 0, 981, 271));
        sizePolicy.setHeightForWidth(position_detail->sizePolicy().hasHeightForWidth());
        position_detail->setSizePolicy(sizePolicy);
        position_detail->setMinimumSize(QSize(0, 0));
        position_detail->setMaximumSize(QSize(16777215, 16777215));
        position_detail->setMouseTracking(false);
        position_detail->setToolTipDuration(0);
        position_detail->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        position_detail->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        position_detail->setEditTriggers(QAbstractItemView::NoEditTriggers);
        position_detail->setSelectionBehavior(QAbstractItemView::SelectRows);
        position_detail->setTextElideMode(Qt::ElideRight);
        position_detail->setRowCount(20);
        position_detail->verticalHeader()->setVisible(false);
        myOrder->addTab(position, QString());

        gridLayout->addWidget(myOrder, 0, 0, 1, 1);


        formLayout->setWidget(3, QFormLayout::FieldRole, widgetLB);

        Qt_femasClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Qt_femasClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1282, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        Qt_femasClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Qt_femasClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Qt_femasClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        toolBar = new QToolBar(Qt_femasClass);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        Qt_femasClass->addToolBar(Qt::TopToolBarArea, toolBar);
        statusBar = new QStatusBar(Qt_femasClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Qt_femasClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addAction(action);
        menu->addAction(actionGuanyu);
        menu->addAction(actionTuichu);
        menu_2->addAction(actionK);
        menu_2->addAction(action_2);

        retranslateUi(Qt_femasClass);

        myOrder->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Qt_femasClass);
    } // setupUi

    void retranslateUi(QMainWindow *Qt_femasClass)
    {
        Qt_femasClass->setWindowTitle(QApplication::translate("Qt_femasClass", "\351\243\236\351\251\254\344\272\244\346\230\223\345\256\242\346\210\267\347\253\257", 0));
        actionK->setText(QApplication::translate("Qt_femasClass", "\347\233\230\345\217\243\350\241\214\346\203\205\347\225\214\351\235\242", 0));
        action->setText(QApplication::translate("Qt_femasClass", "\345\270\256\345\212\251", 0));
        actionGuanyu->setText(QApplication::translate("Qt_femasClass", "\345\205\263\344\272\216", 0));
        actionTuichu->setText(QApplication::translate("Qt_femasClass", "\351\200\200\345\207\272", 0));
        action_2->setText(QApplication::translate("Qt_femasClass", "K\347\272\277\345\233\276", 0));
        QTableWidgetItem *___qtablewidgetitem = MyFunds->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Qt_femasClass", "\346\226\260\345\273\272\345\210\227", 0));
        QTableWidgetItem *___qtablewidgetitem1 = MyFunds->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Qt_femasClass", "\346\226\260\345\273\272\345\210\227", 0));
        QTableWidgetItem *___qtablewidgetitem2 = MyFunds->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("Qt_femasClass", "\346\226\260\345\273\272\345\210\227", 0));
        QTableWidgetItem *___qtablewidgetitem3 = MyFunds->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("Qt_femasClass", "\346\226\260\345\273\272\345\210\227", 0));
        QTableWidgetItem *___qtablewidgetitem4 = MyFunds->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("Qt_femasClass", "\346\226\260\345\273\272\345\210\227", 0));
        QTableWidgetItem *___qtablewidgetitem5 = MyFunds->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("Qt_femasClass", "\346\226\260\345\273\272\345\210\227", 0));
        QTableWidgetItem *___qtablewidgetitem6 = MyFunds->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("Qt_femasClass", "\346\226\260\345\273\272\345\210\227", 0));
        QTableWidgetItem *___qtablewidgetitem7 = MyFunds->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("Qt_femasClass", "\346\226\260\345\273\272\345\210\227", 0));
        QTableWidgetItem *___qtablewidgetitem8 = MyFunds->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("Qt_femasClass", "\346\226\260\345\273\272\345\210\227", 0));
        QTableWidgetItem *___qtablewidgetitem9 = MyFunds->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QApplication::translate("Qt_femasClass", "\346\226\260\345\273\272\345\210\227", 0));
        QTableWidgetItem *___qtablewidgetitem10 = MyFunds->horizontalHeaderItem(10);
        ___qtablewidgetitem10->setText(QApplication::translate("Qt_femasClass", "\346\226\260\345\273\272\345\210\227", 0));
        QTableWidgetItem *___qtablewidgetitem11 = MyFunds->horizontalHeaderItem(11);
        ___qtablewidgetitem11->setText(QApplication::translate("Qt_femasClass", "\346\226\260\345\273\272\345\210\227", 0));

        const bool __sortingEnabled = MyFunds->isSortingEnabled();
        MyFunds->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem12 = MyFunds->item(0, 0);
        ___qtablewidgetitem12->setText(QApplication::translate("Qt_femasClass", "\350\265\204\351\207\221\350\264\246\346\210\267", 0));
        QTableWidgetItem *___qtablewidgetitem13 = MyFunds->item(0, 1);
        ___qtablewidgetitem13->setText(QApplication::translate("Qt_femasClass", "\345\217\257\347\224\250\350\265\204\351\207\221", 0));
        QTableWidgetItem *___qtablewidgetitem14 = MyFunds->item(0, 2);
        ___qtablewidgetitem14->setText(QApplication::translate("Qt_femasClass", "\345\271\263\344\273\223\347\233\210\344\272\217", 0));
        QTableWidgetItem *___qtablewidgetitem15 = MyFunds->item(0, 3);
        ___qtablewidgetitem15->setText(QApplication::translate("Qt_femasClass", "\346\214\201\344\273\223\347\233\210\344\272\217", 0));
        QTableWidgetItem *___qtablewidgetitem16 = MyFunds->item(0, 4);
        ___qtablewidgetitem16->setText(QApplication::translate("Qt_femasClass", "\345\215\240\347\224\250\344\277\235\350\257\201\351\207\221", 0));
        QTableWidgetItem *___qtablewidgetitem17 = MyFunds->item(0, 5);
        ___qtablewidgetitem17->setText(QApplication::translate("Qt_femasClass", "\346\211\213\347\273\255\350\264\271", 0));
        QTableWidgetItem *___qtablewidgetitem18 = MyFunds->item(0, 6);
        ___qtablewidgetitem18->setText(QApplication::translate("Qt_femasClass", "\346\235\203\345\210\251\351\207\221", 0));
        MyFunds->setSortingEnabled(__sortingEnabled);

#ifndef QT_NO_WHATSTHIS
        MyFunds->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        QTableWidgetItem *___qtablewidgetitem19 = mduserTable->horizontalHeaderItem(0);
        ___qtablewidgetitem19->setText(QApplication::translate("Qt_femasClass", "\345\220\210\347\272\246\345\220\215", 0));
        QTableWidgetItem *___qtablewidgetitem20 = mduserTable->horizontalHeaderItem(1);
        ___qtablewidgetitem20->setText(QApplication::translate("Qt_femasClass", "\346\234\200\346\226\260\344\273\267", 0));
        QTableWidgetItem *___qtablewidgetitem21 = mduserTable->horizontalHeaderItem(2);
        ___qtablewidgetitem21->setText(QApplication::translate("Qt_femasClass", "\344\271\260\344\273\267", 0));
        QTableWidgetItem *___qtablewidgetitem22 = mduserTable->horizontalHeaderItem(3);
        ___qtablewidgetitem22->setText(QApplication::translate("Qt_femasClass", "\344\271\260\351\207\217", 0));
        QTableWidgetItem *___qtablewidgetitem23 = mduserTable->horizontalHeaderItem(4);
        ___qtablewidgetitem23->setText(QApplication::translate("Qt_femasClass", "\345\215\226\344\273\267", 0));
        QTableWidgetItem *___qtablewidgetitem24 = mduserTable->horizontalHeaderItem(5);
        ___qtablewidgetitem24->setText(QApplication::translate("Qt_femasClass", "\345\215\226\351\207\217", 0));
        QTableWidgetItem *___qtablewidgetitem25 = mduserTable->horizontalHeaderItem(6);
        ___qtablewidgetitem25->setText(QApplication::translate("Qt_femasClass", "\346\210\220\344\272\244\351\207\217", 0));
        QTableWidgetItem *___qtablewidgetitem26 = mduserTable->horizontalHeaderItem(7);
        ___qtablewidgetitem26->setText(QApplication::translate("Qt_femasClass", "\346\214\201\344\273\223\351\207\217", 0));
        QTableWidgetItem *___qtablewidgetitem27 = mduserTable->horizontalHeaderItem(8);
        ___qtablewidgetitem27->setText(QApplication::translate("Qt_femasClass", "\346\266\250\345\201\234\344\273\267", 0));
        QTableWidgetItem *___qtablewidgetitem28 = mduserTable->horizontalHeaderItem(9);
        ___qtablewidgetitem28->setText(QApplication::translate("Qt_femasClass", "\350\267\214\345\201\234\344\273\267", 0));
        QTableWidgetItem *___qtablewidgetitem29 = mduserTable->horizontalHeaderItem(10);
        ___qtablewidgetitem29->setText(QApplication::translate("Qt_femasClass", "\344\273\212\346\224\266\347\233\230", 0));
        QTableWidgetItem *___qtablewidgetitem30 = mduserTable->horizontalHeaderItem(11);
        ___qtablewidgetitem30->setText(QApplication::translate("Qt_femasClass", "\346\230\250\346\224\266\347\233\230", 0));
        QTableWidgetItem *___qtablewidgetitem31 = mduserTable->horizontalHeaderItem(12);
        ___qtablewidgetitem31->setText(QApplication::translate("Qt_femasClass", "\346\234\200\351\253\230\344\273\267", 0));
        QTableWidgetItem *___qtablewidgetitem32 = mduserTable->horizontalHeaderItem(13);
        ___qtablewidgetitem32->setText(QApplication::translate("Qt_femasClass", "\346\234\200\344\275\216\344\273\267", 0));
        QTableWidgetItem *___qtablewidgetitem33 = mduserTable->horizontalHeaderItem(14);
        ___qtablewidgetitem33->setText(QApplication::translate("Qt_femasClass", "\346\230\250\347\273\223\347\256\227\344\273\267", 0));
        QTableWidgetItem *___qtablewidgetitem34 = mduserTable->horizontalHeaderItem(15);
        ___qtablewidgetitem34->setText(QApplication::translate("Qt_femasClass", "\346\210\220\344\272\244\351\242\235", 0));
        QTableWidgetItem *___qtablewidgetitem35 = mduserTable->horizontalHeaderItem(16);
        ___qtablewidgetitem35->setText(QApplication::translate("Qt_femasClass", "\350\241\214\346\203\205\346\233\264\346\226\260\346\227\266\351\227\264", 0));
        placeOrder->setTitle(QApplication::translate("Qt_femasClass", "\344\270\213\350\256\242\345\215\225", 0));
        label->setText(QApplication::translate("Qt_femasClass", "<html><head/><body><p align=\"center\"><span style=\" font-size:11pt;\">\345\220\210\347\272\246</span></p></body></html>", 0));
        order_buy->setText(QApplication::translate("Qt_femasClass", "\344\271\260\345\205\245", 0));
        order_sale->setText(QApplication::translate("Qt_femasClass", "\345\215\226\345\207\272", 0));
        kaicang->setText(QApplication::translate("Qt_femasClass", "\345\274\200\344\273\223", 0));
        pingcang->setText(QApplication::translate("Qt_femasClass", "\345\271\263\344\273\223", 0));
        shoushu->setText(QApplication::translate("Qt_femasClass", "<html><head/><body><p align=\"center\"><span style=\" font-size:9pt;\">\346\211\213\346\225\260</span></p></body></html>", 0));
        zhidingjia->setText(QApplication::translate("Qt_femasClass", "<html><head/><body><p align=\"center\"><span style=\" font-size:9pt;\">\346\214\207\345\256\232\344\273\267</span></p></body></html>", 0));
        kaiping->setText(QApplication::translate("Qt_femasClass", "<html><head/><body><p align=\"center\">\345\274\200\345\271\263</p></body></html>", 0));
        btnPlaceOrder->setText(QApplication::translate("Qt_femasClass", "\347\241\256\345\256\232\344\270\213\345\215\225", 0));
        reset->setText(QApplication::translate("Qt_femasClass", "\351\207\215\347\275\256", 0));
        limitprice->setText(QApplication::translate("Qt_femasClass", "\351\231\220\344\273\267", 0));
        kaiping_2->setText(QApplication::translate("Qt_femasClass", "<html><head/><body><p align=\"center\">\344\270\213\345\215\225\346\214\207\344\273\244</p></body></html>", 0));
        anyprice->setText(QApplication::translate("Qt_femasClass", "\345\270\202\344\273\267", 0));
        QTableWidgetItem *___qtablewidgetitem36 = entrust_Order->horizontalHeaderItem(0);
        ___qtablewidgetitem36->setText(QApplication::translate("Qt_femasClass", "\346\212\245\345\215\225\347\274\226\345\217\267", 0));
        QTableWidgetItem *___qtablewidgetitem37 = entrust_Order->horizontalHeaderItem(1);
        ___qtablewidgetitem37->setText(QApplication::translate("Qt_femasClass", "\345\220\210\347\272\246", 0));
        QTableWidgetItem *___qtablewidgetitem38 = entrust_Order->horizontalHeaderItem(2);
        ___qtablewidgetitem38->setText(QApplication::translate("Qt_femasClass", "\344\271\260\345\215\226", 0));
        QTableWidgetItem *___qtablewidgetitem39 = entrust_Order->horizontalHeaderItem(3);
        ___qtablewidgetitem39->setText(QApplication::translate("Qt_femasClass", "\345\274\200\345\271\263", 0));
        QTableWidgetItem *___qtablewidgetitem40 = entrust_Order->horizontalHeaderItem(4);
        ___qtablewidgetitem40->setText(QApplication::translate("Qt_femasClass", "\347\212\266\346\200\201", 0));
        QTableWidgetItem *___qtablewidgetitem41 = entrust_Order->horizontalHeaderItem(5);
        ___qtablewidgetitem41->setText(QApplication::translate("Qt_femasClass", "\346\212\245\345\215\225\344\273\267\346\240\274", 0));
        QTableWidgetItem *___qtablewidgetitem42 = entrust_Order->horizontalHeaderItem(6);
        ___qtablewidgetitem42->setText(QApplication::translate("Qt_femasClass", "\346\234\252\346\210\220\344\272\244\346\211\213\346\225\260", 0));
        QTableWidgetItem *___qtablewidgetitem43 = entrust_Order->horizontalHeaderItem(7);
        ___qtablewidgetitem43->setText(QApplication::translate("Qt_femasClass", "\346\212\245\345\215\225\346\211\213\346\225\260", 0));
        QTableWidgetItem *___qtablewidgetitem44 = entrust_Order->horizontalHeaderItem(8);
        ___qtablewidgetitem44->setText(QApplication::translate("Qt_femasClass", "\346\210\220\344\272\244\345\235\207\344\273\267", 0));
        QTableWidgetItem *___qtablewidgetitem45 = entrust_Order->horizontalHeaderItem(9);
        ___qtablewidgetitem45->setText(QApplication::translate("Qt_femasClass", "\346\212\245\345\215\225\346\227\266\351\227\264", 0));
        QTableWidgetItem *___qtablewidgetitem46 = entrust_Order->horizontalHeaderItem(10);
        ___qtablewidgetitem46->setText(QApplication::translate("Qt_femasClass", "\344\272\244\346\230\223\346\211\200", 0));
        myOrder->setTabText(myOrder->indexOf(entrustOrder), QApplication::translate("Qt_femasClass", "\345\247\224\346\211\230\350\256\242\345\215\225", 0));
        QTableWidgetItem *___qtablewidgetitem47 = finish_Order->horizontalHeaderItem(0);
        ___qtablewidgetitem47->setText(QApplication::translate("Qt_femasClass", "\346\210\220\344\272\244\347\274\226\345\217\267", 0));
        QTableWidgetItem *___qtablewidgetitem48 = finish_Order->horizontalHeaderItem(1);
        ___qtablewidgetitem48->setText(QApplication::translate("Qt_femasClass", "\345\220\210\347\272\246", 0));
        QTableWidgetItem *___qtablewidgetitem49 = finish_Order->horizontalHeaderItem(2);
        ___qtablewidgetitem49->setText(QApplication::translate("Qt_femasClass", "\344\271\260\345\215\226", 0));
        QTableWidgetItem *___qtablewidgetitem50 = finish_Order->horizontalHeaderItem(3);
        ___qtablewidgetitem50->setText(QApplication::translate("Qt_femasClass", "\345\274\200\345\271\263", 0));
        QTableWidgetItem *___qtablewidgetitem51 = finish_Order->horizontalHeaderItem(4);
        ___qtablewidgetitem51->setText(QApplication::translate("Qt_femasClass", "\346\210\220\344\272\244\344\273\267\346\240\274", 0));
        QTableWidgetItem *___qtablewidgetitem52 = finish_Order->horizontalHeaderItem(5);
        ___qtablewidgetitem52->setText(QApplication::translate("Qt_femasClass", "\346\210\220\344\272\244\346\211\213\346\225\260", 0));
        QTableWidgetItem *___qtablewidgetitem53 = finish_Order->horizontalHeaderItem(6);
        ___qtablewidgetitem53->setText(QApplication::translate("Qt_femasClass", "\346\210\220\344\272\244\346\227\266\351\227\264", 0));
        QTableWidgetItem *___qtablewidgetitem54 = finish_Order->horizontalHeaderItem(7);
        ___qtablewidgetitem54->setText(QApplication::translate("Qt_femasClass", "\346\210\220\344\272\244\347\261\273\345\236\213", 0));
        QTableWidgetItem *___qtablewidgetitem55 = finish_Order->horizontalHeaderItem(8);
        ___qtablewidgetitem55->setText(QApplication::translate("Qt_femasClass", "\346\212\245\345\215\225\347\274\226\345\217\267", 0));
        QTableWidgetItem *___qtablewidgetitem56 = finish_Order->horizontalHeaderItem(9);
        ___qtablewidgetitem56->setText(QApplication::translate("Qt_femasClass", "\346\212\245\345\215\225\346\227\266\351\227\264", 0));
        QTableWidgetItem *___qtablewidgetitem57 = finish_Order->horizontalHeaderItem(10);
        ___qtablewidgetitem57->setText(QApplication::translate("Qt_femasClass", "\344\272\244\346\230\223\346\211\200", 0));
        myOrder->setTabText(myOrder->indexOf(finishOrder), QApplication::translate("Qt_femasClass", "\346\210\220\344\272\244\350\256\260\345\275\225", 0));
        QTableWidgetItem *___qtablewidgetitem58 = position_detail->horizontalHeaderItem(0);
        ___qtablewidgetitem58->setText(QApplication::translate("Qt_femasClass", "\345\220\210\347\272\246", 0));
        QTableWidgetItem *___qtablewidgetitem59 = position_detail->horizontalHeaderItem(1);
        ___qtablewidgetitem59->setText(QApplication::translate("Qt_femasClass", "\344\271\260\345\215\226", 0));
        QTableWidgetItem *___qtablewidgetitem60 = position_detail->horizontalHeaderItem(2);
        ___qtablewidgetitem60->setText(QApplication::translate("Qt_femasClass", "\346\200\273\346\214\201\344\273\223", 0));
        QTableWidgetItem *___qtablewidgetitem61 = position_detail->horizontalHeaderItem(3);
        ___qtablewidgetitem61->setText(QApplication::translate("Qt_femasClass", "\345\271\263\345\235\207\346\210\220\346\234\254", 0));
        QTableWidgetItem *___qtablewidgetitem62 = position_detail->horizontalHeaderItem(4);
        ___qtablewidgetitem62->setText(QApplication::translate("Qt_femasClass", "\345\217\257\345\271\263\351\207\217", 0));
        QTableWidgetItem *___qtablewidgetitem63 = position_detail->horizontalHeaderItem(5);
        ___qtablewidgetitem63->setText(QApplication::translate("Qt_femasClass", "\344\272\244\346\230\223\346\211\200", 0));
        myOrder->setTabText(myOrder->indexOf(position), QApplication::translate("Qt_femasClass", "\346\214\201\344\273\223\346\230\216\347\273\206", 0));
        menu->setTitle(QApplication::translate("Qt_femasClass", "\347\263\273\347\273\237", 0));
        menu_2->setTitle(QApplication::translate("Qt_femasClass", "\346\230\276\347\244\272", 0));
        toolBar->setWindowTitle(QApplication::translate("Qt_femasClass", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class Qt_femasClass: public Ui_Qt_femasClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT_FEMAS_H
