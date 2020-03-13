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

void plot(const char * filename)
{
	gROOT->Reset();

	TFile *file = (TFile*)gROOT->GetListOfFiles()->FindObject(filename);
	if (!file) {
		file = new TFile(filename);
	}

	TTree *tree;
	file->GetObject("pumpstation", tree);

	tree->Print();

	UInt_t          uTimeOffset;
	UInt_t          uTime;
	Int_t           switchState0;
	Int_t           switchState1;
	Int_t           switchState2;
	Int_t           switchState3;
	Int_t           switchState4;
	Int_t           gaugeState0;
	Double_t        pressure0;
	Int_t           gaugeState1;
	Double_t        pressure1;
	Int_t           gaugeState2;
	Double_t        pressure2;

	// Set branch addresses.
	tree->SetBranchAddress("uTime",&uTime);
	tree->SetBranchAddress("switchState0",&switchState0);
	tree->SetBranchAddress("switchState1",&switchState1);
	tree->SetBranchAddress("switchState2",&switchState2);
	tree->SetBranchAddress("switchState3",&switchState3);
	tree->SetBranchAddress("switchState4",&switchState4);
	tree->SetBranchAddress("gaugeState0",&gaugeState0);
	tree->SetBranchAddress("pressure0",&pressure0);
	tree->SetBranchAddress("gaugeState1",&gaugeState1);
	tree->SetBranchAddress("pressure1",&pressure1);
	tree->SetBranchAddress("gaugeState2",&gaugeState2);
	tree->SetBranchAddress("pressure2",&pressure2);

	Long64_t nentries = tree->GetEntries();

	tree->GetEntry(0);
	uTimeOffset = uTime;

	TGraph * grPressure0 = new TGraph();
	int nPoints = 0;

	Long64_t nbytes = 0;
	for (Long64_t i=0; i<nentries;i++) {
		nbytes += tree->GetEntry(i);

		grPressure0->SetPoint(nPoints, uTime - uTimeOffset, pressure0);

		nPoints++;
	}

	grPressure0->Draw("AL");
}
