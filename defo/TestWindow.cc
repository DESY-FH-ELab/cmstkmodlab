#include "ApplicationConfig.h"
#include "TestWindow.h"

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent)
{

  // Set up current directory for saving images
  QDateTime dt = QDateTime::currentDateTime().toUTC();
  QString measurementDirPath("/home/tkmodlab/Desktop/measurements/%1-%2/");
  measurementDirPath = measurementDirPath.arg(dt.toString("yyyyMMdd"));
  
  int i = 1;
  do {
    currentDir_.setPath( measurementDirPath.arg(i) );
    ++i;
  } while ( currentDir_.exists() );

  // Loop over all the existing directories, create the next one
//  currentDir_.mkpath(currentDir_.absolutePath());
  // END CURRENTDIR


  QHBoxLayout *layout = new QHBoxLayout();
  QWidget *central = new QWidget();
  central->setLayout(layout);

  // CONRAD MODEL
  DefoConradModel* conrad = new DefoConradModel(this);
  DefoConradWidget* conradWidget = new DefoConradWidget(conrad);
  conradWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  layout->addWidget( conradWidget );

  // JULABO MODEL
  //layout->addWidget( new DefoJulaboWidget(new DefoJulaboModel(3)) );

  // KEITHLEY MODEL
  //layout->addWidget( new DefoKeithleyWidget(new DefoKeithleyModel(5)) );

  // MEASUREMENT MODEL
  listModel_ = new DefoMeasurementListModel();
  selectionModel_ = new DefoMeasurementSelectionModel();

  // CANON CAMERA MODEL
  cameraModel_ = new DefoCameraModel(this);
  layout->addWidget(
        new DefoCameraWidget(
            cameraModel_
          , conrad
          , listModel_
          , selectionModel_
          , this
        )
  );

  if (cameraModel_->getDeviceState() == READY) {
    cameraModel_->setOptionSelection(DefoCameraModel::APERTURE, 6);
    cameraModel_->setOptionSelection(DefoCameraModel::ISO, 1);
    cameraModel_->setOptionSelection(DefoCameraModel::SHUTTER_SPEED, 26);
  }

  connect(cameraModel_, SIGNAL(newImage(QString,bool)), SLOT(newCameraImage(QString,bool)));

  pointModel_ = new DefoPointRecognitionModel(this);

  // read default settings
  pointModel_->setThresholdValue(
          DefoPointRecognitionModel::THRESHOLD_1
        , ApplicationConfig::instance()->getValue<int>( "STEP1_THRESHOLD" )
  );
  pointModel_->setThresholdValue(
          DefoPointRecognitionModel::THRESHOLD_2
        , ApplicationConfig::instance()->getValue<int>( "STEP2_THRESHOLD" )
  );
  pointModel_->setThresholdValue(
          DefoPointRecognitionModel::THRESHOLD_3
        , ApplicationConfig::instance()->getValue<int>( "STEP3_THRESHOLD" )
  );
  pointModel_->setHalfSquareWidth(
        ApplicationConfig::instance()->getValue<int>( "HALF_SQUARE_WIDTH" )
  );


  // POINT RECOGNITION TAB
  QTabWidget* tabs = new QTabWidget(this);
//  tabs->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  layout->addWidget(tabs);


  // THRESHOLDS
  QWidget* thresholds = new QWidget(tabs);
  QHBoxLayout* thresholdsLayout = new QHBoxLayout(thresholds);
  thresholds->setLayout(thresholdsLayout);
  tabs->addTab(thresholds, "Thresholds");

  thresholdsLayout->addWidget(
        new DefoImageThresholdsWidget(selectionModel_, pointModel_, thresholds)
  );

  QWidget* thresholdSpinners = new QWidget(thresholds);
  thresholdsLayout->addWidget(thresholdSpinners);
  thresholdSpinners->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  QVBoxLayout* thresholdLayout = new QVBoxLayout(thresholdSpinners);
  thresholdSpinners->setLayout(thresholdLayout);

  QString format("Threshold %1");
  for (int i=0; i<3; i++) {
    thresholdLayout->addWidget(new QLabel(format.arg(i+1)));
    thresholdLayout->addWidget(new DefoThresholdSpinBox(
        pointModel_
      , static_cast<DefoPointRecognitionModel::Threshold>(i)
    ));
  }

  // POINT FINDING
  QWidget* points = new QWidget(tabs);
  tabs->addTab(points, "Points");
  QVBoxLayout* pointsLayout = new QVBoxLayout(points);
  points->setLayout(pointsLayout);

  pointsImage_ = new DefoImagePointsWidget(listModel_, selectionModel_, points);
  pointsLayout->addWidget(pointsImage_);
  QPushButton* findPoints = new QPushButton("Find &pionts", points);
  pointsLayout->addWidget(findPoints);
//  pointsLayout->addWidget(
//        new DefoMeasurementListComboBox(listModel_, selectionModel_, points)
//  );

  connect(findPoints, SIGNAL(clicked()), SLOT(pointButtonClicked()));

  QPushButton* savePoints = new QPushButton("Save points", points);
  pointsLayout->addWidget(savePoints);
  connect(savePoints, SIGNAL(clicked()), SLOT(writePoints()));

//  photographer = new Photographer(listModel_, cameraModel_, 5);

  setCentralWidget( central );

}

