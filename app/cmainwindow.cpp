#include "cmainwindow.h"
#include "settingsui/csettingsdialog.h"
#include "settings/csettingspagecamera.h"
#include "settings/settings.h"
#include "settings/csettings.h"

DISABLE_COMPILER_WARNINGS
#include "ui_cmainwindow.h"

#include <QCamera>
#include <QCameraInfo>
#include <QDebug>
#include <QImage>
RESTORE_COMPILER_WARNINGS

CMainWindow::CMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::CMainWindow),
	_cameraViewWidget(this),
	_trayIcon(QIcon(":/icon.ico"), this),
	_camerasListDialog(this)
{
	ui->setupUi(this);

	setCentralWidget(&_cameraViewWidget);
	_cameraViewWidget.show();

	_frameAnalysisTimer.start(333);
	connect(&_frameAnalysisTimer, &QTimer::timeout, this, &CMainWindow::analyzeImage);

	_camerasListUpdateTimer.start(1000);
	connect(&_camerasListUpdateTimer, &QTimer::timeout, this, &CMainWindow::updateCamerasList);

	setupTrayIcon();

	updateCamerasList();
}

CMainWindow::~CMainWindow()
{
	delete ui;
}

void CMainWindow::setupTrayIcon()
{
	connect(_trayIconMenu.addAction("Show cameras list..."), &QAction::triggered, this, &CMainWindow::showCamerasList);
	_trayIconMenu.addSeparator();
	connect(_trayIconMenu.addAction("Settings..."), &QAction::triggered, this, &CMainWindow::showSettingsDialog);
	_trayIconMenu.addSeparator();
	connect(_trayIconMenu.addAction("Exit"), &QAction::triggered, &QApplication::quit);

	_trayIcon.setContextMenu(&_trayIconMenu);
	_trayIcon.show();
}

const int sampleSquareSize = 20;

// Scans the current image and takes actions (e. g. shows / hides the main window) when the image status changes
void CMainWindow::analyzeImage()
{
	const QImage frame = _cameraViewWidget.width() < sampleSquareSize || _cameraViewWidget.height() < sampleSquareSize ? 
		_cameraViewWidget.grab().toImage() :
		_cameraViewWidget.grab().copy(QRect(QPoint(_cameraViewWidget.width()/2 - sampleSquareSize/2, _cameraViewWidget.height()/2 - sampleSquareSize/2), QSize(sampleSquareSize, sampleSquareSize))).toImage();

	const int w = frame.width(), h = frame.height();
	for (int y = 0; y < h; ++y)
		for (int x = 0; x < w; ++x)
		{
			const QRgb pixel = frame.pixel(x, y);
			if ((pixel & 0x00F8F8F8u) != 0) // Letting the last 3 bits of each color component be non-zero, since it may happen, for whatever reason
			{
				// Valid image detected!
				switchWindowToFullscreen();
				return;
			}
		}

	// Valid image NOT detected
	hideWindow();
}

// Searches for available cameras and connects to the first one, if any, that matches the name filter
void CMainWindow::updateCamerasList()
{
	const auto cameras = QCameraInfo::availableCameras();
	QStringList camerasNames;
	int currentCamIndex = -1;
	for (int i = 0; i < cameras.size(); ++i)
	{
		const auto& cameraInfo = cameras[i];
		const QString cameraName = cameraInfo.deviceName(), nameFilter = CSettings().value(SETTINGS_KEY_CAMERA_NAME_FILTER).toString();
		camerasNames.push_back(cameraName);
		if (!cameraInfo.isNull() && (nameFilter.isEmpty() || cameraName.toLower().contains(nameFilter.toLower())))
		{
			if (!_camera)
			{
				_currentCameraDeviceName = cameraName;
				_camera = std::make_shared<QCamera>(cameraInfo);
				auto settings = _camera->viewfinderSettings();
				settings.setResolution(QSize(CSettings().value(SETTINGS_KEY_IMAGE_WIDTH, 720).toUInt(), CSettings().value(SETTINGS_KEY_IMAGE_HEIGHT, 576).toUInt()));
				_camera->setViewfinderSettings(settings);
				_camera->setViewfinder(&_cameraViewWidget);
				_camera->start();
			}
		}

		if (_currentCameraDeviceName == cameraName)
			currentCamIndex = i;
	}

	// TODO: find a better way to do this
	if (!_currentCameraDeviceName.isEmpty() && _camera && currentCamIndex < 0) // The current camera no longer exists
	{
		_camera->stop();
		_camera.reset();
	}

	_camerasListDialog.listUpdated(camerasNames, currentCamIndex);
}

void CMainWindow::showCamerasList()
{
	_camerasListDialog.exec();
}

void CMainWindow::hideWindow()
{
	if ((windowState() & Qt::WindowFullScreen) != 0) // Not hidden yet
	{
		_cameraViewWidget.setMinimumSize(QSize(sampleSquareSize, sampleSquareSize));
		_cameraViewWidget.resize(_cameraViewWidget.minimumSize());
		setWindowState(windowState() & ~Qt::WindowFullScreen);
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
	CSettingsDialog settingsDialog(this);
	settingsDialog.addSettingsPage(new CSettingsPageCamera);

	_trayIconMenu.setEnabled(false);
	settingsDialog.exec();
	_trayIconMenu.setEnabled(true);
}
