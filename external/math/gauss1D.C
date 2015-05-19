void gauss1D()
{
  TH1F * h = new TH1F("h", "h", 41, -20.5, 20.5);

  for (int i=0;i<10000;++i) {
    h->Fill(gRandom->Gaus(-1.5, 3.0));
  }

  h->Draw();

  std::ofstream ofile("gauss1Ddump.txt");

  for (int bx=1;bx<=41;++bx) {
    ofile << h->GetXaxis()->GetBinCenter(bx) << "\t"
          << 0.0 << "\t"
          << h->GetBinContent(bx) << std::endl;
  }
}
