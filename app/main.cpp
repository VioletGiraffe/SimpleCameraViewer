#include "cmainwindow.h"
#include "settings/csettings.h"
#include "assert/advanced_assert.h"

DISABLE_COMPILER_WARNINGS
#include <QApplication>
#include <QDebug>
RESTORE_COMPILER_WARNINGS

int main(int argc, char *argv[])
{
	AdvancedAssert::setLoggingFunc([](const char* message){
		qDebug() << message;
	});

	QApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(false);

	app.setOrganizationName("GitHubSoft");
	app.setApplicationName("Simple Camera Viewer");

	CSettings::setApplicationName(app.applicationName());
	CSettings::setOrganizationName(app.organizationName());

	CMainWindow w;

	return app.exec();
}
