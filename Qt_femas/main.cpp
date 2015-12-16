#include <QtWidgets/QApplication>
#include "PublicFuncs.h"
#include "qt_femas.h"
#include "logindialog.h"

int main(int argc,char *argv[])
{
		// 控制台
	AllocConsole();                     // 打开控制台资源
	freopen( "CONOUT$", "w+t", stdout );// 申请写
	freopen( "CONIN$", "r+t", stdin );  // 申请读

	QApplication a(argc, argv);
	a.setWindowIcon(QIcon("qt_femas1.ico"));
	LoginDialog loginDialog;
	Qt_femas w;
	if(loginDialog.exec() == QDialog::Accepted)
	{
	   w.show();
	   return a.exec();
	}
	else
	return 0;
}