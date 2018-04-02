//
// Created by jianghualuo on 18-3-17.
//

#include "WaterFallVideoItem.h"
#include "HttpProxy.h"
#include <QMessageBox>
#include <QDateTime>
#include <QMouseEvent>
#include <QDebug>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <glog/logging.h>
#include <cmath>

//rtsp://192.168.0.1/livestream/file&path=/app/sd/DCIM/100HISDV/NORM0007.LRV
// /home/jianghualuo/work/data/images/cloud.jpg
//http://192.168.0.1/cgi-bin/hi3510/t_remo.cgi?-type=Demuxer&-cmd=pause&-param=0
//http://192.168.0.1/cgi-bin/hi3510/t_remo.cgi?-type=Demuxer&-cmd=seek&-param=5
//http://192.168.0.1/cgi-bin/hi3510/t_remo.cgi?-type=GetFileDuration&-cmd=/app/sd/DCIM/100HISDV/NORM0011.LRV&-param=0

const int DEFAULT_WIDTH = 1000;
const int DEFAULT_HEIGHT = 800;
WaterFallVideoItem::WaterFallVideoItem(std::string url, QWidget *parent) :
        QDialog(parent),
        mainLayout(new QVBoxLayout),
        imageLabel(new QLabel),
        playButton(new QPushButton),
        replayButton(new QPushButton),
        progressBarSlider(new ProgressBarSlider),
        progressBarLabelTime(new QLabel("00:00:00/00:00:00")),
        valid(false),
        progressBarIsDragging(false),
        videoStream(new VideoStreamControl),
        progressBarSliderRangeMax(100),
        duration(0),
        videoUrl(url) {
    resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    setLayout(mainLayout);
    mainLayout->addWidget(imageLabel);
    imageLabel->resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    QWidget *progressBarWidget = new QWidget;
    progressBarWidget->setLayout(new QHBoxLayout);
    progressBarWidget->layout()->addWidget(progressBarSlider);
    progressBarWidget->layout()->addWidget(progressBarLabelTime);
    mainLayout->addWidget(progressBarWidget);
    mainLayout->addWidget(playButton);
    mainLayout->addWidget(replayButton);
    playButton->setText(QString::fromLocal8Bit("播放/暂停"));
    replayButton->setText(QString::fromLocal8Bit("重播"));
    progressBarSlider->setRange(0, progressBarSliderRangeMax);
    progressBarSlider->setOrientation(Qt::Horizontal);

    getDurationFromCamera();

    std::string baseUrl = "rtsp://192.168.0.1/livestream/file&path=/app/";
    int pos = videoUrl.find('.');
    std::string path = videoUrl.substr(0, pos);
    videoStream->setUrl(baseUrl + path + ".LRV");

    videoStreamThread = boost::thread(&VideoStreamControl::play, videoStream);

    connect(videoStream, SIGNAL(imageGot(const QImage &)), this, SLOT(setImage(const QImage &)));
    connect(videoStream, SIGNAL(readStreamDone(bool)), this, SLOT(readStreamResult(bool)));
    connect(playButton, SIGNAL(clicked()), this, SLOT(onPlayButtonClicked()));
    connect(progressBarSlider, SIGNAL(sliderReleased()), this, SLOT(onProgressBarSliderReleased()));
    connect(progressBarSlider, SIGNAL(sliderPressed()), this, SLOT(onProgressBarSliderPressed()));
    connect(replayButton, SIGNAL(clicked()), this, SLOT(onReplayButtonClicked()));
    connect(progressBarSlider, SIGNAL(valueChanged(int)), this, SLOT(onProgressBarSliderValueChanged(int)));
    connect(videoStream, SIGNAL(videoTimestampChanged(double)), this, SLOT(onVideoTimestampChanged(double)));
    connect(videoStream, SIGNAL(videoFinished()), this, SLOT(onVideoFinished()));

    setWindowTitle(QString::fromStdString(videoUrl));

    if (duration <= 100) {
        valid = false;
    }
}

WaterFallVideoItem::~WaterFallVideoItem() {
    delete mainLayout;
    delete imageLabel;
    delete playButton;
    delete progressBarSlider;
    videoStreamThread.interrupt();
    videoStreamThread.join();
    videoStream->closeReadStreamThread();
    delete videoStream;
}

