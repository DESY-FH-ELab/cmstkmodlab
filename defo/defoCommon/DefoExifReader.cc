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

#include <iostream>
#include <iomanip>

#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>
#include <exiv2/error.hpp>
#include <exiv2/tags.hpp>

#include "DefoExifReader.h"

DefoExifReader::DefoExifReader(const QString& filename)
  : filename_(filename)
{

}

bool DefoExifReader::read()
{
  try {

    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename_.toStdString().c_str());

    image->readMetadata();

    exifData_ = image->exifData();
    if (exifData_.empty()) {
      throw Exiv2::Error(Exiv2::kerErrorMessage, "no Exif data in file");
    }
    // Exiv2::ExifData::const_iterator end = exifData_.end();
    // for (Exiv2::ExifData::const_iterator i = exifData_.begin(); i != end; ++i) {
    //   const char* tn = i->typeName();
    //   std::cout << std::setw(44) << std::setfill(' ') << std::left
    // 		<< i->key() << " "
    // 		<< "0x" << std::setw(4) << std::setfill('0') << std::right
    // 		<< std::hex << i->tag() << " "
    // 		<< std::setw(9) << std::setfill(' ') << std::left
    // 		<< (tn ? tn : "Unknown") << " "
    // 		<< std::dec << std::setw(3)
    // 		<< std::setfill(' ') << std::right
    // 		<< i->count() << "  "
    // 		<< std::dec << i->value()
    // 		<< "\n";
    // }
    
    return true;
  }
  catch (Exiv2::AnyError& e) {
    std::cout << "Caught Exiv2 exception '" << e.what() << "'\n";
    return false;
  }
}

QString DefoExifReader::getStringValue(const char * key)
{
  Exiv2::ExifData::const_iterator find = exifData_.findKey(Exiv2::ExifKey(key));
  if (find==exifData_.end()) return QString();
  
  return QString(find->value().toString().c_str());
}

float DefoExifReader::getFloatValue(const char * key)
{
  Exiv2::ExifData::const_iterator find = exifData_.findKey(Exiv2::ExifKey(key));
  if (find==exifData_.end()) return -1;
  
  return find->value().toFloat();
}

long DefoExifReader::getLongValue(const char * key)
{
  Exiv2::ExifData::const_iterator find = exifData_.findKey(Exiv2::ExifKey(key));
  if (find==exifData_.end()) return -1;
  
  return find->value().toLong();
}
