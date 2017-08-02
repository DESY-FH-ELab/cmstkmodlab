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

#ifndef DEFOCOLORSELECTIONMODEL_H
#define DEFOCOLORSELECTIONMODEL_H

#include <QColor>
#include <QDir>

class DefoColorSelectionModel : public QObject
{
    Q_OBJECT
public:
  explicit DefoColorSelectionModel(QObject *parent = 0);

  float getDistance(float hue, float saturation);
  void setColor(float hue, float saturation);

  const QColor& getColor() const { return color_; }

  void write(const QString& filename);
  void read(const QString& filename);

protected:
  QColor color_;

signals:
  void colorChanged(float hue, float saturation);
};

#endif // DEFOALIGNMENTMODEL_H