void WaterFallVideoItem::setImage(const QImage &image) {
    QPixmap pix = QPixmap::fromImage(image);
    pix = pix.scaledToWidth(DEFAULT_WIDTH);
    imageLabel->setPixmap(pix);
    resize(pix.size());
//
//    if (!pix.isNull())
//        setFixedSize(pix.size());
}

void WaterFallVideoItem::readStreamResult(bool result) {
    valid = result;
    if (!valid) {
        QMessageBox::warning(nullptr, "网络错误", "无视频流", QMessageBox::Ok);
    }
}

void WaterFallVideoItem::onPlayButtonClicked() {
    std::string url = "http://192.168.0.1/cgi-bin/hi3510/t_remo.cgi?-type=Demuxer&-cmd=pause&-param=0";
    std::vector<uint8_t> buff;
    while (true) {
        HttpProxy::getRequest(url, buff);
        std::string ret;
        ret.assign(buff.begin(), buff.end());
        if (0 == ret.find("Success")) {
            break;
        }
    }
}

void WaterFallVideoItem::onProgressBarSliderReleased() {
    int time = ceil(progressBarSlider->value() / (float)progressBarSliderRangeMax * duration / 1000);
    std::string url = "http://192.168.0.1/cgi-bin/hi3510/t_remo.cgi?-type=Demuxer&-cmd=seek&-param=" +
            boost::lexical_cast<std::string>(time);
    std::vector<uint8_t> buff;
    while (true) {
        HttpProxy::getRequest(url, buff);
        std::string ret;
        ret.assign(buff.begin(), buff.end());
        if (0 == ret.find("Success")) {
            break;
        }
    }
}

void WaterFallVideoItem::getDurationFromCamera() {
    std::string path = videoUrl.substr(0, videoUrl.find('.')) + ".LRV";
    std::string url = "http://192.168.0.1/cgi-bin/hi3510/t_remo.cgi?-type=GetFileDuration&-cmd=/app/" + path + "&-param=0";
    std::vector<uint8_t> buff;
    if (HttpProxy::getRequest(url, buff)) {
        std::string ret;
        ret.assign(buff.begin(), buff.end());
        int pos1 = ret.find("Success");
        int pos2 = ret.find("Duration");
        if (std::string::npos != pos1 && std::string::npos != pos2) {
            try {
                int pos = ret.find("=") + 1;
                std::string timeStr = ret.substr(pos, ret.size() - pos);
                boost::trim(timeStr);
                int time = boost::lexical_cast<int>(timeStr);
                duration = time;
                valid = true;
                durationSeconds = ceil(duration / 1000); //向上取整
                progressBarSliderRangeMax = duration / 10; //10ms进度条步进一次
                progressBarSlider->setRange(0, progressBarSliderRangeMax);
                QString durationStr = QDateTime::fromTime_t(durationSeconds).toUTC().toString("h:m:s");
                progressBarLabelTime->setText(durationStr);
                return;
            }
            catch (std::exception &e) {
                LOG(INFO) << "WaterFallVideoItem::getDuration " << e.what();
                valid = false;
            }
        }
    }
    valid = false;
}

void WaterFallVideoItem::closeEvent(QCloseEvent *event) {

}

void WaterFallVideoItem::onReplayButtonClicked() {
    videoStream->readStream();
}

void WaterFallVideoItem::onProgressBarSliderValueChanged(int value) {
    int time = ceil(value / (float)progressBarSliderRangeMax * duration / 1000);
//    LOG(INFO) << "onProgressBarSliderValueChanged:" << value << ":" << progressBarSliderRangeMax<< ":" << duration << ":" << time;
    QString timeStr = QDateTime::fromTime_t(time).toUTC().toString("h:m:s");
    QString durationStr = QDateTime::fromTime_t(durationSeconds).toUTC().toString("h:m:s");
    timeStr += QString("/") + durationStr;
    progressBarLabelTime->setText(timeStr);
}

void WaterFallVideoItem::onVideoTimestampChanged(double time) {
    int value = time * 1000 / duration * progressBarSliderRangeMax;
//    LOG(INFO) << "WaterFallVideoItem::onVideoTimestampChanged set value" << value;
    progressBarSlider->setValueWithDragging(value);
}

void WaterFallVideoItem::onProgressBarSliderPressed() {
}

void WaterFallVideoItem::onVideoFinished() {
    progressBarSlider->setValue(progressBarSliderRangeMax);
}
