#include <vector>

#include <QVBoxLayout>
#include <QFileDialog>
#include <QPushButton>

#include <nqlogger.h>

#include "AssemblyUEyeSnapShooter.h"

AssemblyUEyeSnapShooter::AssemblyUEyeSnapShooter(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *l = new QVBoxLayout(this);
    setLayout(l);

    QPushButton * button = new QPushButton("save", this);
    connect(button, SIGNAL(clicked(bool)),
            this, SLOT(snapShot()));
    l->addWidget(button);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(220, 220, 220));

    imageView_ = new AssemblyUEyeView();
    imageView_->setMinimumSize(500, 500);
    imageView_->setPalette(palette);
    imageView_->setBackgroundRole(QPalette::Background);
    imageView_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView_->setScaledContents(true);
    imageView_->setAlignment(Qt::AlignCenter);

    scrollArea_ = new QScrollArea(this);
    scrollArea_->setMinimumSize(500, 500);
    scrollArea_->setPalette(palette);
    scrollArea_->setBackgroundRole(QPalette::Background);
    scrollArea_->setAlignment(Qt::AlignCenter);
    scrollArea_->setWidget(imageView_);
    scrollArea_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    l->addWidget(scrollArea_);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void AssemblyUEyeSnapShooter::connectCamera(AssemblyVUEyeCamera *camera)
{
    NQLog("AssemblyUEyeSnapShooter") << ":connectCamera(AssemblyVUEyeCamera *camera)";

    imageView_->connectCamera(camera);

    connect(camera, SIGNAL(imageAcquired(const cv::Mat&)),
            this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyUEyeSnapShooter::connectMarkerFinder(AssemblyVMarkerFinder* finder)
{
    NQLog("AssemblyUEyeSnapShooter") << ":connectMarkerFinder(AssemblyVMarkerFinder* finder)";

    imageView_->connectMarkerFinder(finder);

    connect(finder, SIGNAL(markerFound(const cv::Mat&)),
            this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyUEyeSnapShooter::disconnectCamera(AssemblyVUEyeCamera * camera)
{
    NQLog("AssemblyUEyeSnapShooter") << ":disconnectCamera(AssemblyVUEyeCamera * camera)";

    imageView_->disconnectCamera(camera);

    camera->disconnect(camera, SIGNAL(imageAcquired(const cv::Mat&)),
                       this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyUEyeSnapShooter::disconnectMarkerFinder(AssemblyVMarkerFinder* finder)
{
    NQLog("AssemblyUEyeSnapShooter") << ":connectMarkerFinder(AssemblyVMarkerFinder* finder)";

    imageView_->disconnectMarkerFinder(finder);

    disconnect(finder, SIGNAL(markerFound(const cv::Mat&)),
               this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyUEyeSnapShooter::snapShot()
{
    // NQLog("AssemblyUEyeSnapShooter") << ":snapShot()";

    if (image_.rows==0) return;

    QString filename = QFileDialog::getSaveFileName(this, "save image", ".", "*.jpg");
    if (filename.isNull() || filename.isEmpty()) return;

    if (!filename.endsWith(".jpg")) filename += ".jpg";

    cv::imwrite(filename.toStdString(), image_);
}

void AssemblyUEyeSnapShooter::imageAcquired(const cv::Mat& newImage)
{
    newImage.copyTo(image_);
}

void AssemblyUEyeSnapShooter::keyReleaseEvent(QKeyEvent * event)
{
    if (!(event->modifiers() & Qt::ShiftModifier)) {
        switch (event->key()) {
        case Qt::Key_0:
            imageView_->setZoomFactor(0.25);
            event->accept();
            break;
        case Qt::Key_1:
            imageView_->setZoomFactor(1.00);
            event->accept();
            break;
        case Qt::Key_Plus:
            imageView_->increaseZoomFactor();
            event->accept();
            break;
        case Qt::Key_Minus:
            imageView_->decreaseZoomFactor();
            event->accept();
            break;
        default:
            break;
        }
    }
}
