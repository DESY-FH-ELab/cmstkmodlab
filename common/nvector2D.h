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

#ifndef NVECTOR2D_H
#define NVECTOR2D_H

#include <iostream>

class NPoint2D;

/** @addtogroup common
 *  @{
 */

class NVector2D
{
public:

  NVector2D();
  NVector2D(double x, double y);
  NVector2D(const NPoint2D& a, const NPoint2D& b);
  NVector2D(const NVector2D& other);
  ~NVector2D();

  double x() const { return x_; }
  double y() const { return y_; }

  void rotate(double angle);

  double length() const;
  double dot(const NVector2D& other) const;

protected:

  double x_, y_;
};

/** @} */

#endif // NVECTOR2D_H
