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

#ifndef DEFOVPOINTINDEXER_H
#define DEFOVPOINTINDEXER_H

#include <QObject>
#include <QColor>

#include "DefoPoint.h"

class DefoVPointIndexer : public QObject
{
  Q_OBJECT
public:
  explicit DefoVPointIndexer(QObject *parent = 0);

  virtual void indexPoints(DefoPointCollection* points, const QColor& seedColor) = 0;
};

#endif // DEFOVPOINTINDEXER_H
