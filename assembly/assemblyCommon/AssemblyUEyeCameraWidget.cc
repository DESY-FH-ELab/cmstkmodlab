#include <QFormLayout>
#include <QString>
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

AssemblyUEyeCameraExposureTimeSlider::AssemblyUEyeCameraExposureTimeSlider(AssemblyVUEyeCamera* camera,
                                        QWidget *parent)
    : QSlider(parent),
      camera_(camera)
{
    setMinimumWidth(200);
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

void AssemblyUEyeCameraExposureTimeSlider::currentValueChanged(int value)
{
    double exposureTime = camera_->getExposureTimeMin() + camera_->getExposureTimeInc() * value;

    NQLog("AssemblyUEyeCameraExposureTimeSlider") << ":currentValueChanged(int value) " << exposureTime;

    emit changeExposureTime(exposureTime);
}

void AssemblyUEyeCameraExposureTimeSlider::exposureTimeRangeChanged(double current)
{
    NQLog("AssemblyUEyeCameraExposureTimeSlider") << ":exposureTimeRangeChanged()";

    disconnect(this, SIGNAL(valueChanged(int)),
               this, SLOT(currentValueChanged(int)));

    setMinimum(0);
    setMaximum(1+(camera_->getExposureTimeMax()-camera_->getExposureTimeMin())/camera_->getExposureTimeInc());
    setSingleStep(1);

    exposureTimeChanged(current);

    connect(this, SIGNAL(valueChanged(int)),
            this, SLOT(currentValueChanged(int)));
}

void AssemblyUEyeCameraExposureTimeSlider::exposureTimeChanged(double current)
{
    NQLog("AssemblyUEyeCameraExposureTimeSlider %%%") << ":exposureTimeChanged() " << current;

     setValue((current - camera_->getExposureTimeMin()) / camera_->getExposureTimeInc());

}

AssemblyUEyeCameraExposureTimeWidget::AssemblyUEyeCameraExposureTimeWidget(AssemblyVUEyeCamera* camera,
                                        QWidget *parent)
    : QWidget(parent),
      camera_(camera)
{
    setMinimumHeight(40);

    QGridLayout* layout = new QGridLayout(this);

    minLabel_ = new QLabel("min", this);
    layout->addWidget(minLabel_, 0, 0, 1, 1);

    layout->addWidget(new AssemblyUEyeCameraExposureTimeSlider(camera_, this), 0, 1, 1, 1);

    maxLabel_ = new QLabel("max", this);
    layout->addWidget(maxLabel_, 0, 2, 1, 1);

    currentLabel_ = new QLabel("current", this);
    layout->addWidget(currentLabel_, 1, 1, 1, 1, Qt::AlignHCenter);

    setLayout(layout);

    connect(camera_, SIGNAL(exposureTimeRangeChanged(double)),
            this, SLOT(exposureTimeRangeChanged(double)));

    connect(camera_, SIGNAL(exposureTimeChanged(double)),
            this, SLOT(exposureTimeChanged(double)));
}

void AssemblyUEyeCameraExposureTimeWidget::exposureTimeRangeChanged(double current)
{
    NQLog("AssemblyUEyeCameraExposureTimeWidget") << ":exposureTimeRangeChanged()";

    minLabel_->setText(QString("%1 ms").arg(camera_->getExposureTimeMin()));
    maxLabel_->setText(QString("%1 ms").arg(camera_->getExposureTimeMax()));
    currentLabel_->setText(QString("%1 ms").arg(current));
}

void AssemblyUEyeCameraExposureTimeWidget::exposureTimeChanged(double current)
{
    NQLog("AssemblyUEyeCameraExposureTimeWidget") << ":exposureTimeChanged() " << current;

    currentLabel_->setText(QString("%1 ms").arg(current));
}

AssemblyUEyeCameraSettingsWidget::AssemblyUEyeCameraSettingsWidget(AssemblyVUEyeCamera* camera,
                                                                 QWidget *parent)
    : QWidget(parent),
      camera_(camera)
{
    QFormLayout * layout = new QFormLayout(this);
    layout->addRow("pixel clock", new AssemblyUEyeCameraPixelClockWidget(camera_, this));
    layout->addRow("exposure time", new AssemblyUEyeCameraExposureTimeWidget(camera_, this));
    layout->addRow("", new AssemblyUEyeCameraSettingsCalibrater(camera_, this));
    
    x_coor = new QLineEdit();
    y_coor = new QLineEdit();
    ang = new QLineEdit();
    
    layout->addRow("X", x_coor);
    layout->addRow("Y", y_coor);
    layout->addRow("Angle", ang);
    
    setLayout(layout);
    
//    updateCoordinates(1.0,1.0,1.0);

    // Connect all the signals
    connect(camera_, SIGNAL(cameraInformationChanged()),
            this, SLOT(cameraInformationChanged()));

    connect(camera_, SIGNAL(resultObtained(double, double ,double)),
            this, SLOT(updateResult(double, double, double)));
    
    cameraInformationChanged();
}

void AssemblyUEyeCameraSettingsWidget::cameraInformationChanged()
{

    
    
}


void AssemblyUEyeCameraSettingsWidget::updateResult(double x, double y, double angle)
{
    NQLog("AssemblyUEyeCameraSettingWidget") << ":updateResults";

    QString x_str = QString::number(x);
    QString y_str = QString::number(y);
    QString ang_str = QString::number(angle);
    
    x_coor->setText(x_str);
    y_coor->setText(y_str);
    ang->setText(ang_str);
    
}



AssemblyUEyeCameraSettingsCalibrater::AssemblyUEyeCameraSettingsCalibrater(AssemblyVUEyeCamera* camera,
                                                                 QWidget *parent)
    : QPushButton(parent),
      camera_(camera)
{
    
    // parent->updateCoordinates(1.0,1.0,1.0);
     this->setText("Calibrate camera / Find marker");
     // Connect all the signals
     //   connect(this, SIGNAL(clicked()),
     //       this, SLOT(onCalibrate()));
    

    connect(this, SIGNAL(changeExposureTime(double)),
          camera_, SLOT(setExposureTime(double)));

    connect(this, SIGNAL(clicked()),
  	    camera_, SLOT(calibrateSettings()));
    


}


AssemblyUEyeCameraMarkerFinderResult::AssemblyUEyeCameraMarkerFinderResult()
{

    NQLog("AssemblyUEyeCameraSettingWidget") << ":results";
}






void AssemblyUEyeCameraSettingsCalibrater::onCalibrate()
{
    NQLog("AssemblyUEyeCameraSettingWidget") << ":starting Calibration Routine...";
    //setValue((current - camera_->getExposureTimeMin()) / camera_->getExposureTimeInc());
    //  setValue(100.0);

    //connect(this, SIGNAL(changeExposureTime(double)),
	      //       camera_, SLOT(setExposureTime(double)));

    emit changeExposureTime(50.0);
    //    emit changeExposureTime(100.0);
    //emit changeExposureTime(150.0);
    //emit changeExposureTime(200.0);


    // sleep(5);
    //  emit acquireImage();

   // parent->updateCoordinates(10.0,10.0,10.0);

}



void AssemblyUEyeCameraSettingsWidget::updateCoordinates(double, double, double)
{
    NQLog("AssemblyUEyeCameraSettingWidget") << ":updating coordinates";
    x_coor->setText("Test");
    
}



