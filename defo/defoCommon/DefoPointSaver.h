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

#ifndef DEFOPOINTSAVER_H
#define DEFOPOINTSAVER_H

#include <QFile>

#include "DefoPoint.h"

class DefoPointSaver : public QFile
{
  Q_OBJECT
public:
  explicit DefoPointSaver(const QString& filename, QObject* parent = 0);
  ~DefoPointSaver();

  void writePoints(const DefoPointCollection &points);
  void writeXMLPoints(const DefoPointCollection &points);

protected:
  const static QString LINE_FORMAT;

  void writePoint(const DefoPoint &point);
};

#endif // DEFOPOINTSAVER_H
