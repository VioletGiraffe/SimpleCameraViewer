#ifndef CCAMERASLIST_H
#define CCAMERASLIST_H

#include "compiler/compiler_warnings_control.h"

DISABLE_COMPILER_WARNINGS
#include <QDialog>
RESTORE_COMPILER_WARNINGS

namespace Ui {
class CCamerasList;
}

class CCamerasList : public QDialog
{
	Q_OBJECT

public:
	explicit CCamerasList(QWidget *parent = 0);
	~CCamerasList();

	void listUpdated(const QStringList& camerasList, int currentCameraIndex = -1);

private:
	Ui::CCamerasList *ui;
};

#endif // CCAMERASLIST_H
