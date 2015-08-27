#include <vector>

#include <QPainter>

#include <nqlogger.h>

#include "AssemblyUEyeView.h"

AssemblyUEyeView::AssemblyUEyeView(QWidget *parent)
    : QLabel(parent),
      camera_(0),
      image_(0)
{

}

void AssemblyUEyeView::connectCamera(AssemblyVUEyeCamera *camera)
{
    camera_ = camera;

    connect(camera_, SIGNAL(imageAcquired(const cv::Mat&)),
            this, SLOT(setImage(const cv::Mat&)));
}

void AssemblyUEyeView::disconnectCamera(AssemblyVUEyeCamera *camera)
{
    disconnect(SIGNAL(imageAcquired(const cv::Mat&)),
               this, SLOT(setImage(const cv::Mat&)));

}

void AssemblyUEyeView::setImage(const cv::Mat& newImage)
{
    QMutexLocker lock(&mutex_);

    NQLog("AssemblyUEyeView") << "set image";

    cv::Mat temp;
    cvtColor(newImage, temp, CV_GRAY2RGB);

    image_ = QImage((const uchar *) temp.data,
                    temp.cols, temp.rows,
                    temp.step, QImage::Format_RGB888).scaled(this->width(),
                                                             this->height(),
                                                             Qt::KeepAspectRatio);
    image_.bits();

    update();
}

void AssemblyUEyeView::paintEvent(QPaintEvent* /* ev */)
{
    if (!image_.isNull()) {

        qreal posX = this->width() / 2 - image_.width() / 2;
        qreal posY = this->height() / 2 - image_.height() / 2;
        QRectF target(posX, posY, image_.width(), image_.height());

        QPainter painter(this);
        painter.drawImage(target, image_);
        painter.end();
    }
}
