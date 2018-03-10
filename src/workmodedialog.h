#ifndef WORKMODEDIALOG_H
#define WORKMODEDIALOG_H
#include "protocoldataInterfaceimpl.h"
#include "ui_workmode.h"

#include <QDialog>
#include <QObject>
#include <QWidget>

class WorkModeDialog : public QDialog, public ProtocolDataInterfaceImpl
{
    Q_OBJECT
public:
    explicit WorkModeDialog(QWidget * parent = nullptr);

private:
    Ui::WorkModeDialog *ui;
};

#endif // WORKMODEDIALOG_H
