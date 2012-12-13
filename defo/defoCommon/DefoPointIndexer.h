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
