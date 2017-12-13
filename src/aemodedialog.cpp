#include "aemodedialog.h"


AeModeDialog::AeModeDialog(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::AeMOde)
{
    ui->setupUi(this);
    setFixedSize(300, 200);
}
