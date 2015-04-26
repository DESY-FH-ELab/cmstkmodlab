#ifndef DEFOROI_H
#define DEFOROI_H

#include <QDir>
#include <QPolygonF>

class DefoROI : public QPolygonF
{
public:
  explicit DefoROI();

  float getClosestPoint(int & index,
                        float width, float height,
                        float x, float y);
  bool setPoint(int index, float newX, float newY);
  int getSelectedPoint() const { return selectedPoint_; }
  void selectPoint(int index) { selectedPoint_ = index; }
  int numberOfPoints() const { return size(); }

  bool insertPointBefore();
  bool insertPointAfter();
  bool removePoint();

  bool containsPoint(float width, float height,
                     float x, float y);

  void write(const QString& filename);
  void read(const QString& filename);

  void assignFrom(DefoROI* other);

protected:

  int selectedPoint_;
};

#endif
