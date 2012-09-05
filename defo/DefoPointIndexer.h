#ifndef DEFOPOINTINDEXER_H
#define DEFOPOINTINDEXER_H

#include <QObject>
#include <QList>
#include <QMap>

#include "DefoPoint.h"
#include "DefoPointBin.h"

class DefoPointIndexer : public QObject
{
  Q_OBJECT
public:
  explicit DefoPointIndexer(QObject *parent = 0);

  void indexPoints(DefoPointCollection* points);

protected:
  int maxSpacing_;

  void indexPointsAxis(DefoPointCollection* points, const DefoPoint::Axis& axis);
  void fillBins(DefoPointCollection* points, const DefoPoint::Axis& axis);
  static bool binPrecedes(const DefoPointBin& left, const DefoPointBin& right);

  // Bin lists in x and y directions
  QMap< DefoPoint::Axis, QList<DefoPointBin> > bins_;
  // Caching of the bin means for faster searching
//  QMap< DefoPoint::Axis, QList<double> > binPositions_;


};

#endif // DEFOPOINTINDEXER_H
