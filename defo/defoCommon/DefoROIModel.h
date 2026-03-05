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

  bool write(const QString& filename);
  bool read(const QString& filename);

protected:

  DefoROI *roi_;

protected slots:

signals:

  void roiChanged(bool);
};

#endif // DEFOROIMODEL_H
