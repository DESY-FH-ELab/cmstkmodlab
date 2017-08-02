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
