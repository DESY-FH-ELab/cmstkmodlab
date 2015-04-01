#include <nqlogger.h>

#include "nspline2D.h"

extern "C" {
  void fortfunc_(int *ii, float *ff);
}

NSpline2D::NSpline2D()
{
  int ii=5;
  float ff=5.5;

  fortfunc_(&ii, &ff);
}
