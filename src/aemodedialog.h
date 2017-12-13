#ifndef AEMODE_H
#define AEMODE_H
#include <QDialog>

#include "ui_aemode.h"

class AeModeDialog : public QDialog
{
public:
    explicit AeModeDialog(QWidget * parent);

private:
    Ui::AeMOde * ui;
};

#endif // AEMODE_H
