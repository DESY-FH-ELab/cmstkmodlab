#include <vector>

#include <QPainter>

#include <nqlogger.h>

#include "AssemblyUEyeView.h"

AssemblyUEyeView::AssemblyUEyeView(QWidget *parent)
    : QLabel(parent),
      image_(QImage()),
      zoomFactor_(0.25)
{

}

void AssemblyUEyeView::connectImageProducer(const QObject* sender,
                                            const char* signal)
{
    connect(sender, signal,
            this, SLOT(setImage(const cv::Mat&)));
}

void AssemblyUEyeView::disconnectImageProducer(const QObject* sender,
                                               const char* signal)
{
    disconnect(sender, signal,
               this, SLOT(setImage(const cv::Mat&)));
}

void AssemblyUEyeView::setImage(const cv::Mat& newImage)
{
    QMutexLocker lock(&mutex_);

    //NQLog("AssemblyUEyeView") << "set image " << newImage.type() << " " << newImage.channels();


    if (newImage.channels()==1) {
        cv::Mat temp;
        cvtColor(newImage, temp, CV_GRAY2RGB);
        image_ = QImage((const uchar *) temp.data,
                        temp.cols, temp.rows,
                        temp.step, QImage::Format_RGB888);
        image_.bits();
    } else {
        image_ = QImage((const uchar *) newImage.data,
                        newImage.cols, newImage.rows,
                        newImage.step, QImage::Format_RGB888);
        image_.bits();
    }

    lock.unlock();

    update();
}

void AssemblyUEyeView::paintEvent(QPaintEvent* /* ev */)
{
    if (!image_.isNull()) {

        QImage image = image_.scaled(image_.width()*zoomFactor_,
                                     image_.height()*zoomFactor_,
                                     Qt::KeepAspectRatio);

        resize(image.width(), image.height());

        //qreal posX = this->width() / 2 - image.width() / 2;
        //qreal posY = this->height() / 2 - image.height() / 2;
        QRectF target(0, 0, image.width(), image.height());

        QPainter painter(this);
        painter.drawImage(target, image);
        painter.end();
    }
}

void AssemblyUEyeView::setZoomFactor(float zoomFactor)
{
    if (zoomFactor_==zoomFactor) return;
    zoomFactor_ = zoomFactor;
    update();
}

void AssemblyUEyeView::increaseZoomFactor()
{
    if (zoomFactor_>=4.0) return;

    zoomFactor_ += 0.05;
    update();
}

void AssemblyUEyeView::decreaseZoomFactor()
{
    if (zoomFactor_<=0.25) return;

    zoomFactor_ -= 0.05;
    update();
}
