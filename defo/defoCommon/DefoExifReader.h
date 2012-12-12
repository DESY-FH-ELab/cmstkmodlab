#ifndef DEFOEXIFREADER_H
#define DEFOEXIFREADER_H

#include <exif.hpp>

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
