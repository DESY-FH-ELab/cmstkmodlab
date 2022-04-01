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

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>

#include <QApplication>

#include <nqlogger.h>

#include <nvector3D.h>
#include <npoint3D.h>
#include <ndirection3D.h>
#include <nplane3D.h>
#include <nline3D.h>

#include <nspline2D.h>

#include <SlackBot.h>

#include <MattermostBot.h>

#include <Fifo.h>
#include <HistoryFifo.h>
#include <ValueHistory.h>

#include <ApplicationConfig.h>

#include <MartaModel.h>

#include "CommonTestMainWindow.h"

double imageScale(double focalLength)
{
  double p0 = -0.0240888;
  double p1 = 0.00148648;
  double p2 = -6.91494e-06;

  return p0 + p1 * focalLength + p2 * focalLength * focalLength;
}

int main(int argc, char ** argv)
{
  NQLogger::instance()->addActiveModule("*");
  NQLogger::instance()->addDestiniation(stdout, NQLog::Debug);

  /*
  {
    QApplication app(argc, argv);

    app.setStyle("cleanlooks");

    ApplicationConfig * config = ApplicationConfig::instance();
    config->append(std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo2/thermo2.cfg", "main");

    CommonTestMainWindow mainWindow;

    mainWindow.show();

    return app.exec();
  }
  */

  /*
  {
    MartaModel *marta = new MartaModel("10.255.21.224:502");

    delete marta;
  }
  */
  
  /*
  {
    double f = 50.0;
    double gamma = imageScale(f);
    double imageDistance = f * (gamma + 1.0);
    double objectDistance = imageDistance / gamma;

    NVector3D height1(0., 0., 1720.); // HEIGHT1
    NVector3D height2(0., 0., 150.);  // HEIGHT2
    NVector3D distance(0., -1.0*437., 0.); // DISTANCE
    double angle1 = 20.9 * M_PI / 180.;
    double angle2 = 16.2 * M_PI / 180.;
    double angle3 =  0.4 * M_PI / 180.;

    distance.print();
    distance.rotateX(angle2);
    distance.print();

    std::cout << std::endl;

    NPoint3D cameraPoint(0., 0., 0.);
    cameraPoint.print();
    cameraPoint.move(height1);
    cameraPoint.move(distance);
    cameraPoint.print();

    std::cout << std::endl;

    NPoint3D objectPoint(0., 0., 0.);
    objectPoint.move(height2);
    NDirection3D objectNormal(0., 0., 1.);
    NPlane3D objectPlane(objectPoint, objectNormal);

    NDirection3D centerRayDirection(0., 0., -1.);
    centerRayDirection.print();
    centerRayDirection.rotateX(angle2 + angle3);
    centerRayDirection.print();

    std::cout << std::endl;

    NLine3D centerRay(cameraPoint, centerRayDirection);
    objectPoint.print();
    centerRay.intersection(objectPlane, objectPoint);
    objectPoint.print();

    std::cout << std::endl;

    NVector3D imageDistanceVector(objectPoint, cameraPoint);
    imageDistanceVector.print();
    imageDistanceVector *= objectDistance / imageDistanceVector.length();
    imageDistanceVector.print();

    std::cout << std::endl;

    NPoint3D imagePoint(objectPoint);
    imagePoint.print();
    imagePoint.move(imageDistanceVector);
    imagePoint.print();

    std::cout << std::endl;

    NPoint3D gridPoint(0., 0., 0.);
    gridPoint.move(height1);

    NDirection3D gridNormal(0., 0., -1.);
    gridNormal.print();
    gridNormal.rotateX(-angle1);
    gridNormal.print();

    std::cout << std::endl;

    NPlane3D gridPlane(gridPoint, gridNormal);

    NPoint3D objectIntersection;
    NPoint3D gridIntersection;
    NDirection3D imageBeamDirection(0., 0., -1.);
    imageBeamDirection.rotateX(angle2 + angle3);
    NLine3D imageBeam(imagePoint, imageBeamDirection);
    imageBeam.intersection(objectPlane, objectIntersection);

    NDirection3D gridBeamDirection(imageBeamDirection);
    gridBeamDirection.rotateZ(M_PI);
    NLine3D gridBeam(objectIntersection, gridBeamDirection);
    gridBeam.intersection(gridPlane, gridIntersection);

    objectIntersection.print();
    gridIntersection.print();
  }
  */

  /*
  {
    int ix, iy;
    double x, y, zx, zy;
    double hx, hy;

    std::map<std::pair<int,int>, std::pair<double,double>> ri;
    std::vector<double> rx, ry, rzx, rzy;

    int ixmin = 10000, ixmax = -10000, iymin = 10000, iymax = -10000;

    std::ifstream ifile("defoReco_20150401074552_20150401074653.txt");
    while (ifile >> ix >> iy >> x >> y >> zx >> hx >> zy >> hy) {

      ixmin = std::min(ixmin, ix);
      ixmax = std::max(ixmax, ix);
      iymin = std::min(iymin, iy);
      iymax = std::max(iymax, iy);

      ri[std::pair<int,int>(ix, iy)] = std::pair<double,double>(x, y);
      rx.push_back(x);
      ry.push_back(y);
      rzx.push_back(zx);
      rzy.push_back(zy);
    }

    double s = 0.0; // no smoothing

    NSpline2D splineX;
    splineX.surfit(rx, ry, rzx, 3, 3, s);

    NSpline2D splineY;
    splineY.surfit(rx, ry, rzy, 3, 3, s);

    double xmin = -10000, xmax = 10000, ymin = -10000, ymax = 10000;
    for (auto i : ri) {
      if (i.first.first == ixmin) {
        xmin = std::max(xmin, i.second.first);
      }
      if (i.first.first == ixmax) {
        xmax = std::min(xmax, i.second.first);
      }
      if (i.first.second == iymin) {
        ymin = std::max(ymin, i.second.second);
      }
      if (i.first.second == iymax) {
        ymax = std::min(ymax, i.second.second);
      }
    }

    std::cout << xmin << std::endl;
    std::cout << xmax << std::endl;
    std::cout << ymin << std::endl;
    std::cout << ymax << std::endl << std::endl;

    std::vector<double> rgx, rgy, rgzx, rgzy;

    double dx = (xmax - xmin)/19.0;
    double dy = (ymax - ymin)/19.0;
    for (double gx=xmin;gx<=xmax;gx+=dx) {
      for (double gy=ymin;gy<=ymax;gy+=dy) {
        rgx.push_back(gx);
        rgy.push_back(gy);
        rgzx.push_back(0.0);
        rgzy.push_back(0.0);
      }
    }

    splineX.evaluate(rgx, rgy, rgzx);
    splineY.evaluate(rgx, rgy, rgzy);

    std::ofstream ofile1("defoSpline_20150401074552_20150401074653.txt");
    auto ity = rgy.begin();
    auto itzx = rgzx.begin();
    auto itzy = rgzy.begin();
    for (auto x : rgx) {
      ofile1  << std::setw(14) << std::scientific << x << " "
          << std::setw(14) << std::scientific << *ity << " "
          << std::setw(14) << std::scientific << *itzx << " "
          << std::setw(14) << std::scientific << *itzy << std::endl;
      ++ity;
      ++itzx;
      ++itzy;
    }

    rgx.resize(rx.size());
    rgy.resize(ry.size());
    rgzx.resize(rzx.size());
    rgzy.resize(rzy.size());

    rgx.assign(rx.begin(), rx.end());
    rgy.assign(ry.begin(), ry.end());

    splineX.evaluate(rgx, rgy, rgzx);
    splineY.evaluate(rgx, rgy, rgzy);

    std::ofstream ofile2("defoDiff_20150401074552_20150401074653.txt");
    ity = rgy.begin();
    auto izx = rzx.begin();
    auto izy = rzy.begin();
    itzx = rgzx.begin();
    itzy = rgzy.begin();
    for (auto x : rgx) {
      ofile2  << std::setw(14) << std::scientific << x << " "
          << std::setw(14) << std::scientific << *ity << " "
          << std::setw(14) << std::scientific << (*itzx)-(*izx) << " "
          << std::setw(14) << std::scientific << (*itzy)-(*izy) << std::endl;
      ++ity;
      ++izx;
      ++izy;
      ++itzx;
      ++itzy;
    }
  }
  */

  /*
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
     std::cout << (int)spline.evaluate(2.0, 1.5, r);
     std::cout << " " << r << std::endl;
     std::cout << (int)spline.evaluate(1.0, 1.0, r);
     std::cout << " " << r << std::endl;
     std::cout << (int)spline.evaluate(1.0, 1.5, r);
     std::cout << " " << r << std::endl;
     std::cout << (int)spline.evaluate(1.5, 1.0, r);
     std::cout << " " << r << std::endl;
     std::cout << (int)spline.evaluate(1.5, 1.5, r);
     std::cout << " " << r << std::endl;
     std::cout << (int)spline.evaluate(2.0, 1.0, r);
     std::cout << " " << r << std::endl;
     std::cout << (int)spline.evaluate(2.0, 1.5, r);
     std::cout << " " << r << std::endl;
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
    std::cout << (int)spline.evaluate(1.0, 1.0, r);
    std::cout << " " << r << std::endl;
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
    rx = 0.5; ry = 0.5; std::cout << (int)spline.evaluate(rx, ry, r);
    std::cout << " " << r << " " << std::sin(rx)*std::sin(ry) << " " << 100.*(1.0-r/(std::sin(rx)*std::sin(ry))) << std::endl;
    rx = 0.2; ry = 0.5; std::cout << (int)spline.evaluate(rx, ry, r);
    std::cout << " " << r << " " << std::sin(rx)*std::sin(ry) << " " << 100.*(1.0-r/(std::sin(rx)*std::sin(ry))) << std::endl;
    rx = 0.5; ry = 0.2; std::cout << (int)spline.evaluate(rx, ry, r);
    std::cout << " " << r << " " << std::sin(rx)*std::sin(ry) << " " << 100.*(1.0-r/(std::sin(rx)*std::sin(ry))) << std::endl;
    rx = 0.2; ry = 0.1; std::cout << (int)spline.evaluate(rx, ry, r);
    std::cout << " " << r << " " << std::sin(rx)*std::sin(ry) << " " << 100.*(1.0-r/(std::sin(rx)*std::sin(ry))) << std::endl;
  }
  std::cout << std::endl;
  */

  /*
  {
  	SlackBot bot("TestBot", "https://hooks.slack.com/services/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",
  			"#testbot");

  	bot.postMessage("message from Qt");
  }
  */

  /*
  {
    ApplicationConfig * config = ApplicationConfig::instance("mattermost.cfg");

    MattermostBot bot("test", "cmstkmodlab");

    bot.postMessage("message from cmstkmodlab");
  }
  */

  /*
  {
    ApplicationConfig * config = ApplicationConfig::instance();

    config->append("test1.cfg", "test1");
    config->append("test2.cfg", "test2");

    config->save();
    config->saveAs("testout.cfg", "test");
  }
  */
}
