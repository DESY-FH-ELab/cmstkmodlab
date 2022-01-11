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

#include <unistd.h>

#include <iostream>
#include <sstream>
#include <ctime>

#include "EOS550DFake.h"

using namespace std;
using namespace cv;

const string EOS550DFake::PREVIEW_FILE_NAME = "/tmp/canon-fake-preview.jpg";
const int EOS550DFake::PICTURE_WIDTH = 3000;

EOS550DFake::EOS550DFake(const char* port)
  : VEOS550D(port),
    maxTempFileCount_(20), // If bigger than 25, see TMP_MAX
    initialized_(false),
    inPreviewMode_(false)
{
  // Set all supported selections to -1, i.e. no selection
  optionSelections_[APERTURE] = -1;
  optionSelections_[ISO] = -1;
  optionSelections_[WHITE_BALANCE] = -1;
  optionSelections_[SHUTTER_SPEED] = -1;
}

EOS550DFake::~EOS550DFake()
{
  // Remove temporary files
  while ( !tempFiles_.empty() ) {
    remove( tempFiles_.front().c_str() );
    tempFiles_.pop();
  }

  // Remove preview file
  remove(PREVIEW_FILE_NAME.c_str());
}

bool EOS550DFake::initialize()
{
  initialized_ = true;

  // Assign some values to the fake options lists
  vector<string> apertures;
  apertures.push_back("5");
  apertures.push_back("8");
  optionLists_[APERTURE] = apertures;

  vector<string> isoValues;
  isoValues.push_back("auto");
  isoValues.push_back("100");
  isoValues.push_back("200");
  optionLists_[ISO] = isoValues;

  vector<string> whiteBalances;
  whiteBalances.push_back("auto");
  whiteBalances.push_back("fluorescent");
  whiteBalances.push_back("incandescent");
  optionLists_[WHITE_BALANCE] = whiteBalances;

  vector<string> shutterSpeeds;
  shutterSpeeds.push_back("1s");
  shutterSpeeds.push_back("0.5s");
  shutterSpeeds.push_back("1/5s");
  optionLists_[SHUTTER_SPEED] = shutterSpeeds;

  // Set all selections to default
  for (map<Option,int>::iterator it = optionSelections_.begin();
      it != optionSelections_.end();
      ++it)
    it->second = 0;

  // Always return true
  return true;
}

vector<string> EOS550DFake::readOptions(const VEOS550D::Option &option) const
{
  return optionLists_.find(option)->second;
}

int EOS550DFake::readOption(const VEOS550D::Option &option)
{
  return optionSelections_.find(option)->second;
}

bool EOS550DFake::writeOption(const VEOS550D::Option &option, int value)
{
  // Pretend to be communicating with an external device
  if (initialized_)
    optionSelections_[option] = value;

  // Always return true if 'communication' is possible
  return initialized_;
}

string EOS550DFake::acquirePhoto()
{
  string fileName = renderPicture(false);
  registerTempFileName(fileName);
  return fileName;
}

string EOS550DFake::acquirePreview()
{
  return renderPicture(true);
}

bool EOS550DFake::isInPreviewMode()
{
  return inPreviewMode_;
}

bool EOS550DFake::startPreviewMode()
{
  bool success = initialized_ && !inPreviewMode_;
  // Start if possible
  if (success) inPreviewMode_ = true;
  // Return if starting is possible
  return success;
}

bool EOS550DFake::stopPreviewMode()
{
  // Check if possible to stop
  bool success =  initialized_ && inPreviewMode_;
  // Stop if possible
  if (success) inPreviewMode_ = false;
  // Return success
  return success;
}

void EOS550DFake::registerTempFileName(const string &fileName)
{
  // Limit the amount of disk space used by the temporary files
  if ( tempFiles_.size() > maxTempFileCount_-1 ) {
    remove( tempFiles_.front().c_str() );
    tempFiles_.pop();
  }

  tempFiles_.push(fileName);
}

