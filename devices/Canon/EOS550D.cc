
#include "EOS550D.h"

// DEVICE CONTROLLER CLASS

EOS550D::EOS550D(const char* port) :
    VEOS550D(port)
  , handler_(NULL)
  , optionNames_(4) //create room for the total number of support options
  , optionLists_(4)
  , tempPictureFiles_()
  , previewFileName_("/tmp/canon-preview.jpg")
  , isInPreviewMode_(false)

{

  // Save names for supported options
  optionNames_[APERTURE] = "aperture";
  optionNames_[SHUTTER_SPEED] = "shutterspeed";
  optionNames_[ISO] = "iso";
  optionNames_[WHITE_BALANCE] = "whitebalance";

  handler_ = new CameraComHandler("Canon EOS 550D", port);

}

EOS550D::~EOS550D() {

  // Close connection to device
  delete handler_;

  // Clear all temporary files
  for ( std::deque<std::string>::const_iterator it = tempPictureFiles_.begin()
      ; it < tempPictureFiles_.end()
      ; ++it
  ) {
    // TODO log success/failure
    remove( it->c_str() );
  }

}

bool EOS550D::initialize() {

  bool success = handler_->initialize();

  if ( success ) {
    // Cache list of values per option
    for (unsigned int i = 0; i < optionNames_.size(); ++i ) {
      std::string name = optionNames_[i];
      optionLists_[i] = handler_->readConfigOptions(name.c_str());
    }
  }

  return success;

}

std::vector<std::string> EOS550D::readOptions(
    const EOS550D::Option &option
) const {
  return optionLists_[option];
}

int EOS550D::readOption(const VEOS550D::Option &option) {
  std::string value = handler_->readConfigValue(optionNames_[option].c_str());
  return indexOf(optionLists_[option], value);
}

bool EOS550D::writeOption(const VEOS550D::Option &option, int value) {
//  if ( value > -1) {
    return handler_->writeConfigValue(
          optionNames_[option].c_str()
        , optionLists_[option][value].c_str()
    );
//  }
//  else {
//    return false;
//  }
}

/**
  \brief Captures an image with the camera and returns the download location.
  The camera is instructed to take a picture with the current settings. After
  acquisition, the picture is downloaded to a temporary file and removed from
  the camera. The location of the temporary file is then returned.
  */
std::string EOS550D::acquirePhoto() {

  // Create temporary file
  char tempFileName[] = "/tmp/canon-XXXXXX";
  int filedescriptor = mkstemp(tempFileName);

  handler_->acquireAndDownloadPicture(filedescriptor);

  close(filedescriptor);

  std::string fnCopy(tempFileName);
  tempPictureFiles_.push_back(fnCopy);
//  std::cout << "Created " << fnCopy << std::endl;

  return fnCopy;

}

/**
  \brief Captures a preview image with the camera and returns the download
  location.
  */
std::string EOS550D::acquirePreview() {

  handler_->acquirePreview(previewFileName_.c_str());

  return previewFileName_;
}

bool EOS550D::isInPreviewMode() {

  return isInPreviewMode_;
}

/**
  \brief Switches the camera into preview mode (a.k.a. live view).
  */
bool EOS550D::startPreviewMode() {

  if (isInPreviewMode_) return false;

  handler_->startPreviewMode();

  isInPreviewMode_ = true;
  
  return true;
}

/**
  \brief Switches off preview mode (a.k.a. live view). The file
  with the preview image is deleted.
  */
bool EOS550D::stopPreviewMode() {

  if (!isInPreviewMode_) return false;
 
  handler_->stopPreviewMode();

  remove(previewFileName_.c_str());

  isInPreviewMode_ = false;
 
  return true;
}
