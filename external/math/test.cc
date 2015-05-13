#include <iostream>
#include <fstream>

#include <TkMLVector.h>
#include <TkMLMatrix.h>
#include <TkMLFitter.h>

int main(int argc, char * argv[])
{
  {
    TkML::Vector<double> v1(10, 1.2345);

    v1[4] = 2.6789;

    TkML::Vector<int> v2;

    v2 = v1;

    v1.print();

    v2.print();

    std::cout << std::endl;
  }

  {
    TkML::Matrix<double> m1(3, 4, 1.2345);

    m1[1][2] = 2.6789;

    TkML::Matrix<int> m2;

    m2 = m1;

    m1.print();

    std::cout << std::endl;

    m2.print();

    std::cout << std::endl;
  }

  {
    TkML::Gauss1DFitter fitter(41);

    TkML::Gauss1DFitter::FitterData& data = fitter.data();
    TkML::Gauss1DFitter::FitterParameter& parameter = fitter.parameter();

    double x, y, z, maxz = 0;
    int nData = 0;
    std::ifstream ifile("gauss1Ddump.txt");
    while (ifile >> x >> y >> z) {
      data.x()[nData] = x;
      data.y()[nData] = y;
      data.z()[nData] = z;
      maxz = std::max(z, maxz);
      nData++;
    }

    parameter.a()[0] = maxz;
    parameter.a()[1] = 0.0;
    parameter.a()[2] = 1.0;

    /*
    fitter.initialize();

    std::cout << fitter.alambda() << std::endl;
    std::cout << parameter.a()[0] << std::endl;
    std::cout << parameter.a()[1] << std::endl;
    std::cout << parameter.a()[2] << std::endl;
    std::cout << std::endl;

    int iterations = 0;
    while (fitter.alambda()>1e-9) {
      fitter.MarquartMinimization();
      std::cout << fitter.alambda() << std::endl;
      std::cout << parameter.a()[0] << std::endl;
      std::cout << parameter.a()[1] << std::endl;
      std::cout << parameter.a()[2] << std::endl;
      std::cout << std::endl;

      iterations++;
    }
    */

    std::cout << fitter.run() << std::endl;
    std::cout << parameter.a()[0] << std::endl;
    std::cout << parameter.a()[1] << std::endl;
    std::cout << parameter.a()[2] << std::endl;
    std::cout << std::endl;
  }

  return 0;
}
