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
  GPPortInfo info_;
};

#endif // CAMERACOMHANDLER_H
