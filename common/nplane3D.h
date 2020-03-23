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

#ifndef NPLANE3D_H
#define NPLANE3D_H

#include <iostream>

#include <npoint3D.h>
#include <ndirection3D.h>

/** @addtogroup common
 *  @{
 */

class NPlane3D
{
public:

  NPlane3D(const NPoint3D& point, const NDirection3D& normal);
  NPlane3D(const NPlane3D& other);
  ~NPlane3D();

  const NPoint3D& point() const { return point_; }
  const NDirection3D& normal() const { return normal_; }

protected:

  NPoint3D point_;
  NDirection3D normal_;
};

/** @} */

#endif // NPLANE3D_H
