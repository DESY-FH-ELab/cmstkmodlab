#include "DefoCameraWidget.h"

DefoCameraWidget::DefoCameraWidget(
    DefoCameraModel* cameraModel
  , DefoConradModel* conradModel
  , DefoMeasurementListModel* listModel
  , DefoMeasurementSelectionModel* selectionModel
  , QWidget *parent
) :
    QWidget(parent)
  , cameraModel_(cameraModel)
  , conradModel_(conradModel)
  , selectionModel_(selectionModel)
  , listModel_(listModel)
{

  // Layouts to put everything into place
  QVBoxLayout* layout = new QVBoxLayout();
  setLayout(layout);
//  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  // Camera buttons
  buttons_ = new QWidget(this);
  buttons_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  layout->addWidget(buttons_);

  QGridLayout *buttonLayout = new QGridLayout();
  buttons_->setLayout(buttonLayout);

  // Loading an image
  QPushButton *loadFileButton = new QPushButton("&Load file", buttons_);
  connect(loadFileButton, SIGNAL(clicked()), this, SLOT(openButtonClicked()));

  buttonLayout->addWidget(loadFileButton, 0, 0);

//  enableCheckBox_ = new DefoConradSwitchCheckbox(
//        conradModel_
//      , DefoConradModel::CAMERA
//      , "Enable &camera"
//      , this
//  );
  enableCheckBox_ = new QCheckBox("Enable camera", this);
  connect(
          enableCheckBox_
        , SIGNAL(toggled(bool))
        , cameraModel_
        , SLOT(setDeviceEnabled(bool))
  );
  // TODO connect camera model state change
  enableCheckBox_->setChecked(cameraModel_->getDeviceState() == READY);
  buttonLayout->addWidget(enableCheckBox_, 0, 3);

  connect(
        cameraModel_
      , SIGNAL(deviceStateChanged(State))
      , this
      , SLOT(deviceStateChanged(State))
  );

  // Taking a picture
  pictureButton_ = new QPushButton("&Take picture", buttons_);
  buttonLayout->addWidget(pictureButton_, 0, 1);
  buttonLayout->addWidget(loadFileButton, 0, 0);

//  QPushButton *saveFileButton = new QPushButton("&Save picture", buttons_);
//  connect(saveFileButton, SIGNAL(clicked()), this, SLOT(saveButtonClicked()));
//  buttonLayout->addWidget(saveFileButton, 0, 2);


  connect(
          pictureButton_
        , SIGNAL(clicked())
        , cameraModel_
        , SLOT(acquirePicture())
  );

  // Camera options
  buttonLayout->addWidget( new QLabel("Aperture", buttons_), 1, 0 );
  buttonLayout->addWidget(new DefoCameraOptionComboBox(
        cameraModel_
      , DefoCameraModel::APERTURE
      , this
  ), 2, 0);
  buttonLayout->addWidget( new QLabel("ISO", buttons_), 1, 1 );
  buttonLayout->addWidget(new DefoCameraOptionComboBox(
        cameraModel_
      , DefoCameraModel::ISO
      , this
  ), 2, 1);
  buttonLayout->addWidget( new QLabel("Shutter speed", buttons_), 1, 2 );
  buttonLayout->addWidget(new DefoCameraOptionComboBox(
        cameraModel_
      , DefoCameraModel::SHUTTER_SPEED
      , this
  ), 2, 2);
  buttonLayout->addWidget( new QLabel("White balance", buttons_), 1, 3 );
  buttonLayout->addWidget(new DefoCameraOptionComboBox(
        cameraModel_
      , DefoCameraModel::WHITE_BALANCE
      , this
  ), 2, 3);

  buttonLayout->addWidget(
        new DefoMeasurementListComboBox(listModel_, selectionModel_, buttons_)
      , 3
      , 0
      , 1
      , 4
  );

  // Image display
  layout->addWidget( new DefoRawImageWidget(selectionModel_, this) );

  deviceStateChanged(cameraModel_->getDeviceState());

}

void DefoCameraWidget::deviceStateChanged(State newState) {

  enableCheckBox_->setChecked( newState == INITIALIZING || newState == READY );
  pictureButton_->setEnabled( newState == READY );

}

void DefoCameraWidget::openButtonClicked() {

  QString location( QFileDialog::getOpenFileName(this, "Choose image") );
  const DefoMeasurement* measurement = new DefoMeasurement(location);
  listModel_->addMeasurement( measurement );
  selectionModel_->setSelection( measurement );

}

//void DefoCameraWidget::saveButtonClicked() {

//  QString file = QFileDialog::getSaveFileName(this, "Choose location");
//  const DefoMeasurement* m = selectionModel_->getSelection();
//  m->getImage().save(file);

//}
