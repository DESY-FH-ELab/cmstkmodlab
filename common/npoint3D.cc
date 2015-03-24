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

#include <cmath>

#include "nvector3D.h"
#include "nline3D.h"
#include "nplane3D.h"
#include "npoint3D.h"

NPoint3D::NPoint3D()
: x_(0.),
  y_(0.),
  z_(0.)
{

}

NPoint3D::NPoint3D(double x, double y, double z)
: x_(x),
  y_(y),
  z_(z)
{

}

NPoint3D::NPoint3D(const NPoint3D& other)
: x_(other.x()),
  y_(other.y()),
  z_(other.z())
{

}

NPoint3D::~NPoint3D()
{

}

void NPoint3D::move(double dx, double dy, double dz)
{
  x_ += dx;
  y_ += dy;
  z_ += dz;
}

void NPoint3D::move(const NVector3D& v)
{
  x_ += v.x();
  y_ += v.y();
  z_ += v.z();
}

void NPoint3D::move(const NVector3D& v, double scale)
{
  x_ += v.x() * scale;
  y_ += v.y() * scale;
  z_ += v.z() * scale;
}

double NPoint3D::distanceTo(const NPoint3D& other)
{
  NVector3D v(other, *this);
  return v.length();
}

double NPoint3D::distanceTo(const NLine3D& line)
{
  NPoint3D x1 = line.point();
  NPoint3D x2 = line.point();
  x2.move(line.direction());
  double d = line.direction().length();
  NVector3D v1(x1, *this);
  NVector3D v2(x2, *this);
  NVector3D cross = v1.cross(v2);

  return cross.length()/d;
}

double NPoint3D::distanceTo(const NPlane3D& plane)
{
  NVector3D w(*this, plane.point());
  return std::fabs(w.dot(plane.normal()));
}

double NPoint3D::signedDistanceTo(const NPlane3D& plane)
{
  NVector3D w(*this, plane.point());
  return w.dot(plane.normal());
}

bool NPoint3D::isCoincident(const NLine3D& line)
{
  double d = distanceTo(line);
  if (d==0.) return true;
  return false;
}

bool NPoint3D::isCoincident(const NPlane3D& plane)
{
  double d = distanceTo(plane);
  if (d==0.) return true;
  return false;
}
