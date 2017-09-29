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

#ifndef CAMERACOMHANDLER_H
#define CAMERACOMHANDLER_H

#include <vector>
#include <string>
#include <gphoto2/gphoto2.h>
#include <iostream>
#include <stdio.h>

class CameraComHandler {

public:
  CameraComHandler(const char* modelName, const char* port);
  ~CameraComHandler();

  bool initialize();

  std::string readConfigValue(const char* name) const;
  std::vector<std::string> readConfigOptions(const char* name) const;

  bool writeConfigValue(const char* name, const char* value);

  bool acquireAndDownloadPicture(int filedescriptor) const;
  bool acquirePreview(const char* filename) const;
  bool startPreviewMode() const;
  bool stopPreviewMode() const;

protected:

  GPContext* context_;
  Camera* camera_;

  CameraAbilities abilities_;
  GPPortInfoList* infoList_;
  GPPortInfo info_;
};

#endif // CAMERACOMHANDLER_H
