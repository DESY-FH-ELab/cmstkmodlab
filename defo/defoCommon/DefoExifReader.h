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

#ifndef DEFOEXIFREADER_H
#define DEFOEXIFREADER_H

#include <exiv2/exif.hpp>

#include <QString>

class DefoExifReader {

public:
  DefoExifReader(
      const QString& filename
  );

  bool read();

  QString getStringValue(const char * key);
  float getFloatValue(const char * key);
  long getLongValue(const char * key);

protected:
  const QString filename_;
  Exiv2::ExifData exifData_;
};

#endif // DEFOEXIFREADER_H
