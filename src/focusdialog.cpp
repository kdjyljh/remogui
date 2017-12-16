#include "focusdialog.h"
#include <QDialog>

FocusDialog::FocusDialog(QWidget * parent) :
    QDialog(parent),
    ProtocolDataInterfaceImpl(),
    ui(new Ui::Focus)
{
    ui->setupUi(this);
}
