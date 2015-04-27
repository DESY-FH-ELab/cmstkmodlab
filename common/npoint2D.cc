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

#include "nvector2D.h"
#include "nline2D.h"
#include "npoint2D.h"

NPoint2D::NPoint2D()
: x_(0.),
  y_(0.)
{

}

NPoint2D::NPoint2D(double x, double y)
: x_(x),
  y_(y)
{

}

NPoint2D::NPoint2D(const NPoint2D& other)
: x_(other.x()),
  y_(other.y())
{

}

NPoint2D::~NPoint2D()
{

}

void NPoint2D::move(double dx, double dy)
{
  x_ += dx;
  y_ += dy;
}

void NPoint2D::move(const NVector2D& v)
{
  x_ += v.x();
  y_ += v.y();
}

void NPoint2D::move(const NVector2D& v, double scale)
{
  x_ += v.x() * scale;
  y_ += v.y() * scale;
}

double NPoint2D::distanceTo(const NPoint2D& other)
{
  NVector2D v(other, *this);
  return v.length();
}

double NPoint2D::distanceTo(const NLine2D& line)
{
  return std::fabs(signedDistanceTo(line));
}

double NPoint2D::signedDistanceTo(const NLine2D& line)
{
  NPoint2D p1 = line.point();
  NPoint2D p2 = line.point();
  p2.move(line.direction());

  NDirection2D v(p2.y() - p1.y(),
                 -1.0*(p2.x()-p1.x()));
  NVector2D r(*this, p1);

  return v.dot(r);
}

bool NPoint2D::isCoincident(const NLine2D& line)
{
  double d = distanceTo(line);
  if (d==0.) return true;
  return false;
}
