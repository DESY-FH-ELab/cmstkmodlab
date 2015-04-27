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

#include "npoint3D.h"
#include "nvector3D.h"

NVector3D::NVector3D()
: x_(0.),
  y_(0.),
  z_(0.)
{

}

NVector3D::NVector3D(double x, double y, double z)
: x_(x),
  y_(y),
  z_(z)
{

}

NVector3D::NVector3D(const NPoint3D& a, const NPoint3D& b)
: x_(b.x() - a.x()),
  y_(b.y() - a.y()),
  z_(b.z() - a.z())
{

}

NVector3D::NVector3D(const NVector3D& other)
: x_(other.x()),
  y_(other.y()),
  z_(other.z())
{

}

NVector3D::~NVector3D()
{

}

void NVector3D::rotateX(double angle)
{
  double yn =  1.0*y()*std::cos(angle) - 1.0*z()*std::sin(angle);
  double zn =  1.0*y()*std::sin(angle) + 1.0*z()*std::cos(angle);
  y_ = yn;
  z_ = zn;
}

void NVector3D::rotateY(double angle)
{
  double xn =  1.0*x()*std::cos(angle) + 1.0*z()*std::sin(angle);
  double zn = -1.0*x()*std::sin(angle) + 1.0*z()*std::cos(angle);
  x_ = xn;
  z_ = zn;
}

void NVector3D::rotateZ(double angle)
{
  double xn = 1.0*x()*std::cos(angle) - 1.0*y()*std::sin(angle);
  double yn = 1.0*x()*std::sin(angle) + 1.0*y()*std::cos(angle);
  x_ = xn;
  y_ = yn;
}

double NVector3D::length() const
{
  return std::sqrt(x_*x_ + y_*y_ + z_*z_);
}

double NVector3D::dot(const NVector3D& other) const
{
  return x()*other.x() + y()*other.y() + z()*other.z();
}

NVector3D NVector3D::cross(const NVector3D& other)
{
  return NVector3D(y()*other.z() - z()*other.y(),
                   z()*other.x() - x()*other.z(),
                   x()*other.y() - y()*other.x());
}
