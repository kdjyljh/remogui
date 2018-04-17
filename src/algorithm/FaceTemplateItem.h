//
// Created by jianghualuo on 18-4-12.
//

#ifndef REMO_GUI_FACETEMPLATEITEM_H
#define REMO_GUI_FACETEMPLATEITEM_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCheckBox>
#include "AlgorithmImageWidget.h"
#include "AlgorithmDialog.h"

class FaceTemplateItem : public QWidget {
    Q_OBJECT
public:
    FaceTemplateItem(AlgoParam::FaceTemplate ft, QWidget *parent = nullptr);
    FaceTemplateItem() {}
    ~FaceTemplateItem();
    void setPushButtonUpEnable(bool enable) {pushButtonUp->setEnabled(enable);}
    void setPushButtonDownEnable(bool enable) {pushButtonDown->setEnabled(enable);}

public:
    bool operator<(const FaceTemplateItem& obj)
    {
        return faceData.id_inlib() < obj.faceData.id_inlib(); //从小到大排序
    }

public:
    AlgoParam::FaceTemplate faceData;

private slots:
    void deleteFaceTemp_clicked();
    void pushButtonChoseFaceTemp_clicked();
    void lineEditFaceName_returnPressed();
    void checkBoxIsMaster_clicked(bool checked);
    void pushButtonUpDown_clicked();

private:
    AlgorithmImageWidget *imageWidget;
    QHBoxLayout *mainLayout;
    QPushButton *pushButtonUp;
    QPushButton *pushButtonDown;
    QPushButton *pushButtonChoseFaceTemp;
    QPushButton *pushButtonDeleteFaceTemp;
    QLineEdit *lineEditFaceName;
    QCheckBox *checkBoxIsMaster;
};

namespace std {
    template<> struct greater<FaceTemplateItem*> {
        bool operator()(const FaceTemplateItem* a1, const FaceTemplateItem* a2) const
        {
            return a1->faceData.id_inlib() < a2->faceData.id_inlib();
        }
    };
}
#endif //REMO_GUI_FACETEMPLATEITEM_H
