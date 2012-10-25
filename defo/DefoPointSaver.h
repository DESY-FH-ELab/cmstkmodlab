#ifndef DEFOPOINTSAVER_H
#define DEFOPOINTSAVER_H

#include <QFile>

#include "DefoPoint.h"

class PointSaver : public QFile
{
  Q_OBJECT
public:
  explicit PointSaver(const QString& filename, QObject* parent = 0);
  ~PointSaver();
  void writePoint(const DefoPoint &point);

protected:
  const static QString LINE_FORMAT;
};

#endif // DEFOPOINTSAVER_H
