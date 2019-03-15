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

#ifndef DEFOPOINTINDEXER_H
#define DEFOPOINTINDEXER_H

#include <QList>
#include <QMap>

#include "DefoVPointIndexer.h"
#include "DefoPoint.h"
#include "DefoPointBin.h"

class DefoPointIndexer : public DefoVPointIndexer
{
  Q_OBJECT
public:
  explicit DefoPointIndexer(QObject *parent = 0);

  void indexPoints(DefoPointCollection* points, const QColor& seedColor);

protected:
  int maxSpacing_;

  void indexPointsAxis(DefoPointCollection* points, const QColor& seedColor,
                       const DefoPoint::Axis& axis);
  void fillBins(DefoPointCollection* points, const QColor& seedColor,
                const DefoPoint::Axis& axis);
  static bool binPrecedes(const DefoPointBin& left, const DefoPointBin& right);

  // Bin lists in x and y directions
  QMap< DefoPoint::Axis, QList<DefoPointBin> > bins_;
  // Caching of the bin means for faster searching
  // QMap< DefoPoint::Axis, QList<double> > binPositions_;
};

#endif // DEFOPOINTINDEXER_H
