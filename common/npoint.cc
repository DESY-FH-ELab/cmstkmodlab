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

#include "nvector.h"
#include "nline.h"
#include "nplane.h"
#include "npoint.h"

NPoint::NPoint()
: x_(0.),
  y_(0.),
  z_(0.)
{

}

NPoint::NPoint(double x, double y, double z)
: x_(x),
  y_(y),
  z_(z)
{

}

NPoint::NPoint(const NPoint& other)
: x_(other.x()),
  y_(other.y()),
  z_(other.z())
{

}

NPoint::~NPoint()
{

}

void NPoint::move(double dx, double dy, double dz)
{
  x_ += dx;
  y_ += dy;
  z_ += dz;
}

void NPoint::move(const NVector& v)
{
  x_ += v.x();
  y_ += v.y();
  z_ += v.z();
}

void NPoint::move(const NVector& v, double scale)
{
  x_ += v.x() * scale;
  y_ += v.y() * scale;
  z_ += v.z() * scale;
}

double NPoint::distanceTo(const NPoint& other)
{
  NVector v(other, *this);
  return v.length();
}

double NPoint::distanceTo(const NLine& line)
{
  NPoint x1 = line.point();
  NPoint x2 = line.point();
  x2.move(line.direction());
  double d = line.direction().length();
  NVector v1(x1, *this);
  NVector v2(x2, *this);
  NVector cross = v1.cross(v2);

  return cross.length()/d;
}

double NPoint::distanceTo(const NPlane& plane)
{
  NVector w(*this, plane.point());
  return std::fabs(w.dot(plane.normal()));
}

bool NPoint::isCoincident(const NLine& line)
{
  double d = distanceTo(line);
  if (d==0.) return true;
  return false;
}

bool NPoint::isCoincident(const NPlane& plane)
{
  double d = distanceTo(plane);
  if (d==0.) return true;
  return false;
}
