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

#include "npoint2D.h"
#include "nvector2D.h"

NVector2D::NVector2D()
: x_(0.),
  y_(0.)
{

}

NVector2D::NVector2D(double x, double y)
: x_(x),
  y_(y)
{

}

NVector2D::NVector2D(const NPoint2D& a, const NPoint2D& b)
: x_(b.x() - a.x()),
  y_(b.y() - a.y())
{

}

NVector2D::NVector2D(const NVector2D& other)
: x_(other.x()),
  y_(other.y())
{

}

NVector2D::~NVector2D()
{

}

void NVector2D::rotate(double angle)
{
  double xn =  1.0*x()*std::cos(angle) - 1.0*y()*std::sin(angle);
  double yn =  1.0*x()*std::sin(angle) + 1.0*y()*std::cos(angle);
  x_ = xn;
  y_ = yn;
}

double NVector2D::length() const
{
  return std::sqrt(x_*x_ + y_*y_);
}

double NVector2D::dot(const NVector2D& other) const
{
  return x()*other.x() + y()*other.y();
}
