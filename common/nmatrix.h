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

#ifndef NMATRIX_H
#define NMATRIX_H

#include <vector>

template<typename T> class NMatrix
{
public:

  NMatrix(unsigned int N, unsigned int M)
  :N_(N), M_(M) {
    data_.resize(N*M, T());
  }
  ~NMatrix() {

  }

  unsigned int N() const { return N_; }
  unsigned int M() const { return M_; }

  T operator()(int n, int m) const {
    return data_[m*N_+n];
  }
  T &operator()(int n, int m) {
    return data_[m*N_+n];
  }

protected:

  unsigned int N_;
  unsigned int M_;
  std::vector<T> data_;
};

#endif // NMATRIX_H