string EOS550DFake::renderPicture(bool preview)
{
  // Create matrix to store image information
  Mat image(2*PICTURE_WIDTH/3, PICTURE_WIDTH, CV_8UC3);

  vector<string> lines;
  // Camera name
  lines.push_back("Canon EOS 550D (fake)");

  // Timestamp and preview info
  time_t timestamp;
  time(&timestamp);
  struct tm* timeinfo = gmtime(&timestamp);

  char buffer[40];
  strftime(buffer, 40, "%Y:%m:%d %H:%M:%S", timeinfo);

  // Keep dateTime string for EXIF data, don't polute with preview information
  string dateTime(buffer);
  if (preview)
    lines.push_back(dateTime + " (preview)");
  else
    lines.push_back(dateTime);

  // Camera settings
  stringstream settings;
  settings << "ISO" << readOptionStringValue(ISO)
               << "; F" << readOptionStringValue(APERTURE)
               << "; " << readOptionStringValue(SHUTTER_SPEED)
               << "; WB " << readOptionStringValue(WHITE_BALANCE);
  // copy string to avoid segmentation errors
  lines.push_back(string(settings.str()));

  // Font settings
#ifndef USE_FAKEIO
  int fontFace = cv::FONT_HERSHEY_SIMPLEX;
#else
  int fontFace = FONT_HERSHEY_SIMPLEX;
#endif
  int thickness = 4;
  Scalar white = Scalar::all(255);

  // Determine font scaling
  Size textSize;
  int baseline = 0;
  int lineWidth = 0;
  int lineHeight = 0;

  for (int i=0; i<lines.size(); ++i) {
    textSize = getTextSize(lines[i], fontFace, 1, thickness, &baseline);
    lineWidth = std::max(textSize.width, lineWidth);
    lineHeight = std::max(textSize.height, lineHeight);
  }

  double blockHeight = ( 1.5*lines.size()+1 ) * lineHeight;
  double fontScale = image.rows / ( blockHeight + lineWidth );

  lineHeight *= fontScale;

  // Put text on image
  Point textOrigin(lineHeight/2, lineHeight*1.5);
  for (int i=0; i<lines.size(); ++i) {
    putText(image, lines[i], textOrigin, fontFace, fontScale, white, thickness);
    textOrigin.y += lineHeight*1.5;
  }

  // Copy text block
  Mat textBlock(image,
                cv::Range(0, blockHeight*fontScale),
                cv::Range(0, textOrigin.x + lineWidth*fontScale));

  // Rotate block
  Point rotationCentre(0,0); // top left
  Mat rotationMatrix = getRotationMatrix2D(rotationCentre, 90, 1); // clockwise
  // Add translation along y-axis to affine transformation matrix
  rotationMatrix.col(2).row(1) += textBlock.cols;

  Mat rotatedText(textBlock.cols, textBlock.rows, textBlock.type());
  warpAffine(textBlock, rotatedText, rotationMatrix, rotatedText.size());

  // Copy rotated block
  int width = rotatedText.cols;
  int height = rotatedText.rows;
  // Will contain region-of-interest in image, data is not copied!
  Mat imageRotatedText(image, Rect(0, image.rows-height, width, height));
  rotatedText.copyTo(imageRotatedText);

  // Draw raster of dots
  const int rasterHalfWidth = 5;
  const int dotSeparation = image.size().width / 50;
  const int dotRadius = dotSeparation / 10;
  const int dotLineWidth = -1;
  Scalar green = Scalar::all(0);
  green[1] = 255;

  Point dot(image.cols/2 - (rasterHalfWidth+1)*dotSeparation,
            image.rows/2);
  const Point ySeparation(0, dotSeparation);

  for (int i=1-rasterHalfWidth; i<rasterHalfWidth; ++i) {
    // Draw middle dot
    if (i==0)
      circle(image, dot, dotRadius, green, dotLineWidth);
    else
      circle(image, dot, dotRadius, white, dotLineWidth);

    // Draw one dot above, one dot below
    for (int j=1; j<rasterHalfWidth; ++j) {
      circle(image, dot+j*ySeparation, dotRadius, white, dotLineWidth);
      circle(image, dot-j*ySeparation, dotRadius, white, dotLineWidth);
    }

    dot.x += dotSeparation;
  }

  // Save image to file
  // Add '.jpg' for OpenCV image type recognition!
  string fileName;

  if (preview)
    fileName = PREVIEW_FILE_NAME;
  else {
    char format[] = "/tmp/canon-fake-XXXXXX.jpg";
    // Create file and close it, can't use file descriptor with OpenCV
    close(mkstemps(format, 4));
    fileName = format;
  }

  vector<int> options;
#ifndef USE_FAKEIO
  options.push_back(cv::IMWRITE_JPEG_QUALITY);
#else
  options.push_back(IMWRITE_JPEG_QUALITY);
#endif
  options.push_back(90);
  imwrite(fileName, image, options);

  // Supply file with sensible EXIF data (exiv2)
  // See DefoMeasurement::readExifData for needed tags
  Exiv2::ExifData exifData;

  // Some fake camera information
  exifData["Exif.Image.Make"] = "DESY";
  exifData["Exif.Image.Model"] = "Deformation setup fake";
  exifData["Exif.Image.Software"] = "cmstkmodlab fake camera device";

  // Set orientation according to camera orientation in setup
  exifData["Exif.Image.Orientation"] = int16_t(6);

  // Add some timing information
  exifData["Exif.Image.DateTime"] = dateTime.c_str();
  exifData["Exif.Photo.DateTimeOriginal"] = dateTime.c_str();
  //  exifData["Exif.Photo.DateTimeDigitized"] = dateTime.c_str();

  // For options, see EOS550DFake::initialize
  if (readOption(ISO) != 2)
    exifData["Exif.Photo.ISOSpeedRatings"] = int32_t(100);
  else
    exifData["Exif.Photo.ISOSpeedRatings"] = int32_t(200);

  // For options, see EOS550DFake::initialize
  if (readOption(APERTURE) == 0)
    exifData["Exif.Photo.FNumber"] = Exiv2::Rational(5,1);
  else
    exifData["Exif.Photo.FNumber"] = Exiv2::Rational(8,1);

  // For options, see EOS550DFake::initialize
  int shutterSpeed = readOption(SHUTTER_SPEED);
  if (shutterSpeed == 0)
    exifData["Exif.Photo.ExposureTime"] = Exiv2::Rational(1,1);
  else if (shutterSpeed == 1)
    exifData["Exif.Photo.ExposureTime"] = Exiv2::Rational(1,2);
  else
    exifData["Exif.Photo.ExposureTime"] = Exiv2::Rational(1,5);

  // Set somewhat sensible focal length
  exifData["Exif.Photo.FocalLength"] = Exiv2::Rational(17,1);

  Exiv2::Image::AutoPtr imageFile = Exiv2::ImageFactory::open(fileName);
  if (imageFile.get() != 0) {
    imageFile->setExifData(exifData);
    imageFile->writeMetadata();
  }

  return fileName;
}

string EOS550DFake::readOptionStringValue(const VEOS550D::Option &option)
{
  return readOptions(option)[readOption(option)];
}
