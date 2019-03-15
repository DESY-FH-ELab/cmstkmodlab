 /****************************************************************************
 **
 **  Copyright (C) 2014,2015 Andreas Mussgiller
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
 ****************************************************************************/
#ifndef TKMODLABSPLINE2D_H
#define TKMODLABSPLINE2D_H

#include <vector>

#include <TObject.h>

class TkModLabSpline2D : public TObject
{
 public:

  TkModLabSpline2D();

  int Surfit(const std::vector<double>& x,
              const std::vector<double>& y,
              const std::vector<double>& z,
              const std::vector<double>& w,
              int kx = 3, int ky = 3, double s = 0,
              double nxy = 1.0);

  int Surfit(const std::vector<double>& x,
              const std::vector<double>& y,
              const std::vector<double>& z,
              int kx = 3, int ky = 3, double s = 0,
              double nxy = 1.0);

  int Surfit(int n,
              const double *x,
              const double *y,
              const double *z,
              int kx = 3, int ky = 3, double s = 0,
              double nxy = 1.0);


  void Regrid(const std::vector<double>& x,
              const std::vector<double>& y,
              const std::vector<double>& z,
              int kx = 3, int ky = 3, double s = 0);

  bool Evaluate(double x, double y, double& z);

  bool Evaluate(const std::vector<double>& x,
                const std::vector<double>& y,
                std::vector<double>& z);

 protected:

  int CalcSurfitLwrk1(int m,
                      int ky, int kx,
                      int nyest, int nxest);

  std::vector<double> tx_;
  std::vector<double> ty_;
  std::vector<double> c_;
  int kx_;
  int ky_;
  double fp_;

  ClassDef(TkModLabSpline2D,0);
};

#endif // TKMODLABSPLINE2D_H
