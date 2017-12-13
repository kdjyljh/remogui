#include "workmodedialog.h"
#include "itemdatadef.h"
#include <QFormLayout>

WorkModeDialog::WorkModeDialog(QWidget * parent) :
    QDialog(parent),
    CameraWorkMode(),
    ui(new Ui::WorkModeDialog)
{
    ui->setupUi(this);
//    iniItemData();
    QFormLayout *formLayout_photoTab = new QFormLayout(this);
    ui->tab_Photo->setLayout(formLayout_photoTab);
    for (auto it : itemData) {
//        formLayout_photoTab->addRow(it.second.ShowStr, (QWidget*)it.first);
    }
}
