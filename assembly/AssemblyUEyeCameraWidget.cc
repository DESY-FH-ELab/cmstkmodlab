#include <QFormLayout>

#include <nqlogger.h>

#include "AssemblyUEyeCameraWidget.h"

AssemblyUEyeCameraWidget::AssemblyUEyeCameraWidget(AssemblyUEyeCamera_t* camera,
                                                   QWidget *parent)
    : QToolBox(parent),
      camera_(camera)
{
    addItem(new AssemblyUEyeCameraGeneralWidget(camera, this), "general information");

    // Connect all the signals
    connect(camera_, SIGNAL(cameraInformationChanged()),
            this, SLOT(cameraInformationChanged()));

    cameraInformationChanged();
}

void AssemblyUEyeCameraWidget::cameraInformationChanged()
{
    NQLog("AssemblyUEyeCameraWidget") << "update information";
}

AssemblyUEyeCameraGeneralWidget::AssemblyUEyeCameraGeneralWidget(AssemblyUEyeCamera_t* camera,
                                                                 QWidget *parent)
    : QWidget(parent),
      camera_(camera)
{
    QFormLayout * layout = new QFormLayout(this);

    nameLabel_ = new QLabel("name", this);
    layout->addRow("name:", nameLabel_);

    fullNameLabel_ = new QLabel("full name", this);
    layout->addRow("full name:", fullNameLabel_);

    cameraIDLabel_ = new QLabel("camera ID", this);
    layout->addRow("camera ID:", cameraIDLabel_);

    deviceIDLabel_ = new QLabel("device ID", this);
    layout->addRow("device ID:", deviceIDLabel_);

    sensorIDLabel_ = new QLabel("sensor ID", this);
    layout->addRow("sensor ID:", sensorIDLabel_);

    serialNumberLabel_ = new QLabel("serial number", this);
    layout->addRow("serial number:", serialNumberLabel_);

    statusLabel_ = new QLabel("status", this);
    layout->addRow("status:", statusLabel_);

    idLabel_ = new QLabel("id", this);
    layout->addRow("id:", idLabel_);

    versionLabel_ = new QLabel("version", this);
    layout->addRow("version:", versionLabel_);

    dateLabel_ = new QLabel("date", this);
    layout->addRow("date:", dateLabel_);

    setLayout(layout);

    // Connect all the signals
    connect(camera_, SIGNAL(cameraInformationChanged()),
            this, SLOT(cameraInformationChanged()));

    cameraInformationChanged();
}

void AssemblyUEyeCameraGeneralWidget::cameraInformationChanged()
{
    nameLabel_->setText(camera_->getModelName());
    fullNameLabel_->setText(camera_->getFullModelName());
    cameraIDLabel_->setText(QString::number(camera_->getCameraID()));
    deviceIDLabel_->setText(QString::number(camera_->getDeviceID()));
    sensorIDLabel_->setText(QString::number(camera_->getSensorID()));
    serialNumberLabel_->setText(QString::number(camera_->getSerialNumber()));
    statusLabel_->setText(QString::number(camera_->getStatus()));
    idLabel_->setText(camera_->getID());
    versionLabel_->setText(camera_->getVersion());
    dateLabel_->setText(camera_->getDate());
}
