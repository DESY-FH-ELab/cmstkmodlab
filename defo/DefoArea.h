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

#ifndef _DEFOAREA_H
#define _DEFOAREA_H

#include <QRect>

///
///
///
class DefoArea {

 public:
  DefoArea() {}
  DefoArea( QRect& r ) { rectangle_ = r; }
  ~DefoArea() {}
  
  void setRectangle( QRect& r ) { rectangle_ = r; }
  void setName( QString& name ) { name_ = name; }
  QRect& getRectangle( void ) { return rectangle_; }
  QString& getName( void ) { return name_; }

 private:
  QRect rectangle_;
  QString name_;

};


#endif
