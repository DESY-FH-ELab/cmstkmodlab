void gauss2D()
{
  TH2F * h = new TH2F("h", "h", 41, -20.5, 20.5, 41, -20.5, 20.5);

  for (int i=0;i<50000;++i) {
    h->Fill(gRandom->Gaus(-1.5, 2.0), gRandom->Gaus(2.0, 4.0));
  }

  h->Draw();

  std::ofstream ofile("gauss2Ddump.txt");

  for (int bx=1;bx<=41;++bx) {
    for (int by=1;by<=41;++by) {
      ofile << h->GetXaxis()->GetBinCenter(bx) << "\t"
            << h->GetXaxis()->GetBinCenter(bx) << "\t"
            << h->GetBinContent(bx, by) << std::endl;
    }
  }
}
