#ifndef PLAYBACKDIALOG_H
#define PLAYBACKDIALOG_H
#include <QDialog>
#include <QListWidget>
#include "ui_thumbnailviewer.h"

class PlayBackDialog : public QDialog, Ui::ThumbnailViewer
{
    Q_OBJECT
public:
    PlayBackDialog(QWidget *parent = nullptr);

private:
    Ui::ThumbnailViewer *ui;
    QListWidget *thumbnailImageList;
};

#endif // PLAYBACKDIALOG_H
