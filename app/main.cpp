#include "cmainwindow.h"
#include "settings/csettings.h"

DISABLE_COMPILER_WARNINGS
#include <QApplication>
RESTORE_COMPILER_WARNINGS

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(false);

	app.setOrganizationName("GitHubSoft");
	app.setApplicationName("Simple Camera Viewer");

	CSettings::setApplicationName(app.applicationName());
	CSettings::setOrganizationName(app.organizationName());

	CMainWindow w;

	return app.exec();
}
