#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QCameraViewfinder>
#include <QMainWindow>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTimer>

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

	void hideWindow();
	void switchWindowToFullscreen();
	void showSettingsDialog();

private:
	Ui::CMainWindow *ui;

	QCameraViewfinder _cameraViewWidget;
	std::shared_ptr<QCamera> _camera;
	// Timer for checking the image status periodically
	QTimer _frameAnalysisTimer;
	QSystemTrayIcon _trayIcon;
	QMenu           _trayIconMenu;
};

#endif // CMAINWINDOW_H
