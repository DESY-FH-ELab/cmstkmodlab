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

#ifndef NLINE3D_H
#define NLINE3D_H

#include <iostream>

#include <npoint3D.h>
#include <ndirection3D.h>

class NPlane3D;

/** @addtogroup common
 *  @{
 */

class NLine3D
{
public:

  NLine3D(const NPoint3D& point, const NDirection3D& direction);
  NLine3D(const NLine3D& other);
  ~NLine3D();

  const NPoint3D& point() const { return point_; }
  const NDirection3D& direction() const { return direction_; }

  NPoint3D pointAt(double s);

  bool intersection(const NPlane3D& plane, NPoint3D& point);

protected:

  NPoint3D point_;
  NDirection3D direction_;
};

/** @} */

#endif // NLINE3D_H
