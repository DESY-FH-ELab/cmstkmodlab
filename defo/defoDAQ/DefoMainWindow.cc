#include <string>

#include <QGroupBox>

#include "DefoConfig.h"
#include "DefoMainWindow.h"

DefoMainWindow::DefoMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
  // CONRAD MODEL
  conradModel_ = new DefoConradModel(this);

  // JULABO MODEL
  julaboModel_ = new DefoJulaboModel(5);

  // KEITHLEY MODEL
  keithleyModel_ = new DefoKeithleyModel(10);

  // MEASUREMENT MODEL
  listModel_ = new DefoMeasurementListModel();
  selectionModel_ = new DefoMeasurementSelectionModel();

  // CANON CAMERA MODEL
  cameraModel_ = new DefoCameraModel(this);
  connect(cameraModel_, SIGNAL(newImage(QString,bool)), SLOT(newCameraImage(QString,bool)));

  // POINT MODEL
  pointModel_ = new DefoPointRecognitionModel(this);

  // SCRIPT MODEL
  scriptModel_ = new DefoScriptModel(
        conradModel_
      , cameraModel_
      , julaboModel_
      , keithleyModel_
      , this
  );
  connect(scriptModel_, SIGNAL(prepareNewMeasurement()),
	  this, SLOT(prepareNewMeasurement()));
  connect(scriptModel_, SIGNAL(setControlsEnabled(bool)),
	  conradModel_, SLOT(setControlsEnabled(bool)));
  connect(scriptModel_, SIGNAL(setControlsEnabled(bool)),
	  cameraModel_, SLOT(setControlsEnabled(bool)));
  connect(scriptModel_, SIGNAL(setControlsEnabled(bool)),
	  julaboModel_, SLOT(setControlsEnabled(bool)));
  connect(scriptModel_, SIGNAL(setControlsEnabled(bool)),
	  keithleyModel_, SLOT(setControlsEnabled(bool)));
  connect(scriptModel_, SIGNAL(setControlsEnabled(bool)),
	  pointModel_, SLOT(setControlsEnabled(bool)));

  prepareNewMeasurement();

  tabWidget_ = new QTabWidget(this);
  tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  QBoxLayout *layout = new QHBoxLayout();
  QWidget *powerAndControlWidget = new QWidget(tabWidget_);
  powerAndControlWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  powerAndControlWidget->setLayout(layout);

  QGroupBox * groupbox = new QGroupBox("Power Supplies", powerAndControlWidget);
  // CONRAD MODEL
  DefoConradWidget* conradWidget = new DefoConradWidget(conradModel_);
  conradWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  QVBoxLayout *vbox = new QVBoxLayout;
  vbox->addWidget(conradWidget);
  vbox->addStretch(1);
  groupbox->setLayout(vbox);
  layout->addWidget(groupbox);

  QBoxLayout * vlayout = new QVBoxLayout();
  QWidget *widget = new QWidget(powerAndControlWidget);
  widget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  widget->setLayout(vlayout);

  groupbox = new QGroupBox("Chiller", widget);
  // JULABO MODEL
  DefoJulaboWidget* julaboWidget = new DefoJulaboWidget(julaboModel_);
  julaboWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  vbox = new QVBoxLayout;
  vbox->addWidget(julaboWidget);
  vbox->addStretch(1);
  groupbox->setLayout(vbox);

  vlayout->addWidget(groupbox);

  groupbox = new QGroupBox("Script", widget);
  DefoScriptWidget* scriptWidget = new DefoScriptWidget(scriptModel_);
  scriptWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  vbox = new QVBoxLayout;
  vbox->addWidget(scriptWidget);
  vbox->addStretch(1);
  groupbox->setLayout(vbox);

  vlayout->addWidget(groupbox);

  QPushButton *newMeasurementButton = new QPushButton("&New Measurement", widget);
  connect(newMeasurementButton, SIGNAL(clicked()), this, SLOT(prepareNewMeasurement()));
  vlayout->addWidget(newMeasurementButton);

  layout->addWidget(widget);

  tabWidget_->addTab(powerAndControlWidget, "Power and Control");

  layout = new QHBoxLayout();
  QWidget *measurementWidget = new QWidget(tabWidget_);
  measurementWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  measurementWidget->setLayout(layout);

  layout->addWidget(new DefoCameraWidget(
					 cameraModel_
                   , conradModel_
                   , listModel_
                   , selectionModel_
                   , this
					 ));

  if (cameraModel_->getDeviceState() == READY) {
    cameraModel_->setOptionSelection(DefoCameraModel::APERTURE, 6);
    cameraModel_->setOptionSelection(DefoCameraModel::ISO, 1);
    cameraModel_->setOptionSelection(DefoCameraModel::SHUTTER_SPEED, 26);
  }

  // read default settings
  DefoConfigReader cfgReader( std::string(Defo::CMSTkModLabBasePath) + "/defo/defo.cfg" );
  pointModel_->setThresholdValue(
        DefoPointRecognitionModel::THRESHOLD_1
        , cfgReader.getValue<int>( "STEP1_THRESHOLD" )
        );
  pointModel_->setThresholdValue(
        DefoPointRecognitionModel::THRESHOLD_2
        , cfgReader.getValue<int>( "STEP2_THRESHOLD" )
        );
  pointModel_->setThresholdValue(
        DefoPointRecognitionModel::THRESHOLD_3
        , cfgReader.getValue<int>( "STEP3_THRESHOLD" )
        );
  pointModel_->setHalfSquareWidth(
        cfgReader.getValue<int>( "HALF_SQUARE_WIDTH" )
        );

  DefoPointRecognitionWidget * pointWidget =
      new DefoPointRecognitionWidget(listModel_,
                                     selectionModel_,
                                     pointModel_,
                                     measurementWidget);
  layout->addWidget(pointWidget);

  tabWidget_->addTab(measurementWidget, "Measurement");

  layout = new QHBoxLayout();
  QWidget *temperatureWidget = new QWidget(tabWidget_);
  temperatureWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  temperatureWidget->setLayout(layout);

  // KEITHLEY MODEL
  DefoKeithleyWidget *keithleyWidget = new DefoKeithleyWidget(keithleyModel_);
  layout->addWidget( keithleyWidget );

  tabWidget_->addTab(temperatureWidget, "Temperature");

  setCentralWidget(tabWidget_);
}

