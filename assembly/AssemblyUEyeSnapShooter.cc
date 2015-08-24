#include <vector>

#include <QVBoxLayout>
#include <QFileDialog>

#include <nqlogger.h>

#include "AssemblyUEyeSnapShooter.h"

AssemblyUEyeSnapShooter::AssemblyUEyeSnapShooter(QWidget *parent)
    : QWidget(parent),
      camera_(0),
      takeSnapShot_(false)
{
    QVBoxLayout *l = new QVBoxLayout(this);
    setLayout(l);

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

    l->addWidget(scrollArea_);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void AssemblyUEyeSnapShooter::connectCamera(AssemblyVUEyeCamera *camera)
{
    NQLog("AssemblyMainWindow") << ":connectCamera(AssemblyVUEyeCamera *camera)";

    camera_ = camera;
    if (camera_) imageView_->connectCamera(camera);

    connect(camera_, SIGNAL(imageAcquired(const QImage&)),
            this, SLOT(imageAcquired(const QImage&)));
}

void AssemblyUEyeSnapShooter::disconnectCamera(AssemblyVUEyeCamera * /* camera */)
{
    NQLog("AssemblyMainWindow") << ":disconnectCamera(AssemblyVUEyeCamera * camera)";

    disconnect(SIGNAL(imageAcquired(const QImage&)),
               this, SLOT(imageAcquired(const QImage&)));
}

void AssemblyUEyeSnapShooter::snapShot()
{
    NQLog("AssemblyUEyeSnapShooter") << ":snapShot()";

    takeSnapShot_ = true;
}

void AssemblyUEyeSnapShooter::imageAcquired(const QImage& newImage)
{
    NQLog("AssemblyUEyeSnapShooter") << ":imageAcquired(const QImage& newImage)";

    if (takeSnapShot_) {
        takeSnapShot_ = false;

        QImage image = newImage;

        QString filename = QFileDialog::getSaveFileName(this, "save image", ".", "*.jpg");
        if (filename.isNull() || filename.isEmpty()) return;

        if (!filename.endsWith(".jpg")) filename += ".jpg";

        image.save(filename);
    }
}
