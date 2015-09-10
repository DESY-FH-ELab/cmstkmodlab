struct calPoints {
  double focalLength;
  double imageScale;
};

calPoints data[] = {
  { 85.0, 0.0522222222222222 },
  { 78.0, 0.0500000000000000 },
  { 73.0, 0.0477777777777778 },
  { 70.0, 0.0455555555555556 },
  { 65.0, 0.0433333333333333 },
  { 61.0, 0.0411111111111111 },
  { 55.0, 0.0366666666666667 },
  { -1.0, 0.0 }
};

double funcObjectDistance(double* x, double* p)
{
  double f = x[0];
  double p0 = p[0];
  double p1 = p[1];
  double p2 = p[2];

  double gamma = p0 + p1 * f + p2 * f * f;

  return f * (gamma + 1.0) / gamma;
}

void CameraCalibration()
{
  TGraph * grGammaVsF = new TGraph();
  grGammaVsF->SetMarkerStyle(2);
  int nPointsGammaVsF = 0;

  TGraph * grXVsF = new TGraph();
  grXVsF->SetMarkerStyle(2);
  TGraph * grXVsF2 = new TGraph();
  grXVsF2->SetMarkerStyle(21);
  grXVsF2->SetMarkerColor(2);
  int nPointsXVsF = 0;

  TGraph * grBVsF = new TGraph();
  grBVsF->SetMarkerStyle(2);
  TGraph * grBVsF2 = new TGraph();
  grBVsF2->SetMarkerStyle(21);
  grBVsF2->SetMarkerColor(2);
   int nPointsBVsF = 0;

  TGraph * grGVsF = new TGraph();
  grGVsF->SetMarkerStyle(2);
  TGraph * grGVsF2 = new TGraph();
  grGVsF2->SetMarkerStyle(21);
  grGVsF2->SetMarkerColor(2);
  int nPointsGVsF = 0;

  while (data[nPointsGammaVsF].focalLength!=-1.0) {
    grGammaVsF->SetPoint(nPointsGammaVsF, 
			 data[nPointsGammaVsF].focalLength,
			 data[nPointsGammaVsF].imageScale);
    nPointsGammaVsF++;
  }

  TCanvas * c = new TCanvas("c", "c", 700, 500);
  TH1F * frame = c->DrawFrame(45, 0.011, 95, 0.07);
  frame->GetXaxis()->SetTitle("focal length [mm]");
  frame->GetYaxis()->SetTitle("image scale");
  
  grGammaVsF->Draw("P");

  TF1 * pol2 = new TF1("GammaVsF", "pol2");
  grGammaVsF->Fit(pol2);

  c->Print("CameraCalibration_GammaVsFocalLength.pdf");

  std::cout << "p0 = " << pol2->GetParameter(0) << std::endl;
  std::cout << "p1 = " << pol2->GetParameter(1) << std::endl;
  std::cout << "p2 = " << pol2->GetParameter(2) << std::endl;

  TF1 * pol1 = new TF1("LinVsF", "pol1");

  while (data[nPointsXVsF].focalLength!=-1.0) {
    grXVsF->SetPoint(nPointsXVsF, 
                     data[nPointsXVsF].focalLength,
    		     data[nPointsXVsF].focalLength*data[nPointsXVsF].imageScale);
    grXVsF2->SetPoint(nPointsXVsF, 
		     data[nPointsXVsF].focalLength,
		     data[nPointsXVsF].focalLength*pol2->Eval(data[nPointsXVsF].focalLength));
    nPointsXVsF++;
  }
  grXVsF2->Draw("AP");
  grXVsF->Draw("P");
  grXVsF->Fit(pol1);

  while (data[nPointsBVsF].focalLength!=-1.0) {
    grBVsF->SetPoint(nPointsBVsF, 
		     data[nPointsBVsF].focalLength,
		     data[nPointsBVsF].focalLength*(data[nPointsBVsF].imageScale + 1.0));
    grBVsF2->SetPoint(nPointsBVsF, 
		     data[nPointsBVsF].focalLength,
		     data[nPointsBVsF].focalLength*(pol2->Eval(data[nPointsBVsF].focalLength) + 1.0));
    nPointsBVsF++;
  }
  grBVsF2->Draw("AP");
  //grBVsF->Draw("P");
  grBVsF2->Fit(pol1);

  grBVsF2->GetHistogram()->GetXaxis()->SetTitle("focal length [mm]");
  grBVsF2->GetHistogram()->GetYaxis()->SetTitle("image distance [mm]");
  c->Print("CameraCalibration_ImageDistanceVsFocalLength.pdf");

  while (data[nPointsGVsF].focalLength!=-1.0) {
    grGVsF->SetPoint(nPointsGVsF, 
		     data[nPointsGVsF].focalLength,
		     data[nPointsGVsF].focalLength*(data[nPointsGVsF].imageScale + 1.0)/data[nPointsGVsF].imageScale);
    grGVsF2->SetPoint(nPointsGVsF, 
		     data[nPointsGVsF].focalLength,
		     data[nPointsGVsF].focalLength*(pol2->Eval(data[nPointsGVsF].focalLength) + 1.0)/pol2->Eval(data[nPointsGVsF].focalLength));
    nPointsGVsF++;
  }
  grGVsF2->Draw("AP");

  TF1 * odf = new TF1("odf", funcObjectDistance, 45, 95, 3);
  odf->SetParameter(0, pol2->GetParameter(0));
  odf->SetParameter(1, pol2->GetParameter(1));
  odf->SetParameter(2, pol2->GetParameter(2));

  grGVsF2->Fit(odf);
  std::cout << "p0 = " << odf->GetParameter(0) << std::endl;
  std::cout << "p1 = " << odf->GetParameter(1) << std::endl;
  std::cout << "p2 = " << odf->GetParameter(2) << std::endl;

  grBVsF2->GetHistogram()->GetXaxis()->SetTitle("focal length [mm]");
  grBVsF2->GetHistogram()->GetYaxis()->SetTitle("object distance [mm]");
  c->Print("CameraCalibration_ObjectDistanceVsFocalLength.pdf");
}
