#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include "compiler/compiler_warnings_control.h"
#include "ccameraslist.h"

DISABLE_COMPILER_WARNINGS
#include <QCameraViewfinder>
#include <QMainWindow>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTimer>
RESTORE_COMPILER_WARNINGS

#include <memory>

namespace Ui {
class CMainWindow;
}

class QCamera;
class QCameraImageCapture;

class CMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit CMainWindow(QWidget *parent = 0);

	~CMainWindow();

private:
	void setupTrayIcon();

// Slots
	// Scans the current image and takes actions (e. g. shows / hides the main window) when the image status changes
	void analyzeImage();
	// Searches for available cameras and connects to the first one, if any, that matches the name filter
	void updateCamerasList();
	void showCamerasList();

	void hideWindow();
	void switchWindowToFullscreen();
	void showSettingsDialog();

private:
	Ui::CMainWindow *ui;

	QCameraViewfinder _cameraViewWidget;
	std::shared_ptr<QCamera> _camera;
	QString _currentCameraDeviceName;
	// Timer for checking the image status periodically
	QTimer _frameAnalysisTimer;
	QSystemTrayIcon _trayIcon;
	QMenu           _trayIconMenu;
	// Timer for updating the list of cameras
	QTimer _camerasListUpdateTimer;
	CCamerasList _camerasListDialog;
};

#endif // CMAINWINDOW_H
