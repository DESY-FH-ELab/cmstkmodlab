#include "DefoCameraModel.h"

DefoCameraModel::DefoCameraModel(QObject *parent) :
    QObject(parent)
  , DefoAbstractDeviceModel()
{

  int initValue = 0;
  parameters_[APERTURE] = initValue;
  parameters_[ISO] = initValue;
  parameters_[SHUTTER_SPEED] = initValue;
  parameters_[WHITE_BALANCE] = initValue;

  setDeviceEnabled(true);

}


void DefoCameraModel::resetParameterCache() {

  int resetValue = 0;
  setOptionSelection(APERTURE, resetValue);
  setOptionSelection(ISO, resetValue);
  setOptionSelection(SHUTTER_SPEED, resetValue);
  setOptionSelection(WHITE_BALANCE, resetValue);

}


/// Trivial reimplementation as a slot.
void DefoCameraModel::setDeviceEnabled(bool enabled) {
  DefoAbstractDeviceModel::setDeviceEnabled(enabled);
}


/// Initialize the model by retrieving the current settings.
void DefoCameraModel::initialize() {

  setDeviceState(INITIALIZING);

  renewController("usb:");

  if( controller_->initialize() ) {

    setOptionSelection(
            APERTURE
          , controller_->readOption(EOS550D::APERTURE)
    );
    setOptionSelection(
            ISO
          , controller_->readOption(EOS550D::ISO)
    );
    setOptionSelection(
            SHUTTER_SPEED
          , controller_->readOption(EOS550D::SHUTTER_SPEED)
    );
    setOptionSelection(
            WHITE_BALANCE
          , controller_->readOption(EOS550D::WHITE_BALANCE)
    );

    setDeviceState(READY);

  }
  else {
    resetParameterCache();
    setDeviceState(OFF);
  }

}


std::vector<std::string> DefoCameraModel::getOptions(
    const Option &option
) const {

  return controller_->readOptions(static_cast<EOS550D::Option>(option));

}


void DefoCameraModel::setOptionSelection(
    const Option &option
  , int value
) {

  // Only update if intention to change
  if ( parameters_[option] != value ) {

    int oldValue = parameters_[option];
    bool success = controller_->writeOption(
            static_cast<EOS550D::Option>(option)
          , value
    );
//    std::cout << success << std::endl;

    if (success)
      parameters_[option] = value;
    // else don't change

//    std::cout << parameters_[option] << std::endl;

    emit deviceOptionChanged(option, parameters_[option]);

  }

}


void DefoCameraModel::setDeviceState(State state) {

  if (state != state_) {
    state_ = state;
    emit deviceStateChanged(state);
  }

}


/// Instruct the camera to take a picture and cache the file in a QImage.
void DefoCameraModel::acquirePicture() {

    location_ = controller_->acquirePhoto().c_str();
    image_ = QImage(location_);
    emit newImage(location_);

}


const QImage & DefoCameraModel::getLastPicture() const {
  return image_;
}


int DefoCameraModel::getOptionValue(const Option &option) const {
  return controller_->readOption(static_cast<EOS550D::Option>(option));
}

const QString & DefoCameraModel::getLastPictureLocation() const {
  return location_;
}
