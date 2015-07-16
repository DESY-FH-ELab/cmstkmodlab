double func_(double *x, double*p)
{
  return p[0]*TMath::Gaus(x[0], p[1], p[2])*TMath::Gaus(x[1], p[3], p[4]);
}

void gauss2D()
{
  TH2F * h = new TH2F("h", "h", 41, -20.5, 20.5, 41, -20.5, 20.5);

  /*
  for (int i=0;i<50000;++i) {
    h->Fill(gRandom->Gaus(-1.5, 2.0), gRandom->Gaus(2.0, 4.0));
  }
  */
  double x, y, r;
  for (int i=0;i<100000;++i) {
    x = gRandom->Uniform(-5, 5);
    y = gRandom->Uniform(-5, 5);
    r = TMath::Sqrt(x*x+y*y);
    if (r>5) continue;
    h->Fill(x, y);
  }

  h->Draw("lego2");

  TF2 *f2 = new TF2("f2", func_, -20.5, 20.5, -20.5, 20.5, 5);
  f2->SetParameter(0, 1000);
  f2->SetParameter(1, 0);
  f2->SetParameter(2, 1);
  f2->SetParameter(3, 0);
  f2->SetParameter(4, 1);

  h->Fit(f2);

  //f2->Draw("surf");
  /*
  std::ofstream ofile("gauss2Ddump.txt");

  for (int bx=1;bx<=41;++bx) {
    for (int by=1;by<=41;++by) {
      ofile << h->GetXaxis()->GetBinCenter(bx) << "\t"
            << h->GetXaxis()->GetBinCenter(bx) << "\t"
            << h->GetBinContent(bx, by) << std::endl;
    }
  }
  */
}
