#include "cmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CMainWindow w;

	return a.exec();
}
