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

#ifndef APPLICATIONCONFIGWRITER_H
#define APPLICATIONCONFIGWRITER_H

#include <string>
#include <iostream>
#include <map>

#include <QObject>

#include "ApplicationConfig.h"

/** @addtogroup common
 *  @{
 */

class ApplicationConfigWriter : public QObject
{
  Q_OBJECT

public:

  ApplicationConfigWriter( const std::string & filename );
  ~ApplicationConfigWriter();

  void write(ApplicationConfig::storage_t  &keyvalueMap);
  std::string getFileName( void ) { return outputFileName_; }

private:

  std::string outputFileName_;

  void writeMerge(ApplicationConfig::storage_t  &keyvalueMap);
  void writeNew(ApplicationConfig::storage_t  &keyvalueMap);
};

/** @} */

#endif // APPLICATIONCONFIGWRITER_H
