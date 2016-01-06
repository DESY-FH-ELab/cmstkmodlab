void MaxExposuretimeVsPixelClock()
{
	TGraph * gr = new TGraph();
	gr->SetMarkerStyle(20);
	
	gr->SetPoint(0, 24, 333);
	gr->SetPoint(1, 36, 222);
	gr->SetPoint(2, 43, 186);
	
	gr->Draw("AP");
}