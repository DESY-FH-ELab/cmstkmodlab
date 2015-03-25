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

#include "nplane3D.h"
#include "nline3D.h"

NLine3D::NLine3D(const NPoint3D& point, const NDirection3D& direction)
: point_(point),
  direction_(direction)
{

}

NLine3D::NLine3D(const NLine3D& other)
: point_(other.point()),
  direction_(other.direction())
{

}

NLine3D::~NLine3D()
{

}

NPoint3D NLine3D::pointAt(double s)
{
  NPoint3D p(point());
  p.move(direction().x()*s,
         direction().y()*s,
         direction().z()*s);
  return p;
}

bool NLine3D::intersection(const NPlane3D& plane, NPoint3D& p)
{
  NVector3D u = plane.normal();
  if (u.dot(direction())==0.) return false;

  NVector3D w(point(), plane.point());
  double s = plane.normal().dot(w)/plane.normal().dot(direction());

  p = pointAt(s);

  return true;
}
