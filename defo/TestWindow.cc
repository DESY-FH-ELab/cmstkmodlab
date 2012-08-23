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
//  buttonLayout->addWidget(new DefoCameraOptionComboBox(
//        cameraModel_
//      , DefoCameraModel::WHITE_BALANCE
//      , this
//  ));

  setCentralWidget( central );

}

void TestWindow::fileButtonClicked() {

  QImage image(QFileDialog::getOpenFileName(this, "Choose reference image"));
  DefoMeasurement measurement(image);
  listModel_->addMeasurement( measurement );

}

void TestWindow::newCameraImage(QImage newImage) {

  DefoMeasurement measurement(newImage);
  listModel_->addMeasurement( measurement );

}
