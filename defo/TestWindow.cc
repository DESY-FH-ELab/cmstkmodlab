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
  model_ = new DefoSurfaceModel();
  layout->addWidget( new DefoImageWidget(model_) );
  QPushButton *fileButton = new QPushButton("&Load reference", buttons);
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
  buttonLayout->addWidget(new DefoCameraOptionComboBox(
        cameraModel_
      , DefoCameraModel::WHITE_BALANCE
      , this
  ));

  QPushButton *cameraButton = new QPushButton("&Take picture", buttons);
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
//  connect(
//          cameraButton
//        , SIGNAL(clicked())
//        , this
//        , SLOT(cameraButtonClicked())
//  );
  buttonLayout->addWidget( cameraButton );


  setCentralWidget( central );

}

TestWindow::~TestWindow() {
//  delete model_;
}

void TestWindow::fileButtonClicked()
{

  QImage image(QFileDialog::getOpenFileName(this, "Choose reference image"));
  DefoMeasurement* reference = new DefoMeasurement(image);
  QRect* area = NULL; //new QRect( 0, 0, 200, 200 );

//  reference->findPoints( area );

  model_->setReferenceMeasurement( reference );

}

void TestWindow::newCameraImage(QImage newImage)
{

  QImage image(newImage);

  DefoMeasurement* reference = new DefoMeasurement(image);

//  reference->findPoints( area );

  model_->setReferenceMeasurement( reference );

}

void TestWindow::cameraButtonClicked( )
{

  std::string location = camera_->acquirePhoto();
  QImage image(QString(location.c_str()));

  DefoMeasurement* reference = new DefoMeasurement(image);

//  reference->findPoints( area );

  model_->setReferenceMeasurement( reference );

}
