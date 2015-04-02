#ifndef NSPLINE2D_H
#define NSPLINE2D_H

#include <vector>

class NSpline2D
{

 public:

  NSpline2D();

  void surfit(const std::vector<double>& x,
              const std::vector<double>& y,
              const std::vector<double>& z,
              const std::vector<double>& w,
              int kx = 3, int ky = 3, double s = 0);

  void surfit(const std::vector<double>& x,
              const std::vector<double>& y,
              const std::vector<double>& z,
              int kx = 3, int ky = 3, double s = 0);

  void regrid(const std::vector<double>& x,
              const std::vector<double>& y,
              const std::vector<double>& z,
              int kx = 3, int ky = 3, double s = 0);

  bool evaluate(double x, double y, double& z);

  bool evaluate(const std::vector<double>& x,
                const std::vector<double>& y,
                std::vector<double>& z);

 protected:

  int calcSurfitLwrk1(int m,
                      int ky, int kx,
                      int nyest, int nxest);

  std::vector<double> tx_;
  std::vector<double> ty_;
  std::vector<double> c_;
  int kx_;
  int ky_;
  double fp_;
};

#endif //NSPLINE2D_H
