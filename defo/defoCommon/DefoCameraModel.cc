#include <chrono>
#include <thread>

#include <QCoreApplication>
#include <QPlainTextDocumentLayout>

#include <nqlogger.h>

#include "DefoCameraModel.h"

DefoCameraModel::DefoCameraModel(QObject *parent)
  : QObject(parent),
    AbstractDeviceModel()
{
  comment_ = new QTextDocument(this);
  comment_->setDocumentLayout(new QPlainTextDocumentLayout(comment_));
  comment_->setDefaultFont(QFont("Courier New",10));

  calibAmplitude_ = 0;
  
  int initValue = 0;
  parameters_[APERTURE] = initValue;
  parameters_[ISO] = initValue;
  parameters_[SHUTTER_SPEED] = initValue;
  parameters_[WHITE_BALANCE] = initValue;
 
  liveViewTimer_.setInterval(1000);

  connect(&liveViewTimer_, SIGNAL(timeout()),
          this, SLOT(acquireLiveViewPicture()));
 
  setDeviceEnabled(true);
}

void DefoCameraModel::resetParameterCache()
{
  int resetValue = 0;
  setOptionSelection(APERTURE, resetValue);
  setOptionSelection(ISO, resetValue);
  setOptionSelection(SHUTTER_SPEED, resetValue);
  setOptionSelection(WHITE_BALANCE, resetValue);
}

/// Trivial reimplementation as a slot.
void DefoCameraModel::setDeviceEnabled(bool enabled)
{
  AbstractDeviceModel::setDeviceEnabled(enabled);
}

void DefoCameraModel::setControlsEnabled(bool enabled)
{
  emit controlStateChanged(enabled);
}

void DefoCameraModel::setLiveViewEnabled(bool enabled)
{
  if (enabled && !controller_->isInPreviewMode()) {
    controller_->startPreviewMode();

    liveViewTimer_.start();

    emit liveViewModeChanged(true);
  } else if (!enabled && controller_->isInPreviewMode()) {
    controller_->stopPreviewMode();

    liveViewTimer_.stop();

    emit liveViewModeChanged(false);
  }
}

/// Initialize the model by retrieving the current settings.
void DefoCameraModel::initialize()
{
  setDeviceState(INITIALIZING);

  renewController("usb:");

  if (controller_->initialize()) {

    setOptionSelection(APERTURE,
		       controller_->readOption(VEOS550D::APERTURE));
    setOptionSelection(ISO,
		       controller_->readOption(VEOS550D::ISO));
    setOptionSelection(SHUTTER_SPEED,
		       controller_->readOption(VEOS550D::SHUTTER_SPEED));
    setOptionSelection(WHITE_BALANCE,
		       controller_->readOption(VEOS550D::WHITE_BALANCE));

    setDeviceState(READY);

  } else {
    resetParameterCache();
    setDeviceState(OFF);
  }
}


std::vector<std::string> DefoCameraModel::getOptions(const Option &option) const
{
  return controller_->readOptions(static_cast<VEOS550D::Option>(option));
}


void DefoCameraModel::setOptionSelection(const Option &option, int value)
{
  // Only update if intention to change
  if ( parameters_[option] != value ) {

    // int oldValue = parameters_[option];
    bool success = controller_->writeOption(static_cast<VEOS550D::Option>(option),
					    value);

    if (success)
      parameters_[option] = value;
    // else don't change

    emit deviceOptionChanged(option, parameters_[option]);
  }
}


void DefoCameraModel::setDeviceState(State state)
{
  if (state != state_) {
    state_ = state;
    emit deviceStateChanged(state_);
  }
}

/// Instruct the camera to take a picture and cache the file in a QImage.
void DefoCameraModel::acquirePicture(bool keep)
{
  location_ = controller_->acquirePhoto().c_str();
  image_ = QImage(location_);
  emit newImage(location_, keep);
  emit defoMessage("new image aquired");
}

void DefoCameraModel::acquirePictures(int count)
{
  if (count==1) {
    acquirePicture(true);
    return;
  }

  locations_.clear();
  QStringList list;
  for (int i=0;i<count;i++) {
    location_ = controller_->acquirePhoto().c_str();
    image_ = QImage(location_);
    locations_.append(location_);
    list.append(location_);

    QCoreApplication::processEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  emit newImages(list);
  emit defoMessage("new images aquired");
}

/// Instruct the camera to take a picture and cache the file in a QImage.
void DefoCameraModel::acquireLiveViewPicture()
{
  location_ = controller_->acquirePreview().c_str();
  liveViewImage_ = QImage(location_);
  emit newLiveViewImage(location_);
}

void DefoCameraModel::setComment(const QString& comment)
{
  comment_->setPlainText(comment);
}

void DefoCameraModel::setCalibAmplitude(int amplitude)
{
  calibAmplitude_ = amplitude;
}

const QImage & DefoCameraModel::getLastPicture() const
{
  return image_;
}

const QImage & DefoCameraModel::getLastLiveViewPicture() const
{
  return liveViewImage_;
}

int DefoCameraModel::getOptionValue(const Option &option) const
{
  return controller_->readOption(static_cast<VEOS550D::Option>(option));
}

const QString & DefoCameraModel::getLastPictureLocation() const
{
  return location_;
}

const QStringList & DefoCameraModel::getLastPictureLocations() const
{
  return locations_;
}
