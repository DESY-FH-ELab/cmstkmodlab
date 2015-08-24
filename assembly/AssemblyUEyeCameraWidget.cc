#include <QFormLayout>

#include <nqlogger.h>

#include "AssemblyUEyeCameraWidget.h"

AssemblyUEyeCameraWidget::AssemblyUEyeCameraWidget(AssemblyVUEyeCamera* camera,
                                                   QWidget *parent)
    : QToolBox(parent),
      camera_(camera)
{
    addItem(new AssemblyUEyeCameraGeneralWidget(camera, this), "general information");
    addItem(new AssemblyUEyeCameraSensorWidget(camera, this), "sensor information");
    addItem(new AssemblyUEyeCameraSettingsWidget(camera, this), "settings");

    // Connect all the signals
    connect(camera_, SIGNAL(cameraInformationChanged()),
            this, SLOT(cameraInformationChanged()));

    cameraInformationChanged();
}

void AssemblyUEyeCameraWidget::cameraInformationChanged()
{
    NQLog("AssemblyUEyeCameraWidget") << "update information";
}

AssemblyUEyeCameraGeneralWidget::AssemblyUEyeCameraGeneralWidget(AssemblyVUEyeCamera* camera,
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

AssemblyUEyeCameraSensorWidget::AssemblyUEyeCameraSensorWidget(AssemblyVUEyeCamera* camera,
                                                                 QWidget *parent)
    : QWidget(parent),
      camera_(camera)
{
    QFormLayout * layout = new QFormLayout(this);

    sensorNameLabel_ = new QLabel("sensor name", this);
    layout->addRow("sensor name:", sensorNameLabel_);

    colorModeLabel_ = new QLabel("color mode", this);
    layout->addRow("color mode:", colorModeLabel_);

    maxResolutionLabel_ = new QLabel("maximum resolution", this);
    layout->addRow("maximum resolution:", maxResolutionLabel_);

    masterGainLabel_ = new QLabel("master gain", this);
    layout->addRow("master gain:", masterGainLabel_);

    rgbGainLabel_ = new QLabel("rgb gain", this);
    layout->addRow("rgb gain:", rgbGainLabel_);

    globalShutterLabel_ = new QLabel("global shutter", this);
    layout->addRow("global shutter:", globalShutterLabel_);

    pixelSizeLabel_ = new QLabel("pixel size", this);
    layout->addRow("pixel size:", pixelSizeLabel_);

    setLayout(layout);

    // Connect all the signals
    connect(camera_, SIGNAL(cameraInformationChanged()),
            this, SLOT(cameraInformationChanged()));

    cameraInformationChanged();
}

void AssemblyUEyeCameraSensorWidget::cameraInformationChanged()
{
    sensorNameLabel_->setText(camera_->getSensorName());
    colorModeLabel_->setText(QString::number(camera_->getColorMode()));
    maxResolutionLabel_->setText(QString("%1 x %2").arg(camera_->getMaxWidth()).arg(camera_->getMaxHeight()));
    masterGainLabel_->setText(QString::number(camera_->getMasterGain()));
    rgbGainLabel_->setText(QString("%1 %2 %3").arg(camera_->getRedGain()).arg(camera_->getGreenGain()).arg(camera_->getBlueGain()));
    globalShutterLabel_->setText(QString::number(camera_->getGlobalShutter()));
    pixelSizeLabel_->setText(QString::number(camera_->getPixelSize()));
}

AssemblyUEyeCameraPixelClockWidget::AssemblyUEyeCameraPixelClockWidget(AssemblyVUEyeCamera* camera,
                                        QWidget *parent)
    : QComboBox(parent),
      camera_(camera)
{
    connect(camera_, SIGNAL(pixelClockListChanged(unsigned int)),
            this, SLOT(pixelClockListChanged(unsigned int)));

    connect(camera_, SIGNAL(pixelClockChanged(unsigned int)),
            this, SLOT(pixelClockChanged(unsigned int)));

    connect(this, SIGNAL(changePixelClock(uint)),
            camera_, SLOT(setPixelClock(uint)));

    connect(this, SIGNAL(currentIndexChanged(int)),
            this, SLOT(currentItemChanged(int)));
}

void AssemblyUEyeCameraPixelClockWidget::currentItemChanged(int idx)
{
    unsigned int pixelClock = itemData(idx, Qt::UserRole).toUInt();

    NQLog("AssemblyUEyeCameraPixelClockWidget") << ":currentItemChanged(int idx) " << pixelClock;

    emit changePixelClock(pixelClock);
}

void AssemblyUEyeCameraPixelClockWidget::pixelClockListChanged(unsigned int current)
{
    NQLog("AssemblyUEyeCameraPixelClockWidget") << ":pixelClockListChanged()";

    disconnect(this, SIGNAL(currentIndexChanged(int)),
               this, SLOT(currentItemChanged(int)));

    clear();

    const std::vector<unsigned int> &pixelClocks = camera_->getPixelClockList();
    for (std::vector<unsigned int>::const_iterator it = pixelClocks.begin();
         it != pixelClocks.end();
         ++it) {
        addItem(QString("%1 MHz").arg(*it), QVariant(*it));
    }

    pixelClockChanged(current);

    connect(this, SIGNAL(currentIndexChanged(int)),
            this, SLOT(currentItemChanged(int)));
}

void AssemblyUEyeCameraPixelClockWidget::pixelClockChanged(unsigned int current)
{
    NQLog("AssemblyUEyeCameraPixelClockWidget") << ":pixelClockChanged() " << current;

    int idx = findData(QVariant(current), Qt::UserRole);

    if (idx!=-1) {
        setCurrentIndex(idx);
    }
}

AssemblyUEyeCameraExposureTimeWidget::AssemblyUEyeCameraExposureTimeWidget(AssemblyVUEyeCamera* camera,
                                        QWidget *parent)
    : QSlider(parent),
      camera_(camera)
{
    setTracking(false);
    setOrientation(Qt::Horizontal);

    connect(camera_, SIGNAL(exposureTimeRangeChanged(double)),
            this, SLOT(exposureTimeRangeChanged(double)));

    connect(camera_, SIGNAL(exposureTimeChanged(double)),
            this, SLOT(exposureTimeChanged(double)));

    connect(this, SIGNAL(changeExposureTime(double)),
            camera_, SLOT(setExposureTime(double)));

    connect(this, SIGNAL(valueChanged(int)),
            this, SLOT(currentValueChanged(int)));
}

void AssemblyUEyeCameraExposureTimeWidget::currentValueChanged(int value)
{
    double exposureTime = camera_->getExposureTimeMin() + camera_->getExposureTimeInc() * value;

    NQLog("AssemblyUEyeCameraExposureTimeWidget") << ":currentValueChanged(int value) " << exposureTime;

    emit changeExposureTime(exposureTime);
}

void AssemblyUEyeCameraExposureTimeWidget::exposureTimeRangeChanged(double current)
{
    NQLog("AssemblyUEyeCameraExposureTimeWidget") << ":exposureTimeRangeChanged()";

    disconnect(this, SIGNAL(valueChanged(int)),
               this, SLOT(currentValueChanged(int)));

    setMinimum(0);
    setMaximum(1+(camera_->getExposureTimeMax()-camera_->getExposureTimeMin())/camera_->getExposureTimeInc());
    setSingleStep(1);

    exposureTimeChanged(current);

    connect(this, SIGNAL(valueChanged(int)),
            this, SLOT(currentValueChanged(int)));
}

void AssemblyUEyeCameraExposureTimeWidget::exposureTimeChanged(double current)
{
    NQLog("AssemblyUEyeCameraExposureTimeWidget") << ":exposureTimeChanged() " << current;

    setValue((current - camera_->getExposureTimeMin()) / camera_->getExposureTimeInc());
}

AssemblyUEyeCameraSettingsWidget::AssemblyUEyeCameraSettingsWidget(AssemblyVUEyeCamera* camera,
                                                                 QWidget *parent)
    : QWidget(parent),
      camera_(camera)
{
    QFormLayout * layout = new QFormLayout(this);

    layout->addRow("pixel clock", new AssemblyUEyeCameraPixelClockWidget(camera_, this));
    layout->addRow("exposure time", new AssemblyUEyeCameraExposureTimeWidget(camera_, this));

    setLayout(layout);

    // Connect all the signals
    connect(camera_, SIGNAL(cameraInformationChanged()),
            this, SLOT(cameraInformationChanged()));

    cameraInformationChanged();
}

void AssemblyUEyeCameraSettingsWidget::cameraInformationChanged()
{

}
