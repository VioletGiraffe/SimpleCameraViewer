#include "csettingspagecamera.h"

#include "settings.h"
#include "settings/csettings.h"

DISABLE_COMPILER_WARNINGS
#include "ui_csettingspagecamera.h"
RESTORE_COMPILER_WARNINGS

CSettingsPageCamera::CSettingsPageCamera(QWidget *parent) :
	CSettingsPage(parent),
	ui(new Ui::CSettingsPageCamera)
{
	ui->setupUi(this);

	CSettings s;
	ui->_leCameraFilter->setText(s.value(SETTINGS_KEY_CAMERA_NAME_FILTER).toString());
	ui->_sbImageWidth->setValue(s.value(SETTINGS_KEY_IMAGE_WIDTH, 720).toInt());
	ui->_sbImageHeight->setValue(s.value(SETTINGS_KEY_IMAGE_HEIGHT, 576).toInt());
	ui->_sbPixelValueThreshold->setValue(s.value(SETTINGS_KEY_IMAGE_PIXEL_VALUE_THRESHOLD, 10).toInt());
}

CSettingsPageCamera::~CSettingsPageCamera()
{
	delete ui;
}

void CSettingsPageCamera::acceptSettings()
{
	CSettings s;
	s.setValue(SETTINGS_KEY_CAMERA_NAME_FILTER, ui->_leCameraFilter->text());
	s.setValue(SETTINGS_KEY_IMAGE_WIDTH, ui->_sbImageWidth->value());
	s.setValue(SETTINGS_KEY_IMAGE_HEIGHT, ui->_sbImageHeight->value());
	s.setValue(SETTINGS_KEY_IMAGE_PIXEL_VALUE_THRESHOLD, ui->_sbPixelValueThreshold->value());
}
