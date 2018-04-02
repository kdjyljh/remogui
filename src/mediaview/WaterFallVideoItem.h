//
// Created by jianghualuo on 18-3-17.
//

#ifndef REMO_GUI_WATERFALLVIDEOITEM_H
#define REMO_GUI_WATERFALLVIDEOITEM_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include "VideoStreamControl.h"
#include "ProgressBarSlider.h"
#include <boost/thread.hpp>

class WaterFallVideoItem : public QDialog{
    Q_OBJECT
public:
    explicit WaterFallVideoItem(std::string url, QWidget *parent = nullptr);
    ~WaterFallVideoItem();
    bool isValid() {return valid;}
    int getDuration() {return duration;}

protected:
    virtual void closeEvent(QCloseEvent *event) override ;

private slots:
    void setImage(const QImage &image);
    void readStreamResult(bool result);
    void onPlayButtonClicked();
    void onProgressBarSliderReleased();
    void onProgressBarSliderPressed();
    void onReplayButtonClicked();
    void onProgressBarSliderValueChanged(int value);
    void onVideoTimestampChanged(double time);
    void onVideoFinished();

private:
    void getDurationFromCamera();

private:
    QLayout *mainLayout;
    QLabel *imageLabel;
    QPushButton *playButton;
    QPushButton *replayButton;
    ProgressBarSlider *progressBarSlider;
    QLabel *progressBarLabelTime;
    std::string videoUrl;
    bool valid;
    int duration; //ms
    int durationSeconds; //s
    int progressBarSliderRangeMax;
    VideoStreamControl *videoStream;
    boost::thread videoStreamThread;
    bool progressBarIsDragging;
};


#endif //REMO_GUI_WATERFALLVIDEOITEM_H
