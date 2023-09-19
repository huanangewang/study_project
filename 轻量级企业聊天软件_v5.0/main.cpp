#include "CCMainWindow.h"
#include <QtWidgets/QApplication>
#include	"UserLogin.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//CCMainWindow w;
	//w.show();

	//为了不让换肤窗口点击关闭，整个应用程序关闭 -->默认true,这样的话，只有最后一个窗口关闭，才会退出
	a.setQuitOnLastWindowClosed(false);

	UserLogin* userLogin = new UserLogin;
	userLogin->show(); //2023.8.5 做好了登录窗口，但是是透明的，是因为我们还没设置样式表
	
	return a.exec();
}
