#ifndef _EOS550D_H
#define _EOS550D_H

#include <string>
#include <deque>
#include <stdlib.h>
#include <stdio.h>

#include "CameraComHandler.h"
#include "VEOS550D.h"

///
/// Real device class for the Canon EOS 550D camera.
///
class EOS550D : public VEOS550D {

public:
  explicit EOS550D(const char* port);
  ~EOS550D();

  bool initialize();

  std::vector<std::string> readOptions(const Option& option) const;
  int readOption(const Option& option);
  bool writeOption(const Option& option, int value);

  /// Returns the absolute (local) location of the retrieved picture.
  virtual std::string acquirePhoto();

protected:
  CameraComHandler* handler_;

  std::vector<std::string> optionNames_;
  std::vector<OptionList> optionLists_;

  std::deque<std::string> tempPictureFiles_;

};

#endif
