#ifndef DEFOROIMODEL_H
#define DEFOROIMODEL_H

#include <QObject>
#include <QDir>
#include <QPolygonF>
#include <QImage>
#include <QRect>

class DefoROIModel : public QObject, public QPolygonF
{
    Q_OBJECT
public:
  explicit DefoROIModel(QObject *parent = 0);

  float getClosestPoint(int & index,
                        float width, float height,
                        float x, float y);
  void setPoint(int index, float newX, float newY);
  int getSelectedPoint() const { return selectedPoint_; }
  void selectPoint(int index) { selectedPoint_ = index; }

  void appendPoint();
  void removePoint();

  bool containsPoint(float width, float height,
                     float x, float y);

  void write(const QString& filename);
  void read(const QString& filename);

protected:
  int selectedPoint_;

signals:
  void roiChanged();
};

#endif // DEFOROIMODEL_H
