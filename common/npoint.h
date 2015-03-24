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

#ifndef NPOINT_H
#define NPOINT_H

#include <iostream>

class NVector;
class NLine;
class NPlane;

class NPoint
{
public:

  NPoint();
  NPoint(double x, double y, double z);
  NPoint(const NPoint& other);
  ~NPoint();

  double x() const { return x_; }
  double y() const { return y_; }
  double z() const { return z_; }

  void move(double dx, double dy, double dz);
  void move(const NVector& v);
  void move(const NVector& v, double scale);

  double distanceTo(const NPoint& other);
  double distanceTo(const NLine& line);
  double distanceTo(const NPlane& plane);

  bool isCoincident(const NLine& line);
  bool isCoincident(const NPlane& plane);

protected:

  double x_, y_, z_;
};

#endif // NPOINT_H
