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

#include "../EOS550D.h"

#include <iostream>

using namespace std;

int main() {

//  // require Canon EOS 550D, on usb(:001)
//  CameraComHandler* handler = new CameraComHandler(
//          "Canon EOS 550D"
//        , "usb:001"
//  );

////  cout << "Writing config" << endl;
//  vector<string> options = handler->readConfigOptions("whitebalance");
//  for (int i=0; i<options.size(); ++i)
//    cout << options.at(i) << endl;

//  handler->writeConfigValue("shutterspeed", "1/10");
//  handler->writeConfigValue("aperture", "8");
//  handler->writeConfigValue("iso", "auto");

//  char fullpath[] = "/tmp/defo-XXXXXX";
//  int fileDescriptor = mkstemp(fullpath);

//  handler->acquireAndDownloadPicture(fileDescriptor);

//  close(fileDescriptor);
//  delete handler;

  EOS550D* camera = new EOS550D("usb:");
  if ( camera->initialize() ) {

    vector<string> options = camera->readOptions(EOS550D::ISO);

    for (vector<string>::iterator it = options.begin(); it < options.end(); ++it)
      cout << *it << endl;

    cout << camera->acquirePhoto() << endl;

  }
  else
    cout << "Camera could not be initialized." << endl;

  return 0;

}
