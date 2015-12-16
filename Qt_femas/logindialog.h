#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QtWidgets/QDialog>
#include "ui_LoginDialog.h"

class LoginDialog : public QDialog
{
	Q_OBJECT

public:
	explicit LoginDialog(QWidget *parent = 0);
	~LoginDialog();

private slots:
	void on_button_clickd_LoginIn();

private:
	Ui::Dialog *ui;
};

#endif // LOGINDIALOG_H
