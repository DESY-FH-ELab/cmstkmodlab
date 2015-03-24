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

#ifndef NPOINT3D_H
#define NPOINT3D_H

#include <iostream>

class NVector3D;
class NLine3D;
class NPlane3D;

class NPoint3D
{
public:

  NPoint3D();
  NPoint3D(double x, double y, double z);
  NPoint3D(const NPoint3D& other);
  ~NPoint3D();

  double x() const { return x_; }
  double y() const { return y_; }
  double z() const { return z_; }

  void move(double dx, double dy, double dz);
  void move(const NVector3D& v);
  void move(const NVector3D& v, double scale);

  double distanceTo(const NPoint3D& other);
  double distanceTo(const NLine3D& line);
  double distanceTo(const NPlane3D& plane);
  double signedDistanceTo(const NPlane3D& plane);

  bool isCoincident(const NLine3D& line);
  bool isCoincident(const NPlane3D& plane);

protected:

  double x_, y_, z_;
};

#endif // NPOINT3D_H
