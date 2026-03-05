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

#ifndef DEFOPROPAGATIONPOINTINDEXER_H
#define DEFOPROPAGATIONPOINTINDEXER_H

#include <vector>

#include "DefoVPointIndexer.h"
#include "DefoPoint.h"

class DefoPropagationPointIndexer : public DefoVPointIndexer
{
  Q_OBJECT
public:

  enum Axis { X, Y };
  enum Direction { Forward, Backward };

  explicit DefoPropagationPointIndexer(QObject *parent = 0);

  void indexPoints(DefoPointCollection* points, const QColor& seedColor);

protected:

  typedef std::vector<DefoPoint*> PointVector;

  DefoPoint * findSeed(DefoPointCollection* points, const QColor& seedColor);
  void determineGrid(DefoPointCollection* points, const DefoPoint* seed,
                     double& dx, double& dy, double& d);
  DefoPoint * findNeighbour(DefoPointCollection* points, const DefoPoint* seed,
                            Axis axis, Direction direction, double window);
};

#endif // DEFOPROPAGATIONPOINTINDEXER_H
