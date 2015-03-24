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

#include "npoint.h"
#include "nvector.h"

NVector::NVector()
: x_(0.),
  y_(0.),
  z_(0.)
{

}

NVector::NVector(double x, double y, double z)
: x_(x),
  y_(y),
  z_(z)
{

}

NVector::NVector(const NPoint& a, const NPoint& b)
: x_(b.x() - a.x()),
  y_(b.y() - a.y()),
  z_(b.z() - a.z())
{

}

NVector::NVector(const NVector& other)
: x_(other.x()),
  y_(other.y()),
  z_(other.z())
{

}

NVector::~NVector()
{

}

void NVector::rotateX(double angle)
{
  double yn =  1.0*y()*std::cos(angle) - 1.0*z()*std::sin(angle);
  double zn =  1.0*y()*std::sin(angle) + 1.0*z()*std::cos(angle);
  y_ = yn;
  z_ = zn;
}

void NVector::rotateY(double angle)
{
  double xn =  1.0*x()*std::cos(angle) + 1.0*z()*std::sin(angle);
  double zn = -1.0*x()*std::sin(angle) + 1.0*z()*std::cos(angle);
  x_ = xn;
  z_ = zn;
}

void NVector::rotateZ(double angle)
{
  double xn = 1.0*x()*std::cos(angle) - 1.0*y()*std::sin(angle);
  double yn = 1.0*x()*std::sin(angle) + 1.0*y()*std::cos(angle);
  x_ = xn;
  y_ = yn;
}

double NVector::length() const
{
  return std::sqrt(x_*x_ + y_*y_ + z_*z_);
}

double NVector::dot(const NVector& other) const
{
  return x()*other.x() + y()*other.y() + z()*other.z();
}

NVector NVector::cross(const NVector& other)
{
  return NVector(y()*other.z() - z()*other.y(),
                 z()*other.x() - x()*other.z(),
                 x()*other.y() - y()*other.x());
}
