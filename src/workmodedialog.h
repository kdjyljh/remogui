#ifndef WORKMODEDIALOG_H
#define WORKMODEDIALOG_H
#include "cameraworkmode.h"
#include "ui_workmode.h"

#include <QDialog>
#include <QObject>
#include <QWidget>

class WorkModeDialog : public QDialog, public CameraWorkMode
{
    Q_OBJECT
public:
    explicit WorkModeDialog(QWidget * parent = nullptr);

private:
    Ui::WorkModeDialog *ui;
};

#endif // WORKMODEDIALOG_H
