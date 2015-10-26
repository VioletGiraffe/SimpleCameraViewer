#include "csettingsdialog.h"
#include "ui_csettingsdialog.h"

CSettingsDialog::CSettingsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CSettingsDialog)
{
	ui->setupUi(this);
}

CSettingsDialog::~CSettingsDialog()
{
	delete ui;
}
