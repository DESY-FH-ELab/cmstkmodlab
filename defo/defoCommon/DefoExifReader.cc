#include <iostream>
#include <iomanip>

#include <image.hpp>
#include <exif.hpp>
#include <error.hpp>
#include <tags.hpp>

#include "DefoExifReader.h"

DefoExifReader::DefoExifReader(
    const QString& filename
) :
  filename_(filename)
{}

bool DefoExifReader::read() {

  try {

    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename_.toStdString().c_str());

    image->readMetadata();

    exifData_ = image->exifData();
    if (exifData_.empty()) {
      throw Exiv2::Error(1, "no Exif data in file");
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
