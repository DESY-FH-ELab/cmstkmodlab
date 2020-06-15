/****************************************************************************
 **
 **  Copyright (C) 2015 Andreas Mussgiller
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **
 **
 ****************************************************************************/

#ifndef NPOINT2D_H
#define NPOINT2D_H

#include <iostream>

class NVector2D;
class NLine2D;

/** @addtogroup common
 *  @{
 */

class NPoint2D
{
public:

  NPoint2D();
  NPoint2D(double x, double y);
  NPoint2D(const NPoint2D& other);
  ~NPoint2D();

  double x() const { return x_; }
  double y() const { return y_; }

  void move(double dx, double dy);
  void move(const NVector2D& v);
  void move(const NVector2D& v, double scale);

  double distanceTo(const NPoint2D& other);
  double distanceTo(const NLine2D& line);
  double signedDistanceTo(const NLine2D& line);

  bool isCoincident(const NLine2D& line);

protected:

  double x_, y_;
};

/** @} */

#endif // NPOINT2D_H
