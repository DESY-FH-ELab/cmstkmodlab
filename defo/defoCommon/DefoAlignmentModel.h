#ifndef DEFOALIGNMENTMODEL_H
#define DEFOALIGNMENTMODEL_H

#include <QObject>
#include <QDir>
#include <QPolygonF>
#include <QImage>
#include <QRect>

class DefoAlignmentModel : public QObject
{
    Q_OBJECT
public:
  explicit DefoAlignmentModel(QObject *parent = 0);

  enum AlignmentPoint { None, Origin, Tip };

  float getClosestPoint(AlignmentPoint & point,
                        float width, float height,
                        float x, float y);
  void setPoint(AlignmentPoint point, float newX, float newY);
  AlignmentPoint getSelectedPoint() const { return selectedPoint_; }
  void selectPoint(AlignmentPoint point) { selectedPoint_ = point; }

  const QPointF& getOrigin() const { return origin_; }
  const QPointF& getTip() const { return tip_; }

  double getAngle() const;

  void write(const QDir& path);
  void read(const QString& filename);

protected:
  AlignmentPoint selectedPoint_;
  QPointF origin_;
  QPointF tip_;

signals:
  void alignmentChanged(double angle);
};

#endif // DEFOALIGNMENTMODEL_H
