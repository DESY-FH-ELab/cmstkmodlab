#include <vector>

#include <QPainter>

#include <nqlogger.h>

#include "AssemblyUEyeView.h"

AssemblyUEyeView::AssemblyUEyeView(QWidget *parent)
    : QLabel(parent),
      camera_(0)
{

}

void AssemblyUEyeView::connectCamera(AssemblyUEyeCamera_t* camera)
{
    camera_ = camera;

    connect(camera_, SIGNAL(imageAcquired(const QImage&)),
            this, SLOT(setImage(const QImage&)));
}

void AssemblyUEyeView::disconnectCamera(AssemblyUEyeCamera_t* camera)
{
    disconnect(SIGNAL(imageAcquired(const QImage&)),
               this, SLOT(setImage(const QImage&)));

}

void AssemblyUEyeView::setImage(const QImage& newImage)
{
    NQLog("AssemblyUEyeView") << "set image";

    image_ = newImage.scaled(this->width(), this->height(), Qt::KeepAspectRatio);

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