void DefoMainWindow::prepareNewMeasurement() {

  // Set up current directory for saving images
  QDateTime dt = QDateTime::currentDateTimeUtc();
  QString measurementDirPath(QDir::homePath() + "/Desktop/measurements/%1-%2/");
  measurementDirPath = measurementDirPath.arg(dt.toString("yyyyMMdd"));
  
  int i = 1;
  do {
    currentDir_.setPath( measurementDirPath.arg(i) );
    ++i;
  } while ( currentDir_.exists() );

  listModel_->clear();
}

void DefoMainWindow::newCameraImage(QString location, bool keep) {

  if (!keep) {

    DefoMeasurement * measurement = new DefoMeasurement(location, true);

    listModel_->addMeasurement(measurement);
    selectionModel_->setSelection(measurement);

  } else {

    DefoMeasurement * measurement = new DefoMeasurement(location, false);

    // TODO save when needed, i.e. always from now on
    QDateTime dt = measurement->getTimeStamp();

    // Create with first picture taken
    if (!currentDir_.exists())
      currentDir_.mkpath(currentDir_.absolutePath());

    QString imageLocation = currentDir_.absoluteFilePath("%1.jpg");
    imageLocation = imageLocation.arg(dt.toString("yyyyMMddhhmmss"));

    measurement->setImageLocation(imageLocation);

    QFile file(cameraModel_->getLastPictureLocation());
    file.copy(imageLocation);
    file.close();

    // acquire status information and store in measurement
    measurement->readExifData();
    measurement->acquireData(pointModel_);
    measurement->acquireData(conradModel_);
    measurement->acquireData(julaboModel_);
    measurement->acquireData(keithleyModel_);

    measurement->write(currentDir_.absolutePath());

    listModel_->addMeasurement(measurement);
    selectionModel_->setSelection(measurement);

    listModel_->write(currentDir_.absolutePath());
  }
}
