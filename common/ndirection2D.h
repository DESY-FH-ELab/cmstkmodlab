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

#ifndef NDIRECTION2D_H
#define NDIRECTION2D_H

#include <iostream>

#include <nvector2D.h>

/** @addtogroup common
 *  @{
 */

class NDirection2D : public NVector2D
{
public:

  NDirection2D();
  NDirection2D(double x, double y);
  NDirection2D(const NPoint2D& a, const NPoint2D& b);
  NDirection2D(const NVector2D& other);
  ~NDirection2D();

protected:

  void normalize();
};

/** @} */

#endif // NDIRECTION2D_H
