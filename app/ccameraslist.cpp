#include "ccameraslist.h"
#include "ui_ccameraslist.h"

DISABLE_COMPILER_WARNINGS
#include <QStringBuilder>
RESTORE_COMPILER_WARNINGS

CCamerasList::CCamerasList(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CCamerasList)
{
	ui->setupUi(this);
}

CCamerasList::~CCamerasList()
{
	delete ui;
}

void CCamerasList::listUpdated(const QStringList& camerasList, int currentCameraIndex)
{
	ui->_list->clear();

	QString text = "<ul>";
	for (int i = 0; i < camerasList.size(); ++i)
		text.append("<li>" % (i != currentCameraIndex ? camerasList[i] : ("<b>" % camerasList[i] % "</b>")) % "</li>");
	text.append("</ul>");
	ui->_list->setHtml(text);
}
