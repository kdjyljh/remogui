//
// Created by jianghualuo on 18-4-21.
//

#ifndef REMO_GUI_MainWorkSpaceWidget_H
#define REMO_GUI_MainWorkSpaceWidget_H

#include <QWidget>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "CameraImageWidget.h"
#include "algorithm/AlgorithmDialog.h"

class MainWorkSpaceWidget : public QWidget {
    Q_OBJECT
public:
    MainWorkSpaceWidget(QWidget *parent = nullptr);
    ~MainWorkSpaceWidget();

private:
    boost::shared_ptr<AlgorithmDialog> algorithmWidget;
    boost::shared_ptr<CameraImageWidget> cameraImageWidget;
};


#endif //REMO_GUI_MAINWORKSPACE_H
