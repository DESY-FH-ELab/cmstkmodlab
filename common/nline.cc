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

#include "nplane.h"
#include "nline.h"

NLine::NLine(const NPoint& point, const NDirection& direction)
: point_(point),
  direction_(direction)
{

}

NLine::NLine(const NLine& other)
: point_(other.point()),
  direction_(other.direction())
{

}

NLine::~NLine()
{

}

NPoint NLine::pointAt(double s)
{
  NPoint p(point());
  p.move(direction().x()*s,
         direction().y()*s,
         direction().z()*s);
  return p;
}

bool NLine::intersection(const NPlane& plane, NPoint& p)
{
  NVector u = plane.normal();
  if (u.dot(direction())==0.) return false;

  NVector w(point(), plane.point());
  double s = plane.normal().dot(w)/plane.normal().dot(direction());

  p = pointAt(s);

  return true;
}
