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
              int kx = 3, int ky = 3, double s = 0);

  int Surfit(const std::vector<double>& x,
              const std::vector<double>& y,
              const std::vector<double>& z,
              int kx = 3, int ky = 3, double s = 0);

  int Surfit(int n,
              const double *x,
              const double *y,
              const double *z,
              int kx = 3, int ky = 3, double s = 0);


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
