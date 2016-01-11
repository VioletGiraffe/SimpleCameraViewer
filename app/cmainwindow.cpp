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
#include <QMouseEvent>
#include <QPainter>
RESTORE_COMPILER_WARNINGS

CMainWindow::CMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::CMainWindow),
	_cameraViewWidget(this),
	_trayIcon(QIcon(":/icon.ico"), this),
	_camerasListDialog(this)
{
	ui->setupUi(this);

	_cameraViewWidget.setContextMenuPolicy(Qt::CustomContextMenu);
	setCentralWidget(&_cameraViewWidget);
	_cameraViewWidget.show();

	_frameAnalysisTimer.start(333);
	connect(&_frameAnalysisTimer, &QTimer::timeout, this, &CMainWindow::analyzeImage);

	_camerasListUpdateTimer.start(1000);
	connect(&_camerasListUpdateTimer, &QTimer::timeout, this, &CMainWindow::updateCamerasList);

	setupTrayIcon();

	updateCamerasList();

	connect(&_cameraViewWidget, &QWidget::customContextMenuRequested, [this](const QPoint& point){
		QMenu menu(this);
		const QAction * const cropImageAction = menu.addAction("Crop image");
		if (menu.exec(_cameraViewWidget.mapToGlobal(point)) == cropImageAction)
		{
			_cropHandler.activate();
			_cameraViewWidget.update(); // To render the hint text
		}
	});

	_cameraViewWidget.installEventFilter(&_cropHandler);
	connect(&_cropHandler, &CCropFrameHandler::cropFrameEdited, [this](const QRect frame) {

		CSettings().setValue(SETTINGS_KEY_IMAGE_WIDTH, frame.width());
		CSettings().setValue(SETTINGS_KEY_IMAGE_HEIGHT, frame.height());
		applyViewFinderResolutionSettings();
	});

	showNormal();
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
			if ((pixel & 0x00F0F0F0u) != 0) // Letting the last 4 bits of each color component be non-zero, since it may happen, for whatever reason
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
				applyViewFinderResolutionSettings();
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
// 	if ((windowState() & Qt::WindowFullScreen) != 0) // Not hidden yet
// 	{
// 		_cameraViewWidget.setMinimumSize(QSize(sampleSquareSize, sampleSquareSize));
// 		_cameraViewWidget.resize(_cameraViewWidget.minimumSize());
// 		setWindowState(windowState() & ~Qt::WindowFullScreen);
// 		hide();
// 	}
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

void CMainWindow::applyViewFinderResolutionSettings()
{
	if (!_camera)
		return;

	auto settings = _camera->viewfinderSettings();
	settings.setResolution(QSize(CSettings().value(SETTINGS_KEY_IMAGE_WIDTH, 720).toUInt(), CSettings().value(SETTINGS_KEY_IMAGE_HEIGHT, 576).toUInt()));
	_camera->setViewfinderSettings(settings);
}

bool CCropFrameHandler::eventFilter(QObject * target, QEvent * event)
{
	QWidget * targetWidget = dynamic_cast<QWidget*>(target);
	if (!targetWidget)
		return false;

	switch (event->type())
	{
	case QEvent::MouseButtonPress:
	{
		QMouseEvent * mouseEvent = dynamic_cast<QMouseEvent*>(event);
		if (_active && mouseEvent && mouseEvent->button() == Qt::LeftButton)
		{
			_lastMousePos = mouseEvent->pos();
			targetWidget->update();
		}
		else
			_active = false; // Just in case
		break;
	}
	case QEvent::MouseMove:
	{
		QMouseEvent * mouseEvent = dynamic_cast<QMouseEvent*>(event);
		if (_active && mouseEvent)
		{
			_lastMousePos = mouseEvent->pos();
			targetWidget->update();
		}
		break;
	}
	case QEvent::MouseButtonRelease:
	{
		if (_active)
		{
			targetWidget->update();
			_active = false;
			_lastMousePos = {0, 0};

			QMouseEvent * mouseEvent = dynamic_cast<QMouseEvent*>(event);
			if (mouseEvent)
				emit cropFrameEdited(QRect(0, 0, mouseEvent->pos().x(), mouseEvent->pos().y()));
		}
		break;
	}
	case QEvent::Paint:
	{
		if (_active)
		{
			target->event(event);
			QPainter painter(targetWidget);

			const QRect textRect = QRect(0, 0, targetWidget->width(), 50);
			const QString hintText = tr("Hold LMB and drag to select the crop area.\nRelease the button to apply.");
			const float factor = std::min(
				textRect.width() / (float)painter.fontMetrics().width(hintText),
				textRect.height() / ((float)painter.fontMetrics().height() * (hintText.count('\n') + 1)) // Text height = number of text lines * line height
				);
			QFont font = painter.font();
			font.setPointSizeF(font.pointSizeF() * factor);
			painter.setFont(font);

			QPen pen = painter.pen();
			painter.setPen(Qt::green);
			painter.drawText(textRect, Qt::AlignCenter, hintText);
			painter.drawRect(0, 0, _lastMousePos.x(), _lastMousePos.y());
		}

		break;
	}
	default:
		break;
	}

	return false;
}
