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

#ifndef DEFOIMAGEAVERAGER_H
#define DEFOIMAGEAVERAGER_H

#include <QStringList>
#include <QImage>

class DefoImageAverager
{
public:

  explicit DefoImageAverager(const QStringList& filenames);
  ~DefoImageAverager();

  QImage getAveragedImage();

protected:

  QStringList filenames_;
};

#endif // DEFOIMAGEAVERAGER_H
