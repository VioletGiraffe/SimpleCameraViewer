#ifndef CSETTINGSPAGEGENERAL_H
#define CSETTINGSPAGEGENERAL_H

#include "settingsui/csettingspage.h"

namespace Ui {
class CSettingsPageGeneral;
}

class CSettingsPageGeneral : public CSettingsPage
{
	Q_OBJECT

public:
	explicit CSettingsPageGeneral(QWidget *parent = 0);
	~CSettingsPageGeneral();

	void acceptSettings() override;

private:
	Ui::CSettingsPageGeneral *ui;
};

#endif // CSETTINGSPAGEGENERAL_H
