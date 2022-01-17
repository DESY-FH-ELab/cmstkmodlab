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

#ifndef APPLICATIONCONFIGREADER_H
#define APPLICATIONCONFIGREADER_H

#include <string>
#include <iostream>
#include <map>

#include <QObject>

/** @addtogroup common
 *  @{
 */

class ApplicationConfigReader : public QObject
{

  Q_OBJECT

public:
  ApplicationConfigReader( const std::string & filename );
  ~ApplicationConfigReader();

  void fill(std::multimap<std::string,std::string> &keyvalueMap);
  std::string getFileName( void ) { return inputFileName_; }

private:

  std::string inputFileName_;
};

/** @} */

#endif // APPLICATIONCONFIGREADER_H
