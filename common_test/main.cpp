#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include <nspline2D.h>

int main(int argc, char *argv[])
{
  std::cout << "test NSpline2D 1" << std::endl;
  {
     NSpline2D spline;

     double ax[] = { 1., 1., 1., 2., 2., 2., 3., 3., 3. };
     std::vector<double> x(ax, ax + sizeof(ax)/sizeof(double));

     double ay[] = { 1., 2., 3., 1., 2., 3., 1., 2., 3. };
     std::vector<double> y(ay, ay + sizeof(ay)/sizeof(double));

     double az[] = { 0., 0., 0., 2., 2., 2., 4., 4., 4. };
     std::vector<double> z(az, az + sizeof(az)/sizeof(double));

     spline.surfit(x, y, z, 1, 1, sizeof(ax)/sizeof(double));

     double r;
     std::cout << (int)spline.evaluate(2.0, 1.5, r) << " " << r << std::endl;

     std::cout << (int)spline.evaluate(1.0, 1.0, r) << " " << r << std::endl;
     std::cout << (int)spline.evaluate(1.0, 1.5, r) << " " << r << std::endl;
     std::cout << (int)spline.evaluate(1.5, 1.0, r) << " " << r << std::endl;
     std::cout << (int)spline.evaluate(1.5, 1.5, r) << " " << r << std::endl;
     std::cout << (int)spline.evaluate(2.0, 1.0, r) << " " << r << std::endl;
     std::cout << (int)spline.evaluate(2.0, 1.5, r) << " " << r << std::endl;
  }
  std::cout << std::endl;

  std::cout << "test NSpline2D 2" << std::endl;
  {
    NSpline2D spline;
    std::vector<double> x, y, z;

    for (double rx=-2.0;rx<=2.0;rx+=4.0/20.) {
      for (double ry=-2.0;ry<=2.0;ry+=4.0/20.) {
        x.push_back(rx);
        y.push_back(ry);
        z.push_back(rx + ry);
      }
    }

    spline.surfit(x, y, z, 3, 3, 20);

    double r;
    std::cout << (int)spline.evaluate(1.0, 1.0, r) << " " << r << std::endl;
  }
  std::cout << std::endl;

  std::cout << "test NSpline2D 3" << std::endl;
  {
    NSpline2D spline;

    std::vector<double> x, y, z;

    for (double rx=0.0;rx<=1.0;rx+=0.025) {
      for (double ry=0.0;ry<=1.0;ry+=0.025) {
        x.push_back(rx);
        y.push_back(ry);
        z.push_back(std::sin(rx) * std::sin(ry));
      }
    }

    spline.surfit(x, y, z, 3, 3, 20);

    double r, rx, ry;
    rx = 0.5; ry = 0.5; std::cout << (int)spline.evaluate(rx, ry, r) << " " << r << " " << std::sin(rx)*std::sin(ry) << " " << 100.*(1.0-r/(std::sin(rx)*std::sin(ry))) << std::endl;
    rx = 0.2; ry = 0.5; std::cout << (int)spline.evaluate(rx, ry, r) << " " << r << " " << std::sin(rx)*std::sin(ry) << " " << 100.*(1.0-r/(std::sin(rx)*std::sin(ry))) << std::endl;
    rx = 0.5; ry = 0.2; std::cout << (int)spline.evaluate(rx, ry, r) << " " << r << " " << std::sin(rx)*std::sin(ry) << " " << 100.*(1.0-r/(std::sin(rx)*std::sin(ry))) << std::endl;
    rx = 0.2; ry = 0.1; std::cout << (int)spline.evaluate(rx, ry, r) << " " << r << " " << std::sin(rx)*std::sin(ry) << " " << 100.*(1.0-r/(std::sin(rx)*std::sin(ry))) << std::endl;
  }
  std::cout << std::endl;

}
