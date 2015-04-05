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

  gr1->Draw("SURF1Z");

  TkModLabSpline2D spline;

  spline.Surfit(gr1->GetN(),
                gr1->GetX(), gr1->GetY(), gr1->GetZ());

  TGraph *gr21D = new TGraph();
  TGraph2D *gr2 = new TGraph2D();
  int nPoints2 = 0;

  for (int i=0;i<gr1->GetN();i++) {
    double z;
    spline.Evaluate(gr1->GetX()[i], gr1->GetY()[i], z);
    gr2->SetPoint(nPoints2++, gr1->GetX()[i], gr1->GetY()[i], z);
  }

  gr2->Draw("SURF1Z");

  TGraph2D *gr3 = new TGraph2D();
  int nPoints3 = 0;

  for (int i=0;i<gr1->GetN();i++) {
    double z;
    spline.Evaluate(gr1->GetX()[i], gr1->GetY()[i], z);
    gr3->SetPoint(nPoints3++, gr1->GetX()[i], gr1->GetY()[i], z - gr1->GetZ()[i]);
  }

  gr3->Draw("SURF1Z");
}
