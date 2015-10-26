#ifndef CSETTINGSPAGECAMERA_H
#define CSETTINGSPAGECAMERA_H

#include "settingsui/csettingspage.h"

namespace Ui {
class CSettingsPageCamera;
}

class CSettingsPageCamera : public CSettingsPage
{
	Q_OBJECT

public:
	explicit CSettingsPageCamera(QWidget *parent = 0);
	~CSettingsPageCamera();

	void acceptSettings() override;

private:
	Ui::CSettingsPageCamera *ui;
};

#endif // CSETTINGSPAGECAMERA_H
