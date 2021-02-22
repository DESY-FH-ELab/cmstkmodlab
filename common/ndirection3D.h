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

#ifndef NDIRECTION3D_H
#define NDIRECTION3D_H

#include <iostream>

#include <nvector3D.h>

/** @addtogroup common
 *  @{
 */

class NDirection3D : public NVector3D
{
public:

  NDirection3D();
  NDirection3D(double x, double y, double z);
  NDirection3D(const NPoint3D& a, const NPoint3D& b);
  NDirection3D(const NVector3D& other);
  ~NDirection3D();

protected:

  void normalize();
};

/** @} */

#endif // NDIRECTION3D_H
