//
// Created by jianghualuo on 18-4-21.
//

#ifndef REMO_GUI_MainWorkSpaceWidget_H
#define REMO_GUI_MainWorkSpaceWidget_H

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "MainImageWidget.h"

#include <QWidget>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "MainImageWidget.h"
#include "AlgorithmDialog.h"

class MainWorkSpaceWidget : public QWidget {
Q_OBJECT
public:
    MainWorkSpaceWidget(QWidget *parent = nullptr);
    ~MainWorkSpaceWidget();
    bool init();
    boost::shared_ptr<MainImageWidget> getImageWidget() {return imageWidget;}

private:
    boost::shared_ptr<AlgorithmDialog> algorithmWidget;
    boost::shared_ptr<MainImageWidget> imageWidget;
    std::string imageSource;
};


#endif //REMO_GUI_MAINWORKSPACE_H
