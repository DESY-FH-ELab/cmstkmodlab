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
  // setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  // Camera buttons
  buttons_ = new QWidget(this);
  buttons_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  layout->addWidget(buttons_);

  QGridLayout *buttonLayout = new QGridLayout();
  buttons_->setLayout(buttonLayout);

  enableCheckBox_ = new QCheckBox("Enable camera", this);
  connect(
          enableCheckBox_
        , SIGNAL(toggled(bool))
        , cameraModel_
        , SLOT(setDeviceEnabled(bool))
  );
  // TODO connect camera model state change
  enableCheckBox_->setChecked(cameraModel_->getDeviceState() == READY);
  buttonLayout->addWidget(enableCheckBox_, 0, 4);

  connect(
        cameraModel_
      , SIGNAL(deviceStateChanged(State))
      , this
      , SLOT(deviceStateChanged(State))
  );

  connect(
        cameraModel_
      , SIGNAL(controlStateChanged(bool))
      , this
      , SLOT(controlStateChanged(bool))
  );

  connect(
        cameraModel_
      , SIGNAL(liveViewModeChanged(bool))
      , this
      , SLOT(liveViewModeChanged(bool))
  );

  // Taking a preview picture
  previewButton_ = new QPushButton("&Take preview", buttons_);
  buttonLayout->addWidget(previewButton_, 0, 1);

//  QPushButton *saveFileButton = new QPushButton("&Save picture", buttons_);
//  connect(saveFileButton, SIGNAL(clicked()), this, SLOT(saveButtonClicked()));
//  buttonLayout->addWidget(saveFileButton, 0, 2);

  connect(
          previewButton_
        , SIGNAL(clicked())
        , this
        , SLOT(previewButtonClicked())
  );

  // Taking a picture
  pictureButton_ = new QPushButton("&Take picture", buttons_);
  buttonLayout->addWidget(pictureButton_, 0, 2);

  connect(
          pictureButton_
        , SIGNAL(clicked())
        , this
        , SLOT(pictureButtonClicked())
  );

  // Switching into preview mode
  liveviewCheckBox_ = new QCheckBox("Live", this);
  connect(liveviewCheckBox_
        , SIGNAL(toggled(bool))
        , cameraModel_
        , SLOT(setLiveViewEnabled(bool))
  );
  buttonLayout->addWidget(liveviewCheckBox_, 0, 3);

  commentEditor_ = new DefoImageCommentEdit(this);
  commentEditor_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  commentEditor_->setDocument(cameraModel_->commentDocument());
  commentEditor_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  commentEditor_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  layout->addWidget(commentEditor_);

  calibAmplitudeSpinBox_ = new QSpinBox(this);
  calibAmplitudeSpinBox_->setRange(-1000, 1000);
  calibAmplitudeSpinBox_->setSuffix("um");
  layout->addWidget(calibAmplitudeSpinBox_);
  connect(calibAmplitudeSpinBox_, SIGNAL(valueChanged(int)),
          cameraModel_, SLOT(setCalibAmplitude(int)));

  // Image display
  imageStack_ = new QStackedWidget(this);
 
  buttonsNRawImage_ = new QFrame(imageStack_);
  QVBoxLayout* frameLayout = new QVBoxLayout(buttonsNRawImage_);
  buttonsNRawImage_->setFrameShape(QFrame::Box);
  buttonsNRawImage_->setLayout(frameLayout);

  buttonLayout = new QGridLayout();
  buttonsCamera_ = new QWidget(buttonsNRawImage_);
  buttonsCamera_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  buttonsCamera_->setLayout(buttonLayout);

  // Camera options
  buttonLayout->addWidget( new QLabel("Aperture", buttonsCamera_), 0, 0 );
  buttonLayout->addWidget(new DefoCameraOptionComboBox(
        cameraModel_
      , DefoCameraModel::APERTURE
      , this
  ), 1, 0);
  buttonLayout->addWidget( new QLabel("ISO", buttonsCamera_), 0, 1 );
  buttonLayout->addWidget(new DefoCameraOptionComboBox(
        cameraModel_
      , DefoCameraModel::ISO
      , this
  ), 1, 1);
  buttonLayout->addWidget( new QLabel("Shutter speed", buttonsCamera_), 0, 2 );
  buttonLayout->addWidget(new DefoCameraOptionComboBox(
        cameraModel_
      , DefoCameraModel::SHUTTER_SPEED
      , this
  ), 1, 2);
  buttonLayout->addWidget( new QLabel("White balance", buttonsCamera_), 0, 3 );
  buttonLayout->addWidget(new DefoCameraOptionComboBox(
        cameraModel_
      , DefoCameraModel::WHITE_BALANCE
      , this
  ), 1, 3);

  buttonLayout->addWidget(
        new DefoMeasurementListComboBox(listModel_, selectionModel_, this)
      , 2
      , 0
      , 1
      , 4
  );

  frameLayout->addWidget(buttonsCamera_);

  rawImage_ = new DefoRawImageWidget(selectionModel_, buttonsNRawImage_);
  frameLayout->addWidget(rawImage_);

  imageStack_->addWidget(buttonsNRawImage_);
  
  liveImageFrame_ = new QFrame(imageStack_);
  frameLayout = new QVBoxLayout(liveImageFrame_);
  liveImageFrame_->setLayout(frameLayout);
  liveImageFrame_->setFrameShape(QFrame::Box);
  liveImage_ = new DefoLiveViewImageWidget(cameraModel_, liveImageFrame_);
  frameLayout->addWidget(liveImage_);

  imageStack_->addWidget(liveImageFrame_);

  imageStack_->setCurrentWidget(buttonsNRawImage_);

  layout->addWidget( imageStack_ );
  
  deviceStateChanged(cameraModel_->getDeviceState());
  
}

void DefoCameraWidget::deviceStateChanged(State newState) {

  enableCheckBox_->setChecked( newState == INITIALIZING || newState == READY );
  previewButton_->setEnabled( newState == READY );
  pictureButton_->setEnabled( newState == READY );
  liveviewCheckBox_->setEnabled( newState == READY );
}

void DefoCameraWidget::controlStateChanged(bool enabled) {

  if (enabled) {
    enableCheckBox_->setEnabled(true);
    deviceStateChanged(cameraModel_->getDeviceState());
  } else {
    enableCheckBox_->setEnabled(false);
    previewButton_->setEnabled(false);
    pictureButton_->setEnabled(false);
    liveviewCheckBox_->setEnabled(false);
  }
}

void DefoCameraWidget::liveViewModeChanged( bool enabled ) {

  if (enabled) {
    imageStack_->setCurrentWidget(liveImageFrame_);

    enableCheckBox_->setEnabled( false );
    previewButton_->setEnabled( false );
    pictureButton_->setEnabled( false );

  } else {
    imageStack_->setCurrentWidget(buttonsNRawImage_);

    enableCheckBox_->setEnabled( true );
    previewButton_->setEnabled( true );
    pictureButton_->setEnabled( true );
  }
}

void DefoCameraWidget::previewButtonClicked() {

  cameraModel_->acquirePicture(false);
}

void DefoCameraWidget::pictureButtonClicked() {

  cameraModel_->acquirePicture(true);
}
