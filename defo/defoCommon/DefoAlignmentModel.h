/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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
  void reset();
  AlignmentPoint getSelectedPoint() const { return selectedPoint_; }
  void selectPoint(AlignmentPoint point) { selectedPoint_ = point; }

  const QPointF& getOrigin() const { return origin_; }
  const QPointF& getTip() const { return tip_; }

  double getAngle() const;

  void write(const QString& filename);
  void read(const QString& filename);

protected:
  AlignmentPoint selectedPoint_;
  QPointF origin_;
  QPointF tip_;

signals:
  void alignmentChanged(double angle);
};

#endif // DEFOALIGNMENTMODEL_H
