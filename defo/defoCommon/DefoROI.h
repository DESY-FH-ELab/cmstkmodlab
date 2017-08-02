/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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

  bool assignFrom(DefoROI* other);
  bool assignTo(DefoROI* other);

protected:

  int selectedPoint_;
};

#endif
