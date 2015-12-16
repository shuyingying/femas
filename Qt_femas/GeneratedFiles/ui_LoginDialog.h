/********************************************************************************
** Form generated from reading UI file 'LoginDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLabel *label;
    QPushButton *quit;
    QLabel *label_2;
    QPushButton *LoginIn;
    QLineEdit *textUserID;
    QLineEdit *textPassword;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *textInvestorID;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(734, 477);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/ femasImage"), QSize(), QIcon::Normal, QIcon::Off);
        Dialog->setWindowIcon(icon);
        Dialog->setStyleSheet(QStringLiteral("background-image: url(:/images/ backImage);"));
        label = new QLabel(Dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 731, 131));
        quit = new QPushButton(Dialog);
        quit->setObjectName(QStringLiteral("quit"));
        quit->setGeometry(QRect(380, 300, 121, 41));
        quit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(190, 230, 101, 31));
        LoginIn = new QPushButton(Dialog);
        LoginIn->setObjectName(QStringLiteral("LoginIn"));
        LoginIn->setGeometry(QRect(210, 300, 121, 41));
        LoginIn->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        textUserID = new QLineEdit(Dialog);
        textUserID->setObjectName(QStringLiteral("textUserID"));
        textUserID->setGeometry(QRect(310, 150, 231, 31));
        textPassword = new QLineEdit(Dialog);
        textPassword->setObjectName(QStringLiteral("textPassword"));
        textPassword->setGeometry(QRect(310, 230, 231, 31));
        textPassword->setEchoMode(QLineEdit::Password);
        label_3 = new QLabel(Dialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(190, 150, 101, 31));
        label_3->setStyleSheet(QStringLiteral("color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));"));
        label_4 = new QLabel(Dialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(190, 190, 101, 31));
        label_4->setStyleSheet(QStringLiteral("color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));"));
        textInvestorID = new QLineEdit(Dialog);
        textInvestorID->setObjectName(QStringLiteral("textInvestorID"));
        textInvestorID->setGeometry(QRect(310, 190, 231, 31));
#ifndef QT_NO_SHORTCUT
        label_2->setBuddy(textPassword);
        label_3->setBuddy(textUserID);
        label_4->setBuddy(textInvestorID);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(textUserID, textInvestorID);
        QWidget::setTabOrder(textInvestorID, textPassword);
        QWidget::setTabOrder(textPassword, LoginIn);
        QWidget::setTabOrder(LoginIn, quit);

        retranslateUi(Dialog);
        QObject::connect(quit, SIGNAL(clicked()), Dialog, SLOT(close()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "DIY\351\243\236\351\251\254\344\272\244\346\230\223\345\256\242\346\210\267\347\253\257 V0.0", 0));
        label->setText(QApplication::translate("Dialog", "<html><head/><body><p align=\"center\"><span style=\" font-size:16pt; font-weight:600; color:#aa0000;\">\346\254\242\350\277\216\344\275\277\347\224\250\346\210\221\344\273\254\347\232\204DIY\351\243\236\351\251\254\344\272\244\346\230\223\345\256\242\346\210\267\347\253\257</span></p></body></html>", 0));
        quit->setText(QApplication::translate("Dialog", "\351\200\200 \345\207\272", 0));
        label_2->setText(QApplication::translate("Dialog", "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt; font-weight:600; color:#55007f;\">\345\257\206 \347\240\201</span></p></body></html>", 0));
        LoginIn->setText(QApplication::translate("Dialog", "\347\231\273 \345\275\225", 0));
        textUserID->setPlaceholderText(QApplication::translate("Dialog", "\350\257\267\350\276\223\345\205\245UserID", 0));
        textPassword->setPlaceholderText(QApplication::translate("Dialog", "\350\257\267\350\276\223\345\205\245Password", 0));
        label_3->setText(QApplication::translate("Dialog", "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt; font-weight:600; color:#55007f;\">\347\224\250\346\210\267\344\273\243\347\240\201</span></p></body></html>", 0));
        label_4->setText(QApplication::translate("Dialog", "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt; font-weight:600; color:#55007f;\">\346\212\225\350\265\204\350\200\205\347\274\226\345\217\267</span></p></body></html>", 0));
        textInvestorID->setPlaceholderText(QApplication::translate("Dialog", "\350\257\267\350\276\223\345\205\245InvestorID", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
