#ifndef TKMLFITTER_h
#define TKMLFITTER_h

#include <iostream>
#include <cmath>

#include <TkMLVector.h>
#include <TkMLMatrix.h>

namespace TkML {

  template<typename T>
  class Fitter
  {
  public:

    class FitterData
    {
    public:

      FitterData(size_t n)
      : x_(n), y_(n), z_(n), s_(n, 1.0) { }

      Vector<T> & x() { return x_; }
      const Vector<T> & x() const { return x_; }

      Vector<T> & y() { return y_; }
      const Vector<T> & y() const { return y_; }

      Vector<T> & z() { return z_; }
      const Vector<T> & z() const { return z_; }

      Vector<T> & s() { return s_; }
      const Vector<T> & s() const { return s_; }

      size_t size() const { return x_.size(); }

    protected:

      Vector<T> x_;
      Vector<T> y_;
      Vector<T> z_;
      Vector<T> s_;
    };

    class FitterParameter
    {
    public:

      FitterParameter(size_t n)
      : a_(n), ia_(n, true),
        covar_(n, n), alpha_(n, n) { }

      Vector<T> & a() { return a_; }
      const Vector<T> & a() const { return a_; }

      Vector<bool> & ia() { return ia_; }
      const Vector<bool> & ia() const { return ia_; }

      Matrix<T> & covar() { return covar_; }
      const Matrix<T> & covar() const { return covar_; }

      Matrix<T> & alpha() { return alpha_; }
      const Matrix<T> & alpha() const { return alpha_; }

      const T & chisq() const { return chisq_; }
      T & chisq() { return chisq_; }

      size_t size() const { return a_.size(); }

    protected:

      Vector<T> a_;
      Vector<bool> ia_;
      Matrix<T> covar_;
      Matrix<T> alpha_;
      T chisq_;
    };

    Fitter(size_t nd, size_t np)
    : data_(nd), parameter_(np),
      oneda_(np, 1),
      atry_(np), beta_(np), da_(np) { }

    FitterData& data() { return data_; }
    const FitterData& data() const { return data_; }

    FitterParameter& parameter() { return parameter_; }
    const FitterParameter& parameter() const { return parameter_; }

    const T & alambda() const { return alambda_; }

    int run(const T convergence = 1e-9) {
      int iterations = 0;

      initialize();

      T oc;
      int i = 0;
      for (;;) {
        oc = ochisq_;

        MarquartMinimization();

        //std::cout << oc << "\t" << parameter().chisq() << std::endl;

        if (parameter().chisq() > oc) {
          i = 0;
        } else if (fabs(oc-parameter().chisq())/parameter().chisq() < 1e-32) {
          //std::cout << fabs(oc-parameter().chisq())/parameter().chisq() << std::endl;
          i++;
        }
        iterations++;
        if (i==10) break;
      }

      /*
      while (alambda()>convergence) {
        MarquartMinimization();
        iterations++;
      }
      */

      return iterations;
    }

    void initialize() {
      mfit_ = 0;
      for (size_t i=0;i<parameter_.ia().size();++i) {
        if (parameter_.ia().at(i)==true) mfit_++;
      }

      alambda_ = 0.001;

      MarquartCoefficients(parameter().a(), parameter().alpha(), beta_);

      ochisq_ = parameter_.chisq();

      for (size_t i=0;i<parameter_.ia().size();++i) {
        atry_[i] = parameter_.a()[i];
        // std::cout << "try " << atry_[i] << std::endl;
      }
    }

