/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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
    parameter.a()[1] = -5.0;
    parameter.a()[2] = 1.0;

    std::cout << fitter.run() << std::endl;
    std::cout << parameter.a()[0] << std::endl;
    std::cout << parameter.a()[1] << std::endl;
    std::cout << parameter.a()[2] << std::endl;
    std::cout << std::endl;
  }

  {
    TkML::Gauss2DFitter fitter(1681);

    TkML::Gauss2DFitter::FitterData& data = fitter.data();
    TkML::Gauss2DFitter::FitterParameter& parameter = fitter.parameter();

    double x, y, z, maxz = 0;
    int nData = 0;
    std::ifstream ifile("gauss2Ddump.txt");
    while (ifile >> x >> y >> z) {
      data.x()[nData] = x;
      data.y()[nData] = y;
      data.z()[nData] = z;
      maxz = std::max(z, maxz);
      nData++;
    }

    parameter.a()[0] = maxz;
    parameter.a()[1] = -0.5;
    parameter.a()[2] = 1.0;
    parameter.a()[3] = 0.5;
    parameter.a()[4] = 2.0;

    fitter.run();

    /*
    fitter.initialize();
     */
    std::cout << parameter.a()[0] << std::endl;
    std::cout << parameter.a()[1] << std::endl;
    std::cout << parameter.a()[2] << std::endl;
    std::cout << parameter.a()[3] << std::endl;
    std::cout << parameter.a()[4] << std::endl;
    std::cout << std::endl;

    /*
    int iterations = 0;
    for (int i=0;i<5;++i) {
    //while (fitter.alambda()>1e-9) {
      fitter.MarquartMinimization();
      std::cout << fitter.alambda() << std::endl;
      std::cout << parameter.a()[0] << std::endl;
      std::cout << parameter.a()[1] << std::endl;
      std::cout << parameter.a()[2] << std::endl;
      std::cout << parameter.a()[3] << std::endl;
      std::cout << parameter.a()[4] << std::endl;
      std::cout << std::endl;

      iterations++;
    }
    */
  }

  return 0;
}
