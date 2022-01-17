/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cmath>

#include <gsl/gsl_multifit.h>

#include "Thermo2ThroughPlaneFitter.h"

Thermo2ThroughPlaneFitter::Thermo2ThroughPlaneFitter()
{
  gsl_set_error_handler_off();
}

bool Thermo2ThroughPlaneFitter::fit(std::vector<std::pair<double,double> > &values, unsigned int order,
    double & p0, double & p1)
{
  unsigned int p = order;
  if (p<2) p = 2;
  if (p>3) p = 3;

  gsl_matrix *X, *cov;
  gsl_vector *x, *y, *c;
  gsl_multifit_robust_workspace * work;
  int ret;

  X = gsl_matrix_alloc(values.size(), p);
  x = gsl_vector_alloc(values.size());
  y = gsl_vector_alloc(values.size());
  c = gsl_vector_alloc(p);
  cov = gsl_matrix_alloc(p, p);

  unsigned int idx = 0;
  for (std::vector<std::pair<double,double> >::iterator it = values.begin();
       it!=values.end();
       ++it) {
    gsl_vector_set(x, idx, it->first);
    gsl_vector_set(y, idx, it->second);

    gsl_matrix_set(X, idx, 0, 1.0);
    gsl_matrix_set(X, idx, 1, it->first);
    if (p==3) gsl_matrix_set(X, idx, 2, it->first*it->first);

    idx++;
  }

  work = gsl_multifit_robust_alloc(gsl_multifit_robust_bisquare, X->size1, X->size2);
  work->maxiter = 1000;
  ret = gsl_multifit_robust(X, y, c, cov, work);
  gsl_multifit_robust_free(work);

  if (ret!=GSL_EMAXITER) {
    p0 = gsl_vector_get(c, 0);
    p1 = gsl_vector_get(c, 1);
  } else {
    p0 = 0.;
    p1 = 1.;
  }

  gsl_matrix_free(X);
  gsl_vector_free(x);
  gsl_vector_free(y);
  gsl_vector_free(c);
  gsl_matrix_free(cov);

  return (ret!=GSL_EMAXITER);
}
