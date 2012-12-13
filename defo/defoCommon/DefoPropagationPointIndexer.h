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
