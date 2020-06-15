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

#ifndef NLINE2D_H
#define NLINE2D_H

#include <iostream>

#include <npoint2D.h>
#include <ndirection2D.h>

/** @addtogroup common
 *  @{
 */

class NLine2D
{
public:

  NLine2D(const NPoint2D& point, const NDirection2D& direction);
  NLine2D(const NLine2D& other);
  ~NLine2D();

  const NPoint2D& point() const { return point_; }
  const NDirection2D& direction() const { return direction_; }

  NPoint2D pointAt(double s);

protected:

  NPoint2D point_;
  NDirection2D direction_;
};

/** @} */

#endif // NLINE2D_H
