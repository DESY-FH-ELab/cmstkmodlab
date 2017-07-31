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

void plot()
{
  int ix, iy;
  double x, y, zx, zy;
  double hx, hy;

  TGraph2D *gr1 = new TGraph2D();
  int nPoints1 = 0;

  std::ifstream ifile1("defoReco_20150401074552_20150401074653.txt");
  while (ifile1 >> ix >> iy >> x >> y >> zx >> hx >> zy >> hy) {
    zx *= 1e3;
    zy *= 1e3;

    gr1->SetPoint(nPoints1, x, y, zx);
    nPoints1++;
  }

  //gr1->Draw("SURF1Z");

  TGraph *gr21D = new TGraph();
  TGraph2D *gr2 = new TGraph2D();
  int nPoints2 = 0;

  std::ifstream ifile2("defoSpline_20150401074552_20150401074653.txt");
  while (ifile2 >> x >> y >> zx >> zy) {
    zx *= 1e3;
    zy *= 1e3;

    gr2->SetPoint(nPoints2, x, y, zx);
    gr21D->SetPoint(nPoints2, x, y);
    nPoints2++;
  }

  //gr21D->Draw("AP");
  gr2->Draw("SURF1Z");

  //return;

  TGraph2D *gr3 = new TGraph2D();
  int nPoints3 = 0;

  std::ifstream ifile3("defoDiff_20150401074552_20150401074653.txt");
  while (ifile3 >> x >> y >> zx >> zy) {
    zx *= 1e3;
    zy *= 1e3;

    gr3->SetPoint(nPoints3, x, y, zx);
    nPoints3++;
  }

  gr3->Draw("SURF1Z");
}
