#include "TestWindow.h"

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent)
{

  QHBoxLayout *layout = new QHBoxLayout();
  QWidget *central = new QWidget();
  central->setLayout(layout);

  QWidget* buttons = new QWidget(this);
  layout->addWidget(buttons);
  buttons->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum);
  QVBoxLayout *buttonLayout = new QVBoxLayout();
  buttons->setLayout(buttonLayout);


  // MEASUREMENT MODEL
  listModel_ = new DefoMeasurementListModel();
  layout->addWidget( new DefoRawImageWidget(listModel_, this) );

  QPushButton *fileButton = new QPushButton("&Load file", buttons);
  fileButton->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum);
  connect(fileButton, SIGNAL(clicked()), this, SLOT(fileButtonClicked()));

  buttonLayout->addWidget( fileButton );

  // CONRAD MODEL
//  layout->addWidget( new DefoConradWidget(new DefoConradModel()) );

  // JULABO MODEL
//  layout->addWidget( new DefoJulaboWidget(new DefoJulaboModel(3)) );

  // KEITHLEY MODEL
//  layout->addWidget( new DefoKeithleyWidget(new DefoKeithleyModel(5)) );

  // CANON CAMERA MODEL
  cameraModel_ = new DefoCameraModel(this);

  QPushButton *cameraButton = new QPushButton("&Take picture", buttons);
  cameraButton->setEnabled( cameraModel_->getDeviceState() == READY );
  cameraButton->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum);
  connect(
          cameraButton
        , SIGNAL(clicked())
        , cameraModel_
        , SLOT(acquirePicture())
  );
  connect(
          cameraModel_
        , SIGNAL(newImage(QImage))
        , this
        , SLOT(newCameraImage(QImage))
  );
  buttonLayout->addWidget( cameraButton );

  // Camera options
  buttonLayout->addWidget(new DefoCameraOptionComboBox(
        cameraModel_
      , DefoCameraModel::APERTURE
      , this
  ));
  buttonLayout->addWidget(new DefoCameraOptionComboBox(
        cameraModel_
      , DefoCameraModel::ISO
      , this
  ));
  buttonLayout->addWidget(new DefoCameraOptionComboBox(
        cameraModel_
      , DefoCameraModel::SHUTTER_SPEED
      , this
  ));

  if (cameraModel_->getDeviceState() == READY) {
    cameraModel_->setOptionSelection(DefoCameraModel::APERTURE, 6);
    cameraModel_->setOptionSelection(DefoCameraModel::ISO, 1);
    cameraModel_->setOptionSelection(DefoCameraModel::SHUTTER_SPEED, 26);
  }

  DefoPointRecognitionModel* pointModel = new DefoPointRecognitionModel(this);

  // read default settings
  DefoConfigReader cfgReader( "defo.cfg" );
  pointModel->setThresholdValue(
          DefoPointRecognitionModel::THRESHOLD_1
        , cfgReader.getValue<int>( "STEP1_THRESHOLD" )
  );
  pointModel->setThresholdValue(
          DefoPointRecognitionModel::THRESHOLD_2
        , cfgReader.getValue<int>( "STEP2_THRESHOLD" )
  );
  pointModel->setThresholdValue(
          DefoPointRecognitionModel::THRESHOLD_3
        , cfgReader.getValue<int>( "STEP3_THRESHOLD" )
  );
  //  buttonLayout->addWidget(new DefoCameraOptionComboBox(
  //        cameraModel_
  //      , DefoCameraModel::WHITE_BALANCE
  //      , this
  //  ));

  layout->addWidget(new DefoImageThresholdsWidget(listModel_, pointModel, this));

  // THRESHOLDS
  QWidget* thresholds = new QWidget(this);
  layout->addWidget(thresholds);
  thresholds->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );

  QVBoxLayout* thresholdLayout = new QVBoxLayout(thresholds);
  thresholds->setLayout(thresholdLayout);

  QString format("Threshold %1");
  for (int i=0; i<3; i++) {
    thresholdLayout->addWidget(new QLabel(format.arg(i+1)));
    thresholdLayout->addWidget(new DefoThresholdSlider(
        pointModel
       , static_cast<DefoPointRecognitionModel::Threshold>(i)
    ));
  }


  setCentralWidget( central );

}

void TestWindow::fileButtonClicked() {

  QImage image(QFileDialog::getOpenFileName(this, "Choose reference image"));
  DefoMeasurement measurement(image);
  listModel_->addMeasurement( measurement );

}

void TestWindow::newCameraImage(QImage newImage) {

  DefoMeasurement* measurement = new DefoMeasurement(newImage);
  listModel_->addMeasurement( *measurement );

}
