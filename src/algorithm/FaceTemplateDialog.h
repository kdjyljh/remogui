//
// Created by jianghualuo on 18-4-12.
//

#ifndef REMO_GUI_FACETEMPLATE_H
#define REMO_GUI_FACETEMPLATE_H

#include <QScrollArea>
#include <QPushButton>
#include <list>
#include "FaceTemplateItem.h"

class FaceTemplateItem;

class FaceTemplateDialog : public QScrollArea {
    Q_OBJECT
public:
    explicit FaceTemplateDialog(QWidget *parent = nullptr);
    ~FaceTemplateDialog();
    void updateFaceTemplateUi(const AlgoParamMsg &msg);

    static void syncFaceTempHandle(const AlgoParamMsg &msg);

protected:

private slots:
    void pushButtonCreateClicked();

private:
    void refreshItem();

private:
    QWidget *contentWidget;
    QWidget *faceItemWidget;
    QPushButton *pushButtonCreate;
    const int faceTemplateItemHeight;
    std::list<FaceTemplateItem*> faceItemUiList;
    std::list<AlgoParam::FaceTemplate> faceItemList;
};


#endif //REMO_GUI_FACETEMPLATE_H
