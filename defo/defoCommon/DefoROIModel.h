#ifndef DEFOROIMODEL_H
#define DEFOROIMODEL_H

#include <QObject>
#include <QPolygonF>

#include <DefoROI.h>

class DefoROIModel : public QObject
{
    Q_OBJECT

public:

  explicit DefoROIModel(QObject *parent = 0);

  QRectF boundingRect() const;
  int size() const;
  const QPointF at(int i) const;
  const QPointF first() const;
  const QPointF last() const;

  float getClosestPoint(int & index,
                        float width, float height,
                        float x, float y);
  void setPoint(int index, float newX, float newY);
  int getSelectedPoint() const;
  void selectPoint(int index);
  int numberOfPoints() const;

  void insertPointBefore();
  void insertPointAfter();
  void removePoint();

  bool containsPoint(float width, float height,
                     float x, float y);

  void assignFrom(DefoROI* roi);
  void assignTo(DefoROI* roi);

  bool isEqualTo(DefoROIModel* other);

protected:

  DefoROI *roi_;

protected slots:

signals:

  void roiChanged(bool);
};

#endif // DEFOROIMODEL_H
