/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef _VEOS550D_H
#define _VEOS550D_H

#include <vector>
#include <string>

///
/// Base class for EOS550D
///
class VEOS550D {

 public:

  /// Enumeration of currently supported settings.
  enum Option {
    APERTURE,
    SHUTTER_SPEED,
    ISO,
    WHITE_BALANCE
  };

  VEOS550D(const char* port);
  virtual ~VEOS550D();

  virtual bool initialize() = 0;

  /// Read the allowed values for the given option.
  virtual std::vector<std::string> readOptions(const Option& option) const = 0;
  /// Read the current value for the given option.
  virtual int readOption(const Option& option) = 0;
  /// Set the given option to the new value.
  virtual bool writeOption(const Option& option, int value) = 0;

  /// Returns the absolute (local) location of the retrieved picture.
  virtual std::string acquirePhoto() = 0;

  /// Returns the absolute (local) location of the retrieved preview picture.
  virtual std::string acquirePreview() = 0;
  
  /// Returns true if camera is in preview mode (a.k.a. live view)
  virtual bool isInPreviewMode() = 0;
  /// Switches camera into preview mode (a.k.a. live view)
  virtual bool startPreviewMode() = 0;
  /// Switches off preview mode (a.k.a. live view)
  virtual bool stopPreviewMode() = 0;

protected:

  typedef std::vector<std::string> OptionList;
  static int indexOf(const OptionList& list, const std::string& value);
};

#endif
