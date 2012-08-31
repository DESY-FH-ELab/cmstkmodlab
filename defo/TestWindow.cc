#include "TestWindow.h"

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent)
{

  // Set up current directory for saving images
  QDateTime dt = QDateTime::currentDateTimeUtc();
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
//  layout->addWidget( new DefoJulaboWidget(new DefoJulaboModel(3)) );

  // KEITHLEY MODEL
//  layout->addWidget( new DefoKeithleyWidget(new DefoKeithleyModel(5)) );

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

  connect(cameraModel_, SIGNAL(newImage(QString)), SLOT(newCameraImage(QString)));

  pointModel_ = new DefoPointRecognitionModel(this);

  // read default settings
  DefoConfigReader cfgReader( "defo.cfg" );
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

  setCentralWidget( central );

//  timer_ = new QTimer(this);

//  pictureInterval_ = 1000*60;

//  timer_->setInterval(pictureInterval_);
//  timer_->setSingleShot(false);
//  connect(timer_, SIGNAL(timeout()), SLOT(timedPicture()));
//  timer_->start();

}

void TestWindow::pointButtonClicked() {

  const DefoMeasurement* measurement = selectionModel_->getSelection();
  listModel_->setMeasurementPoints(measurement, NULL);

  PointFinder* finder;
  QRect searchArea = measurement->getImage().rect();
  const int width = searchArea.width();

  /*
    The number of blocks depends on the number of cores available and the
    number of cores you want to be available for other tasks.
    */
  const int blocks = 6;

  for (double i = 0; i < blocks; ++i) {

    searchArea.setLeft( i/blocks * width );
    searchArea.setRight( (i+1)/blocks * width );

    finder = new PointFinder(
        listModel_
      , pointModel_
      , measurement
      , searchArea
    );

    finder->start();

  }

}

void TestWindow::newCameraImage(QString location) {

  const DefoMeasurement* measurement = new DefoMeasurement(location);

  // TODO save when needed, i.e. always from now on
  QDateTime dt = measurement->getTimeStamp();

  // Create with first picture taken
  if (!currentDir_.exists())
    currentDir_.mkpath(currentDir_.absolutePath());

  QString imageLocation = currentDir_.absoluteFilePath("%1.jpg");
  imageLocation = imageLocation.arg(dt.toString("yyyyMMddhhmmss"));

  QFile file(cameraModel_->getLastPictureLocation());
  file.copy(imageLocation);
  file.close();

  listModel_->addMeasurement( measurement );
  selectionModel_->setSelection(measurement);

}

//// Growing interval picture taking test for camera timeout
//void TestWindow::timedPicture()
//{

//  pictureInterval_ *= 2;
//  timer_->setInterval(pictureInterval_);

//  cameraModel_->acquirePicture();

//}

/* POINT FINDING THREAD */
PointFinder::PointFinder(
    DefoMeasurementListModel *listModel
  , DefoPointRecognitionModel *pointModel
  , const DefoMeasurement *measurement
  , const QRect &searchRectangle
) :
    listModel_(listModel)
  , pointModel_(pointModel)
  , measurement_(measurement)
  , searchArea_(searchRectangle)
{}

void PointFinder::run() {

  const DefoPointCollection* points = measurement_->findPoints(
      &searchArea_
    , pointModel_->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_1)
    , pointModel_->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_2)
    , pointModel_->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_3)
    , pointModel_->getHalfSquareWidth()
  );

  listModel_->appendMeasurementPoints(measurement_, points);

}


/* Coordinate binning test */
const QString CoordinateSaver::LINE_FORMAT = "%1\t%2\n";

CoordinateSaver::CoordinateSaver(const QString &filename, QObject* parent) :
  QFile(filename, parent)
{
  open( QIODevice::WriteOnly | QIODevice::Truncate );
}

CoordinateSaver::~CoordinateSaver() {
  close();
}

void CoordinateSaver::writePoint(double x, double y)
{
  QString line = LINE_FORMAT.arg(x, 0, 'e', 5).arg(y, 0, 'e', 5);
  write(line.toAscii());
}

void TestWindow::writePoints()
{

  CoordinateSaver saver("/home/tkmodlab/Desktop/coords.txt");
  const DefoMeasurement* meas = selectionModel_->getSelection();
  const DefoPointCollection* points = listModel_->getMeasurementPoints(meas);

  for (DefoPointCollection::const_iterator it = points->begin(); it < points->end(); ++it)
    saver.writePoint(it->getX(), it->getY());

}
