#include "CCMainWindow.h"
#include <QtWidgets/QApplication>
#include	"UserLogin.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//CCMainWindow w;
	//w.show();

	//Ϊ�˲��û������ڵ���رգ�����Ӧ�ó���ر� -->Ĭ��true,�����Ļ���ֻ�����һ�����ڹرգ��Ż��˳�
	a.setQuitOnLastWindowClosed(false);

	UserLogin* userLogin = new UserLogin;
	userLogin->show(); //2023.8.5 �����˵�¼���ڣ�������͸���ģ�����Ϊ���ǻ�û������ʽ��
	
	return a.exec();
}
