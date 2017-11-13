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

#ifndef UTIL_H
#define UTIL_H

#include <string>

#include <QString>

namespace Util {

  QString QtCacheDirectory();

  bool QDir_mkpath(const QString&);
  bool QDir_mkpath(const std::string&);

  bool DirectoryExists(const QString&);
  bool DirectoryExists(const std::string&);

  bool IsFile(const QString&);
  bool IsFile(const std::string&);

}

#endif
