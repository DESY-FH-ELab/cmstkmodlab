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

#include <nqlogger.h>

#include <AssemblyThresholder.h>

AssemblyThresholder::AssemblyThresholder(QObject* parent) :
  QObject(parent),

//  method_(AssemblyThresholder::ThresholdingMethod::undefined),

  updated_img_raw_(false),
  updated_img_bin_(false)
{
  NQLog("AssemblyThresholder", NQLog::Debug) << "constructed";
}

AssemblyThresholder::~AssemblyThresholder()
{
  NQLog("AssemblyThresholder", NQLog::Debug) << "destructed";
}

void AssemblyThresholder::update_image_raw(const cv::Mat& img)
{
//  mutex_.lock();

  if(img.channels() > 1)
  {
    img_raw_ = img;
  }
  else
  {
    cv::Mat img_color;
    cv::cvtColor(img, img_color, cv::COLOR_GRAY2BGR);

    img_raw_ = img_color.clone();
  }

  if(updated_img_raw_ == false){ updated_img_raw_ = true ; }
  if(updated_img_bin_ == true ){ updated_img_bin_ = false; }

//  mutex_.unlock();

  NQLog("AssemblyThresholder", NQLog::Spam) << "update_image_raw"
     << ": emitting signal \"updated_image_raw\"";

  emit updated_image_raw(img_raw_);
  emit updated_image_raw();
}

void AssemblyThresholder::delete_image_raw()
{
//  mutex_.lock();

  img_raw_ = cv::Mat();

  if(updated_img_raw_ == true){ updated_img_raw_ = false; }
  if(updated_img_bin_ == true){ updated_img_bin_ = false; }

//  mutex_.unlock();

  return;
}

void AssemblyThresholder::update_image_binary_threshold(const int threshold_value)
{
  if(img_raw_.empty())
  {
    NQLog("AssemblyThresholder", NQLog::Critical) << "update_image_binary_threshold"
       << ": empty input image, thresholding not applied to input image";

    return;
  }

//  mutex_.lock();

  img_bin_ = this->get_image_binary_threshold(img_raw_, threshold_value);

  if(updated_img_bin_ == false){ updated_img_bin_ = true; }

//  mutex_.unlock();

  NQLog("AssemblyThresholder", NQLog::Spam) << "update_image_binary_threshold(" << threshold_value << ")"
     << ": emitting signal \"updated_image_binary\"";

  emit updated_image_binary(img_bin_);
  emit updated_image_binary();
}

cv::Mat AssemblyThresholder::get_image_binary_threshold(const cv::Mat& img, const int threshold) const
{
  // greyscale image
  cv::Mat img_gs(img.size(), img.type());

  if(img.channels() > 1)
  {
    // convert color to GS
    cv::cvtColor(img, img_gs, cv::COLOR_BGR2GRAY);
  }
  else
  {
    img_gs = img.clone();
  }

  // binary image
  cv::Mat img_bin(img_gs.size(), img_gs.type());

  if(threshold < 0)
  {
    NQLog("AssemblyThresholder", NQLog::Warning) << "get_image_binary_threshold"
       << ": invalid threshold value (" << threshold << ", is negative), thresholding not applied to input image (empty binary image)";

    img_bin = cv::Mat();
  }
  else if(threshold > 255)
  {
    NQLog("AssemblyThresholder", NQLog::Warning) << "get_image_binary_threshold"
       << ": invalid threshold value (" << threshold << ", higher than allowed maximum of 255), thresholding not applied to input image (empty binary image)";

    img_bin = cv::Mat();
  }
  else
  {
    cv::threshold(img_gs, img_bin, threshold, 255, cv::THRESH_BINARY);
  }

  return img_bin;
}

void AssemblyThresholder::update_image_binary_adaptiveThreshold(const int blocksize_value)
{
  if(img_raw_.empty())
  {
    NQLog("AssemblyThresholder", NQLog::Critical) << "update_image_binary_adaptiveThreshold"
       << ": empty input image, thresholding not applied to input image";

    return;
  }

//  mutex_.lock();

  img_bin_ = this->get_image_binary_adaptiveThreshold(img_raw_, blocksize_value);

  if(updated_img_bin_ == false){ updated_img_bin_ = true; }

//  mutex_.unlock();

  NQLog("AssemblyThresholder", NQLog::Spam) << "update_image_binary_adaptiveThreshold(" << blocksize_value << ")"
     << ": emitting signal \"updated_image_binary\"";

  emit updated_image_binary(img_bin_);
  emit updated_image_binary();
}

cv::Mat AssemblyThresholder::get_image_binary_adaptiveThreshold(const cv::Mat& img, const int blocksize) const
{
  // greyscale image
  cv::Mat img_gs(img.size(), img.type());

  if(img.channels() > 1)
  {
    // convert color to GS
    cv::cvtColor(img, img_gs, cv::COLOR_BGR2GRAY);
  }
  else
  {
    img_gs = img.clone();
  }

  // binary image
  cv::Mat img_bin(img_gs.size(), img_gs.type());

  if(blocksize < 3)
  {
    NQLog("AssemblyThresholder", NQLog::Warning) << "get_image_binary_adaptiveThreshold"
       << ": invalid block-size value (" << blocksize << ", lower than allowed minimum of 3), thresholding not applied to input image (empty binary image)";

    img_bin = cv::Mat();
  }
  else if(blocksize > 1499)
  {
    NQLog("AssemblyThresholder", NQLog::Warning) << "get_image_binary_adaptiveThreshold"
       << ": invalid block-size value (" << blocksize << ", higher than allowed maximum of 1499), thresholding not applied to input image (empty binary image)";

    img_bin = cv::Mat();
  }
  else if((blocksize % 2) == 0)
  {
    NQLog("AssemblyThresholder", NQLog::Warning) << "get_image_binary_adaptiveThreshold"
       << ": invalid block-size value (" << blocksize << ", is even), thresholding not applied to input image (empty binary image)";

    img_bin = cv::Mat();
  }
  else
  {
    cv::adaptiveThreshold(img_gs, img_bin, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, blocksize, 0.);
  }

  return img_bin;
}

void AssemblyThresholder::delete_image_binary()
{
//  mutex_.lock();

  img_bin_ = cv::Mat();

  if(updated_img_bin_){ updated_img_bin_ = false; }

//  mutex_.unlock();

  return;
}

void AssemblyThresholder::send_image_raw()
{
  if(img_raw_.empty())
  {
    NQLog("AssemblyThresholder", NQLog::Critical) << "send_image_raw"
       << ": raw image is empty, no action taken";

    return;
  }

  NQLog("AssemblyThresholder", NQLog::Spam) << "send_image_raw"
     << ": emitting signal \"image_sent\"";

  emit image_sent(img_raw_);
}

void AssemblyThresholder::send_image_binary()
{
  if(img_bin_.empty())
  {
    NQLog("AssemblyThresholder", NQLog::Critical) << "send_image_binary"
       << ": binary image is empty, no action taken";

    return;
  }

  NQLog("AssemblyThresholder", NQLog::Spam) << "send_image_binary"
     << ": emitting signal \"image_sent\"";

  emit image_sent(img_bin_);
}
