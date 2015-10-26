#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QDebug>
#include <QImage>

CMainWindow::CMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::CMainWindow),
	_cameraViewWidget(this),
	_trayIcon(QIcon(":/icon.ico"), this)
{
	ui->setupUi(this);

	setCentralWidget(&_cameraViewWidget);
	_cameraViewWidget.show();

	const auto cameras = QCameraInfo::availableCameras();
	qDebug() << "Cameras found:\n" << cameras;
	for (const auto& cameraInfo: cameras)
		if (!cameraInfo.isNull())
		{
			qDebug() << cameraInfo.deviceName();
			_camera = std::make_shared<QCamera>(cameraInfo);
			_camera->setViewfinder(&_cameraViewWidget);
			_camera->start();

			break;
		}

	_frameAnalysisTimer.start(333);
	connect(&_frameAnalysisTimer, &QTimer::timeout, this, &CMainWindow::analyzeImage);

	setupTrayIcon();
}

CMainWindow::~CMainWindow()
{
	delete ui;
}

void CMainWindow::setupTrayIcon()
{
	QAction * settingsAction = _trayIconMenu.addAction("Settings...");
	connect(settingsAction, &QAction::triggered, this, &CMainWindow::showSettingsDialog);

	_trayIconMenu.addSeparator();

	QAction * exitAction = _trayIconMenu.addAction("Exit");
	connect(exitAction, &QAction::triggered, &QApplication::quit);

	_trayIcon.setContextMenu(&_trayIconMenu);
	_trayIcon.show();
}

const int sampleSquareSize = 20;

// Scans the current image and takes actions (e. g. shows / hides the main window) when the image status changes
void CMainWindow::analyzeImage()
{
	const auto frame = _cameraViewWidget.grab().copy(QRect(QPoint(_cameraViewWidget.width()/2 - sampleSquareSize/2, _cameraViewWidget.height()/2 - sampleSquareSize/2), QSize(sampleSquareSize, sampleSquareSize))).toImage();
	for (int y = 0; y < sampleSquareSize; ++y)
		for (int x = 0; x < sampleSquareSize; ++x)
			if ((frame.pixel(x, y) & 0x00FFFFFFu) != 0)
			{
				// Valid image detected!
				switchWindowToFullscreen();
				return;
			}

	// Valid image NOT detected
	hideWindow();
}

void CMainWindow::hideWindow()
{
	if ((windowState() & Qt::WindowFullScreen) != 0) // Not hidden yet
	{
		_cameraViewWidget.setMinimumSize(QSize(sampleSquareSize, sampleSquareSize));
		setMinimumSize(_cameraViewWidget.minimumSize());
		resize(_cameraViewWidget.minimumSize());
		hide();
	}
}

void CMainWindow::switchWindowToFullscreen()
{
	if ((windowState() & Qt::WindowFullScreen) == 0) // The window is not full screen yet
	{
		showNormal(); // showFullScreen doesn't work properly if the window was minimized
		showFullScreen();
	}
}

void CMainWindow::showSettingsDialog()
{

}
