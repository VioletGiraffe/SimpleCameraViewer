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

class CCropFrameHandler : public QObject
{
	Q_OBJECT

public:
	inline void activate() {
		_lastMousePos = {0, 0};
		_active = true;
	}

	inline const bool active() const {
		return _active;
	}

signals:
	void cropFrameEdited(QRect frame);

protected:
	bool eventFilter(QObject * target, QEvent * event) override;

private:
	QPoint _lastMousePos;
	bool   _active = false;
};

class CMainWindow : public QMainWindow
{
public:
	explicit CMainWindow(QWidget *parent = 0);

	~CMainWindow();

private:
	void setupTrayIcon();

// Slots
	// Scans the current image and takes actions (e. g. shows / hides the main window) when the image status changes
	void analyzeFrame();
	// Searches for available cameras and connects to the first one, if any, that matches the name filter
	void updateCamerasList();
	void showCamerasList();

	void hideWindow();
	void switchWindowToFullscreen();
	void showSettingsDialog();

	void applyViewFinderResolutionSettings();

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

	CCropFrameHandler _cropHandler;
};

#endif // CMAINWINDOW_H
