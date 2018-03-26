#include "waterfallwidget.h"
#include <QPainter>
#include <glog/logging.h>
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include "HttpProxy.h"

//获取远程文件列表  http://192.168.0.1/cgi-bin/hi3510/getfilelist.cgi?-start=0&-end=3
//获取指定文件缩略图 http://192.168.0.1//app/sd/DCIM/100HISDV/NORM0002.THM

WaterFallWidget::WaterFallWidget(QWidget *parent) :
        QWidget(parent),
        kHSpace(5),
        kVSpace(5),
        kLeftSpace(1),
        kRightSpace(10),
        kTopSpace(2),
        kBottomSpace(15),
        kItemInterHSpace(10),
        kItemInterVSpace(5),
        valid(false),
        thumbnailUrlIndex(0) {
//    reInit();
}

void WaterFallWidget::getImages(int num) {
    int itemWidth = getItemWidth();
    int lastIndex = thumbnailUrlIndex;
    if (thumbnailUrlIndex != 0)
        ++lastIndex; //如果不是第一次获取图片，不重复获取上次已经获取过的最后一张图片
    for (int i = lastIndex; i < lastIndex + num; ++i) {
        thumbnailUrlIndex = i;

        if (thumbnailUrlIndex >= thumbnailUrlVec.size()) {
            LOG(INFO) << "WaterFallWidget::getImages get all images!!!!!!!!!!!!!!!!!!!";
            emit allImageGot();
            break;
        }

        boost::shared_ptr<WaterFallItem> item(new WaterFallItem(thumbnailUrlVec[thumbnailUrlIndex], this));
        if (!item->loadImage()) {
            item.reset();
            ++num; //获取图片失败，多读一张图片
            continue;
        }

        int minHeightIndex = 0;
        int minHeight = 999999;
        for (int j = 0; j < kItemColumnSize; ++j) {
            if (minHeight > curColumnHeights[j]) {
                minHeight = curColumnHeights[j];
                minHeightIndex = j;
            }
        }

        QSize s = item->getPixSize();
        int itemHeight = itemWidth / (double)item->getPixSize().width() * item->getPixSize().height();
        int posX = kLeftSpace + minHeightIndex * kItemInterHSpace + minHeightIndex * itemWidth;
        int posY = kItemInterVSpace + minHeight;
        curColumnHeights[minHeightIndex] = posY + itemHeight;
        item->setGeometry(posX, posY, itemWidth, itemHeight);
        item->setVisible(true);

        itemListColumn[minHeightIndex].push_back(item);
        LOG(INFO) << "WaterFallWidget::getImages push back (x,y):" << posX << " " << posY;
        LOG(INFO) << "WaterFallWidget::getImages push back url:" << item->getImageUrl() << " index:" << thumbnailUrlIndex;
    }

    int maxHeight = *std::max_element(curColumnHeights, curColumnHeights + kItemColumnSize);
    setFixedHeight(maxHeight + kBottomSpace);
//    setFixedHeightSafely();
}

void WaterFallWidget::paintEvent(QPaintEvent *event) {
}

int WaterFallWidget::getItemWidth() {
    QSize s = size();
    return (width() - kLeftSpace - kRightSpace - kItemInterHSpace * (kItemColumnSize - 1)) / (double)kItemColumnSize;
}

void WaterFallWidget::resizeEvent(QResizeEvent *event) {
    int columnHeights[kItemColumnSize];
    for (int i = 0; i < kItemColumnSize; ++i) {
        columnHeights[i] = kTopSpace;
    }

    std::vector<std::vector<boost::shared_ptr<WaterFallItem> >::iterator> its;
    for (int i = 0; i < kItemColumnSize; ++i) {
        its.push_back(itemListColumn[i].begin());
    }

    int loadColumnCount = 0;
    while (loadColumnCount < 4) {
        for (int i = 0; i < kItemColumnSize; ++i) {
            if (its[i] != itemListColumn[i].end()) {
                auto it = *its[i];
                int minHeightIndex = 0;
                int minHeight = 999999;
                for (int j = 0; j < kItemColumnSize; ++j) {
                    if (minHeight > columnHeights[j]) {
                        minHeight = columnHeights[j];
                        minHeightIndex = j;
                    }
                }

                int itemWidth = getItemWidth();
                int itemHeight = itemWidth / (double)it->getPixSize().width() * it->getPixSize().height();
                int posX = kLeftSpace + minHeightIndex * kItemInterHSpace + minHeightIndex * itemWidth;
                int posY = kItemInterVSpace + minHeight;
                columnHeights[minHeightIndex] = posY + itemHeight;

                it->setGeometry(posX, posY, itemWidth, itemHeight);
                it->setVisible(true);
                LOG(INFO) << "WaterFallWidget::resizeEvent (x,y):" << posX << " " << posY;
                LOG(INFO) << "WaterFallWidget::resizeEvent url:" << it->getImageUrl();
                its[i]++;
            }
            else {
                ++loadColumnCount;
            }
        }
    }

    int maxHeight = *std::max_element(columnHeights, columnHeights + kItemColumnSize);
    setFixedHeight(maxHeight + kBottomSpace);
}

void WaterFallWidget::setFixedHeightSafely() {
//    int h = *std::max_element(curColumnHeights, curColumnHeights + kItemColumnSize);
//    if (h > height() + 5) {
//        h = h + 10;
//    }
//    else {
//        h = height() + 10;
//    }

//    setFixedHeight(h + kBottomSpace);
}

void WaterFallWidget::getThmUrls() {
    std::vector<uint8_t> buff;
    if (HttpProxy::getRequest("http://192.168.0.1/cgi-bin/hi3510/getfilelist.cgi?-start=0&-end=100", buff)) {
        std::string str(buff.begin(), buff.end());
        boost::trim(str);
        boost::split(thumbnailUrlVec, str, boost::is_any_of(";"));
        std::reverse(thumbnailUrlVec.begin(), thumbnailUrlVec.end());//最新拍的放最前面
        valid = true;
        LOG(INFO) << "WaterFallWidget::getThmUrls urls:" << str;
    }  else {
        valid = false;
    }
}

int WaterFallWidget::calculateMaxHeight() {
//    int maxHeight = 0;
//    for (int i = 0; i < kItemColumnSize; ++i) {
//        if (!itemListColumn[i].empty() && itemListColumn[i].back()->height() > maxHeight) {
//            maxHeight = itemListColumn[i].back()->height();
//        }
//    }
//    return maxHeight;
    return *std::max_element(curColumnHeights, curColumnHeights + kItemColumnSize);
}

bool WaterFallWidget::reInit() {
    valid = false;
    thumbnailUrlIndex = 0;
    for (int i = 0; i < kItemColumnSize; ++i)
        curColumnHeights[i] = kTopSpace;

    for (int i = 0; i < kItemColumnSize; ++i) {
        itemListColumn[i].clear();
    }
    thumbnailUrlVec.clear();

    getThmUrls();
    getImages(20);
    return valid;
}

