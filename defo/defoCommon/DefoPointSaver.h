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
