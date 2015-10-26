#include "csettingspagegeneral.h"
#include "settings.h"

DISABLE_COMPILER_WARNINGS
#include "ui_csettingspagegeneral.h"
RESTORE_COMPILER_WARNINGS

CSettingsPageGeneral::CSettingsPageGeneral(QWidget *parent) :
	CSettingsPage(parent),
	ui(new Ui::CSettingsPageGeneral)
{
	ui->setupUi(this);
}

CSettingsPageGeneral::~CSettingsPageGeneral()
{
	delete ui;
}

void CSettingsPageGeneral::acceptSettings()
{

}
