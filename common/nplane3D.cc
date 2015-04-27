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

NPlane3D::NPlane3D(const NPoint3D& point, const NDirection3D& normal)
: point_(point),
  normal_(normal)
{

}

NPlane3D::NPlane3D(const NPlane3D& other)
: point_(other.point()),
  normal_(other.normal())
{

}

NPlane3D::~NPlane3D()
{

}
