/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
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
  TCanvas * c = new TCanvas("c", "c", 700, 500);
  
  int ix, iy;
  double x, y, zx, zy;
  double hx, hy;
  
  TGraph2D *gr1 = new TGraph2D();
  int nPoints1 = 0;
  
  //std::ifstream ifile1("defoReco_20150330095913_20150330100348.txt");
  //std::ifstream ifile1("defoReco_20150401074552_20150401074653.txt");
  //std::ifstream ifile1("defoReco_20150401074552_20150401074812.txt");
  std::ifstream ifile1("defoReco_20150401143932_20150401144027.txt");
  while (ifile1 >> ix >> iy >> x >> y >> zx >> hx >> zy >> hy) {
    zx *= 1e3;
    zy *= 1e3;
    
    gr1->SetPoint(nPoints1, x, y, zx);
    nPoints1++;
  }
  
  // gr1->Draw("SURF1Z");
  // gr1->Draw("P0");
  
  // c->Print("defoReco.png");
  
  TkModLabSpline2D spline;

  std::cout << spline.Surfit(gr1->GetN(),
			     gr1->GetX(), gr1->GetY(), gr1->GetZ(),
			     3, 3, 1,
			     0.75) << std::endl;

  TGraph *gr21D = new TGraph();
  TGraph2D *gr2 = new TGraph2D();
  int nPoints2 = 0;

  for (int i=0;i<gr1->GetN();i++) {
    double z;
    spline.Evaluate(gr1->GetX()[i], gr1->GetY()[i], z);
    gr2->SetPoint(nPoints2++, gr1->GetX()[i], gr1->GetY()[i], z);
  }

  // gr2->Draw("SURF1Z");
  // gr2->Draw("P0");

  // c->Print("defoRecoSpline2D.png");

  // return;

  TGraph2D *gr3 = new TGraph2D();
  int nPoints3 = 0;

  for (int i=0;i<gr1->GetN();i++) {
    double z;
    spline.Evaluate(gr1->GetX()[i], gr1->GetY()[i], z);
    gr3->SetPoint(nPoints3++, gr1->GetX()[i], gr1->GetY()[i], z - gr1->GetZ()[i]);
  }
  
  gr3->Draw("SURF1Z");

  // c->Print("defoRecoSpline2DMinusReco.png");
}
