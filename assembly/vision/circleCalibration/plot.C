void plot()
{
	int t1, t2;
	float c, xy, r, q;
	
	std::ifstream ifile("dump.txt");
	
	TGraph2D * gr = new TGraph2D();
	int nPoints = 0;
	
	while (ifile >> t1 >> t2 >> c >> xy >> r >> q) {
		gr->SetPoint(nPoints, t1, t2, q);
		
		nPoints++;
	}
	
	gr->Draw("colz");
}
