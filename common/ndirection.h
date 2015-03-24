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

#ifndef NDIRECTION_H
#define NDIRECTION_H

#include <iostream>

#include <nvector.h>

class NDirection : public NVector
{
public:

  NDirection();
  NDirection(double x, double y, double z);
  NDirection(const NPoint& a, const NPoint& b);
  NDirection(const NVector& other);
  ~NDirection();

protected:

  void normalize();
};

#endif // NDIRECTION_H
