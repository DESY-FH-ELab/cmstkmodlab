/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef APPLICATIONCONFIGREADER_H
#define APPLICATIONCONFIGREADER_H

#include <string>
#include <iostream>
#include <map>

#include <QObject>

#include "ApplicationConfig.h"

/** @addtogroup common
 *  @{
 */

class ApplicationConfigReader : public QObject
{

  Q_OBJECT

public:
  ApplicationConfigReader( const std::string & filename,
      const std::string& alias);
  ~ApplicationConfigReader();

  void fill(ApplicationConfig::storage_t &keyvalueMap);

  const std::string& getFileName( void ) const { return filename_; }
  const std::string& getAlias( void ) const { return alias_; }

private:

  std::string filename_;
  std::string alias_;
};

/** @} */

#endif // APPLICATIONCONFIGREADER_H
