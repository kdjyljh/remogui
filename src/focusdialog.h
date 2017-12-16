#ifndef FOCUS_H
#define FOCUS_H
#include "protocoldataInterfaceimpl.h"
#include "ui_focus.h"
#include <QDialog>

class FocusDialog : public QDialog, public ProtocolDataInterfaceImpl
{
public:
    explicit FocusDialog(QWidget * parent);

private:
    Ui::Focus * ui;
};

#endif // FOCUS_H