    void MarquartMinimization() {
      int ma = parameter().size();

      Matrix<T> temp(mfit_, mfit_);

      for (int j=0;j<mfit_;++j) {
        for (int k=0;k<mfit_;++k) parameter().covar()[j][k] = parameter().alpha()[j][k];
        parameter().covar()[j][j] = parameter().alpha()[j][j] * (1.0+alambda_);
        for (int k=0;k<mfit_;++k) temp[j][k] = parameter().covar()[j][k];
        oneda_[j][0] = beta_[j];
      }

      GaussJordan(temp, oneda_);

      for (int j=0;j<mfit_;++j) {
        for (int k=0;k<mfit_;++k) parameter().covar()[j][k] = temp[j][k];
        da_[j] = oneda_[j][0];
      }

      if (alambda_==0.0) {
        // covsrt(parameter().covar(), parameter().ia(), mfit_);
        // covsrt(parameter().alpha(), parameter().ia(), mfit_);
        return;
      }

      for (int j=0,l=0;l<ma;++l)
        if (parameter().ia().at(l)==true) atry_[l] = parameter().a()[l] + da_[j++];

      MarquartCoefficients(atry_, parameter().covar(), da_);

      if (parameter().chisq()<ochisq_) {
        alambda_ *= 0.1;
        ochisq_ = parameter().chisq();

        for (int j=0;j<mfit_;++j) {
          for (int k=0;k<mfit_;++k) parameter().alpha()[j][k] = parameter().covar()[j][k];
          beta_[j] = da_[j];
        }
        for (int j=0;j<ma;++j) parameter().a()[j] = atry_[j];
      } else {
        alambda_ *= 10.0;
        parameter().chisq() = ochisq_;
      }
    }

    virtual void eval(const T x, const T y, Vector<T> & a,
                      T & zfit, Vector<T> & dzdp) = 0;

  protected:

    FitterData data_;
    FitterParameter parameter_;

    double alambda_;
    int mfit_;
    T ochisq_;
    Matrix<T> oneda_;
    Vector<T> atry_, beta_, da_;

    void MarquartCoefficients(Vector<T> & a, Matrix<T> & b, Vector<T> &da) {
      T zmod, wt, sig2i, dz;

      int ndata = data().size();
      int ma = a.size();

      Vector<T> dzdp(ma);

      for (int i=0;i<mfit_;++i) {
        for (int k=0;k<=i;++k) {
          b[i][k] = 0.0;
        }
        da[i] = 0.0;
      }

      parameter().chisq() = 0.0;

      for (int i=0;i<ndata;++i) {
        eval(data().x()[i], data().y()[i], a, zmod, dzdp);
        sig2i = 1. / (data().s()[i] * data().s()[i]);
        dz = data().z()[i] - zmod;
        for (int j=0,l=0;l<ma;++l) {
          if (parameter().ia().at(l)==true) {
            wt = dzdp[l]*sig2i;
            for (int k=0,m=0;m<l+1;m++) {
              if (parameter().ia().at(m)==true) {
                b[j][k++] += wt * dzdp[m];
              }
            }
            da[j++] += dz * wt;
          }
        }
        parameter().chisq() += dz * dz * sig2i;
      }

      for (int i=0;i<mfit_;++i) {
        for (int k=0;k<i;++k) {
          b[k][i] = b[i][k];
        }
      }
    }
  };

  class Gauss1DFitter : public Fitter<double>
  {
  public:
    Gauss1DFitter(size_t nd)
    : Fitter<double>(nd, 3) {

    }

    void eval(const double x, const double y,
              Vector<double> & a,
              double & zfit, Vector<double> & dzdp) {
      double fac, ex, arg;

      arg = (x - a[1])/a[2];
      ex = std::exp(-arg*arg);
      zfit = a[0] * ex;
      fac = a[0] * ex * 2.0 * arg;
      dzdp[0] = ex;
      dzdp[1] = fac / a[2];
      dzdp[2] = fac * arg / a[2];
    }
  };

  class Gauss2DFitter : public Fitter<double>
  {
  public:
    Gauss2DFitter(size_t nd)
    : Fitter<double>(nd, 5) {

    }

    void eval(const double x, const double y,
              Vector<double> & a,
              double & zfit, Vector<double> & dzdp) {
      double facx, facy, exx, exy, argx, argy;

      argx = (x - a[1])/a[2];
      argy = (y - a[3])/a[4];
      exx = std::exp(-argx*argx);
      exy = std::exp(-argy*argy);
      zfit = a[0] * exx * exy;
      facx = a[0] * exx * exy * 2.0 * argx;
      facy = a[0] * exx * exy * 2.0 * argy;
      dzdp[0] = exx * exy;
      dzdp[1] = facx / a[2];
      dzdp[2] = facx * argx / a[2];
      dzdp[3] = facy / a[4];
      dzdp[4] = facy * argy / a[4];
    }
  };

};

#endif // TKMLFITTER_h
