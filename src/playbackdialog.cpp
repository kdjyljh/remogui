#include "playbackdialog.h"
#include <QGridLayout>
#include <QPicture>
#include <QLabel>
#include <QPixmapCache>
PlayBackDialog::PlayBackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ThumbnailViewer),
    thumbnailImageList(new QListWidget)
{
    ui->setupUi(this);
    QPixmap *pix = new QPixmap(100, 100);
    pix->load("/home/jianghualuo/work/data/images/cloud.jpg");
    *pix = pix->scaled(100, 100);
    QGridLayout *mainLayout = new QGridLayout;
    setLayout(mainLayout);
    int numPixRaw = 3;
    int numPIxColumn = 3;
    for (int i = 0; i < numPixRaw; ++i)
        for (int j = 0; j < numPIxColumn; ++j) {
            QLabel *label = new QLabel;
            label->setPixmap(*pix);
            mainLayout->addWidget(label, i, j);
        }
}
