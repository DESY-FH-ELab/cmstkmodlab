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
#include "ndirection3D.h"

NDirection3D::NDirection3D()
: NVector3D(0., 0., 1.)
{

}

NDirection3D::NDirection3D(double x, double y, double z)
: NVector3D(x, y, z)
{
  normalize();
}

NDirection3D::NDirection3D(const NPoint3D& a, const NPoint3D& b)
: NVector3D(a, b)
{
  normalize();
}

NDirection3D::NDirection3D(const NVector3D& other)
: NVector3D(other)
{
  normalize();
}

NDirection3D::~NDirection3D()
{

}

void NDirection3D::normalize()
{
  double l = length();
  x_ /= l;
  y_ /= l;
  z_ /= l;
}