void TestWindow::pointButtonClicked() {

  DefoMeasurement* measurement = selectionModel_->getSelection();
  listModel_->setMeasurementPoints(measurement, NULL);

  DefoPointFinder* finder;
  QRect searchArea = measurement->getImage().rect();
  const int width = searchArea.width();

  /*
    The number of blocks depends on the number of cores available and the
    number of cores you want to be available for other tasks.
    */
  const int blocks = 6;

  QMutex mutex;

  for (double i = 0; i < blocks; ++i) {

    /*
      In Qt the rectangle borders are placed ON the pixels, in contrast to e.g.
      Swing where they are in between the pixels.
      As a consequence, the right-side border has to be one pixel to the left in
      order to avoid overlapping rectangles.
      */
    searchArea.setLeft( i/blocks * width );
    searchArea.setRight( (i+1)/blocks * width - 1 );

    finder = new DefoPointFinder(i,
				 &mutex,
				 listModel_,
				 pointModel_,
				 measurement,
				 searchArea);

    finder->start();

  }

}

void TestWindow::newCameraImage(QString location, bool keep) {

  if (!keep) {
    
    DefoMeasurement * measurement = new DefoMeasurement(location, false);
  
    listModel_->addMeasurement(measurement);
    selectionModel_->setSelection(measurement);

  } else {

    DefoMeasurement * measurement = new DefoMeasurement(location, true);
    
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
    
    measurement->write(currentDir_.absolutePath());

    listModel_->addMeasurement(measurement);
    selectionModel_->setSelection(measurement);

    listModel_->write(currentDir_.absolutePath());
  }
}

/* Coordinate binning test */
const QString CoordinateSaver::LINE_FORMAT = "%1\t%2\t%3\t%4\t%5\n";

CoordinateSaver::CoordinateSaver(const QString &filename, QObject* parent) :
  QFile(filename, parent)
{
  open( QIODevice::WriteOnly | QIODevice::Truncate );
}

CoordinateSaver::~CoordinateSaver() {
  close();
}

void CoordinateSaver::writePoint(const DefoPoint& point)
{
  double x = point.getX();
  double y = point.getY();
  QColor color = point.getColor();
  int hue = color.hsvHue();
  int saturation = color.hsvSaturation();
  int value = color.value();

  QString line = LINE_FORMAT
      .arg(x, 0, 'e', 5)
      .arg(y, 0, 'e', 5)
      .arg(hue)
      .arg(saturation)
      .arg(value);
  write(line.toStdString().c_str());
}

void TestWindow::writePoints()
{

  QString file = QFileDialog::getSaveFileName(this, "Choose location");

  if (file.length() > 0) {
    CoordinateSaver saver(file);
    DefoMeasurement* meas = selectionModel_->getSelection();
    const DefoPointCollection* points = listModel_->getMeasurementPoints(meas);

    for ( DefoPointCollection::const_iterator it = points->begin()
        ; it < points->end()
        ; ++it
    ) {
      saver.writePoint(*it);
    }
  }

}

/**
  Constructs a new timed picture acquisition controller.
  \arg listModel The measurement list model.
  \arg cameraModel The camera device model.
  \arg interval The interval between two consecutive images, in minutes.
  */
Photographer::Photographer(
    DefoMeasurementListModel *listModel
  , DefoCameraModel* cameraModel
  , int interval
  , QObject* parent
) :
    QObject(parent)
  , listModel_(listModel)
  , cameraModel_(cameraModel)
  , interval_(1000*60*interval)
  , timer_(this)
{

    timer_.setInterval(interval_);
    timer_.setSingleShot(false);
    connect(&timer_, SIGNAL(timeout()), SLOT(takePicture()));
    connect(
          cameraModel_
        , SIGNAL(deviceStateChanged(State))
        , SLOT(setCameraState(State))
    );

    setCameraState(cameraModel_->getDeviceState());

}

// Growing interval picture taking test for camera timeout
void Photographer::takePicture() {

//  pictureInterval_ *= 2;
//  timer_->setInterval(pictureInterval_);

  cameraModel_->acquirePicture(true);

}

void Photographer::setCameraState(State state) {
  if (state == READY)
    timer_.start();
  else
    timer_.stop();
}

ScriptWindow::ScriptWindow(QWidget *parent) :
    QMainWindow(parent)
{
  QHBoxLayout *layout = new QHBoxLayout();
  QWidget *central = new QWidget();
  central->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  central->setLayout(layout);
  
  //DefoCameraModel* cameraModel = new DefoCameraModel(this);

  //DefoScriptModel* script = new DefoScriptModel(cameraModel, this);
  //DefoScriptWidget* scriptWidget = new DefoScriptWidget(script, this);
  //scriptWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  //layout->addWidget(scriptWidget);

  setCentralWidget( central );
}
