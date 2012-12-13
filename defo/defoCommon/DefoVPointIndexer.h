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
