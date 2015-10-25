#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QCameraViewfinder>
#include <QMainWindow>
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
	void onFrameAnalysisTimerTick();

private:
	Ui::CMainWindow *ui;

	QCameraViewfinder _cameraViewWidget;
	std::shared_ptr<QCamera> _camera;

	QTimer _frameAnalysisTimer;
};

#endif // CMAINWINDOW_H
