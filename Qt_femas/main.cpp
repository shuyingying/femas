#include <QtWidgets/QApplication>
#include "PublicFuncs.h"
#include "qt_femas.h"
#include "logindialog.h"

int main(int argc,char *argv[])
{
		// ����̨
	AllocConsole();                     // �򿪿���̨��Դ
	freopen( "CONOUT$", "w+t", stdout );// ����д
	freopen( "CONIN$", "r+t", stdin );  // �����

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