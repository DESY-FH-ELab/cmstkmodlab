/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef EOS550DFAKE_H
#define EOS550DFAKE_H

#include "VEOS550D.h"

// opencv2
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <exiv2/exif.hpp>
#include <exiv2/jpgimage.hpp>

#include <stdio.h>

#include <map>
#include <queue>

class EOS550DFake : public VEOS550D
{
public:

  EOS550DFake(const char* port);
  ~EOS550DFake();

  bool initialize();

  std::vector<std::string> readOptions(const Option &option) const;
  int readOption(const Option &option);
  bool writeOption(const Option &option, int value);

  std::string acquirePhoto();

  std::string acquirePreview();
  virtual bool isInPreviewMode();
  virtual bool startPreviewMode();
  virtual bool stopPreviewMode();

protected:

  /**
    \brief Renders a fake picture and returns the location of the file.
    The picture will be rendered displaying a small dot grid and showing some
    extra information of which settings the fake device had at rendition time.
    The picture is stored in a temporary JPEG-file and has some EXIF headers.
    \param preview Whether the rendered picture is a preview picture or not.
    \returns The (temporary) file used to store the rendered picture.
    */
  std::string renderPicture(bool preview);
  std::string readOptionStringValue(const Option& option);
  /**
    \brief Registers the name of a temporary file to keep track of these files.
    The file names are stored in a list so that the fake device can limit disk
    usage and clean up upon destruction.
    \param fileName Name of the temporary file.
    */
  // TODO move temporary file handling to VEOS550D, is common
  void registerTempFileName(const std::string& fileName);

private:

  /// File name of the preview picture
  static const std::string PREVIEW_FILE_NAME;
  static const int PICTURE_WIDTH;
  /// Maximum number of temporary files to limit disk space usage.
  const int maxTempFileCount_;

  /// Whether the device is initialized.
  bool initialized_;
  /// Whether the device is in preview mode.
  bool inPreviewMode_;
  /// Cache of lists of values per suported option
  std::map<Option,OptionList> optionLists_;
  /// Current selection for each supported option
  std::map<Option,int> optionSelections_;

  /// List of currently stored temporary files.
  std::queue<std::string> tempFiles_;
};

#endif // EOS550DFAKE_H
