/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <AssemblyUEyeView.h>

#include <QPainter>

AssemblyUEyeView::AssemblyUEyeView(QWidget* parent) :
  QLabel(parent),
  image_(QImage()),
  zoomFactor_(0.25)
{
}

void AssemblyUEyeView::connectImageProducer(const QObject* sender, const char* signal)
{
    connect   (sender, signal, this, SLOT(setImage(const cv::Mat&)));
}

void AssemblyUEyeView::disconnectImageProducer(const QObject* sender, const char* signal)
{
    disconnect(sender, signal, this, SLOT(setImage(const cv::Mat&)));
}

void AssemblyUEyeView::setImage(const cv::Mat& newImage)
{
    QMutexLocker lock(&mutex_);

    if(newImage.channels() == 1)
    {
        cv::Mat temp;
        cvtColor(newImage, temp, cv::COLOR_GRAY2RGB);

        image_ = QImage((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
        image_.bits();
    }
    else
    {
        image_ = QImage((const uchar *) newImage.data, newImage.cols, newImage.rows, newImage.step, QImage::Format_RGB888);
        image_.bits();
    }

    lock.unlock();

    update();
}

void AssemblyUEyeView::resetImage()
{
    QMutexLocker lock(&mutex_);
    image_ = QImage();
    lock.unlock();
    update();
}

void AssemblyUEyeView::paintEvent(QPaintEvent*)
{
    if(image_.isNull() == false)
    {
        const QImage image = image_.scaled(image_.width()  * zoomFactor_, image_.height() * zoomFactor_, Qt::KeepAspectRatio);

        resize(image.width(), image.height());

        //qreal posX = this->width()  / 2 - image.width()  / 2;
        //qreal posY = this->height() / 2 - image.height() / 2;
        QRectF target(0, 0, image.width(), image.height());

        QPainter painter(this);
        painter.drawImage(target, image);
        painter.end();
    }

    return;
}

void AssemblyUEyeView::setZoomFactor(float zoomFactor)
{
    if(zoomFactor_ == zoomFactor){ return; }

    zoomFactor_ = zoomFactor;
    update();
}

void AssemblyUEyeView::increaseZoomFactor()
{
    if(zoomFactor_ >= 4.0){ return; }

    zoomFactor_ += 0.05;
    update();
}

void AssemblyUEyeView::decreaseZoomFactor()
{
    if (zoomFactor_<=0.25) return;

    zoomFactor_ -= 0.05;
    update();
}
