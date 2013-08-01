#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include <TStyle.h>
#include"TLegend.h"

#include <fstream>
#include <iostream>

const TString* inpath = new TString("measurements/20130730/");
const TString* outpath = new TString("./");

void thermo(){
	
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	
	std::ofstream outputfile;
    	outputfile.open ("output.txt");
	
	TCanvas* c1 = new TCanvas("c1","c1",700,500);
	c1->SetGrid();
	
	
	
  // open ROOT file and access the nTuple
	TFile* inputFile(0);
	inputFile = TFile::Open(inpath->Copy().Append("20130730-1.root"));

	TTree* nTuple(0);
	TString* folderName = new TString("");
	TString* treeName = new TString("thermoDAQ");
	inputFile->GetObject(folderName->Copy().Append(*treeName),nTuple);
	if(!nTuple)std::cout<<"\n\tTTree with nTuple not found in file!\n";
  
  // access branches of interest
	TBranch* b_temp0 = nTuple->GetBranch("temperature0");
	TBranch* b_temp1 = nTuple->GetBranch("temperature1");
	TBranch* b_temp2 = nTuple->GetBranch("temperature2");
	TBranch* b_temp3 = nTuple->GetBranch("temperature3");
	TBranch* b_temp4 = nTuple->GetBranch("temperature4");
	TBranch* b_temp5 = nTuple->GetBranch("temperature5");
	TBranch* b_temp6 = nTuple->GetBranch("temperature6");
	TBranch* b_temp7 = nTuple->GetBranch("temperature7");
	TBranch* b_temp8 = nTuple->GetBranch("temperature8");
	TBranch* b_temp9 = nTuple->GetBranch("temperature9");
  
  // book histrograms
	TH1* h_T0 = new TH1F("T0","T0",600,16,22);
	TH1* h_T1 = new TH1F("T1","T1",600,16,22);
	TH1* h_T2 = new TH1F("T2","T2",600,16,22);
	TH1* h_T3 = new TH1F("T3","T3",600,16,22);
	TH1* h_T4 = new TH1F("T4","T4",600,16,22);
	TH1* h_T5 = new TH1F("T5","T5",600,16,22);
	TH1* h_T6 = new TH1F("T6","T6",600,16,22);
	TH1* h_T7 = new TH1F("T7","T7",600,16,22);
	TH1* h_T8 = new TH1F("T8","T8",600,16,22);
	TH1* h_T9 = new TH1F("T9","T9",600,16,22);
	
	TMultiGraph *mg = new TMultiGraph("T_all","T_all");
	TGraph* g0 = new TGraph();
	TGraph* g1 = new TGraph();
	TGraph* g2 = new TGraph();
	TGraph* g3 = new TGraph();
	TGraph* g4 = new TGraph();
	TGraph* g5 = new TGraph();
	TGraph* g6 = new TGraph();
	TGraph* g7 = new TGraph();
	TGraph* g8 = new TGraph();
	TGraph* g9 = new TGraph();
	
	TMultiGraph *mg1 = new TMultiGraph("DeltaT","DeltaT");
	TGraph* g10 = new TGraph();

  // loop over entries of nTuple and fill histograms
  const unsigned int nEntry(nTuple->GetEntries());
  for( double iEntry = 0; iEntry < nEntry; ++iEntry){
    
    float temperature0(0);
    b_temp0->SetAddress(&temperature0);
    b_temp0->GetEntry(iEntry);
    
    float temperature1(0);
    b_temp1->SetAddress(&temperature1);
    b_temp1->GetEntry(iEntry);
    
    float temperature2(0);
    b_temp2->SetAddress(&temperature2);
    b_temp2->GetEntry(iEntry);
    
    float temperature3(0);
    b_temp3->SetAddress(&temperature3);
    b_temp3->GetEntry(iEntry);

    float temperature4(0);
    b_temp4->SetAddress(&temperature4);
    b_temp4->GetEntry(iEntry);
    
    float temperature5(0);
    b_temp5->SetAddress(&temperature5);
    b_temp5->GetEntry(iEntry);

    float temperature6(0);
    b_temp6->SetAddress(&temperature6);
    b_temp6->GetEntry(iEntry);
    
    float temperature7(0);
    b_temp7->SetAddress(&temperature7);
    b_temp7->GetEntry(iEntry);

    float temperature8(0);
    b_temp8->SetAddress(&temperature8);
    b_temp8->GetEntry(iEntry);
    
    float temperature9(0);
    b_temp9->SetAddress(&temperature9);
    b_temp9->GetEntry(iEntry);

    
   /* std::cout << iEntry     << "    "
              << temperature0 << "    " << temperature1 << "  "
              << temperature2 << "    " << temperature3 << "  " 
              << temperature4 << "    " << temperature5 << "  "
              << temperature6 << "    " << temperature7 << "  "
              << temperature8 << "    " << temperature9 << "  "
              << std::endl;*/
    
		h_T0->Fill(temperature0); h_T0->SetFillColor(3); h_T0->SetLineColor(1); 
		h_T1->Fill(temperature1); h_T1->SetFillColor(3); h_T1->SetLineColor(1); 
		h_T2->Fill(temperature2); h_T2->SetFillColor(3); h_T2->SetLineColor(1); 
		h_T3->Fill(temperature3); h_T3->SetFillColor(3); h_T3->SetLineColor(1);
		h_T4->Fill(temperature4); h_T4->SetFillColor(3); h_T4->SetLineColor(1); 
		h_T5->Fill(temperature5); h_T5->SetFillColor(3); h_T5->SetLineColor(1);
		h_T6->Fill(temperature6); h_T6->SetFillColor(3); h_T6->SetLineColor(1);
		h_T7->Fill(temperature7); h_T7->SetFillColor(3); h_T7->SetLineColor(1); 
		h_T8->Fill(temperature8); h_T8->SetFillColor(3); h_T8->SetLineColor(1);
		h_T9->Fill(temperature9); h_T9->SetFillColor(3); h_T9->SetLineColor(1); 

		g0->SetPoint(iEntry, iEntry, temperature0); g0->SetLineColor(1); g0->SetLineWidth(3);
		g1->SetPoint(iEntry, iEntry, temperature1); g1->SetLineColor(2); g1->SetLineWidth(3);
		g2->SetPoint(iEntry, iEntry, temperature2); g2->SetLineColor(3); g2->SetLineWidth(3);
		g3->SetPoint(iEntry, iEntry, temperature3); g3->SetLineColor(4); g3->SetLineWidth(3);
		g4->SetPoint(iEntry, iEntry, temperature4); g4->SetLineColor(5); g4->SetLineWidth(3);
		g5->SetPoint(iEntry, iEntry, temperature5); g5->SetLineColor(6); g5->SetLineWidth(3);
		g6->SetPoint(iEntry, iEntry, temperature6); g6->SetLineColor(7); g6->SetLineWidth(3);
		g7->SetPoint(iEntry, iEntry, temperature7); g7->SetLineColor(8); g7->SetLineWidth(3);
		g8->SetPoint(iEntry, iEntry, temperature8); g8->SetLineColor(9); g8->SetLineWidth(3);
		g9->SetPoint(iEntry, iEntry, temperature9); g9->SetLineColor(46); g9->SetLineWidth(3);

		g10->SetPoint(iEntry, iEntry, temperature7 - temperature6); g10->SetLineColor(46); g10->SetLineWidth(3);

		outputfile
			<< iEntry       << "    "
			<< temperature0 << "    " << temperature1 << "  "
			<< temperature2 << "    " << temperature3 << "  " 
			<< temperature4 << "    " << temperature5 << "  "
			<< temperature6 << "    " << temperature7 << "  "
			<< temperature8 << "    " << temperature9 << "  "
			<< endl;
  }
  outputfile.close();
  


	std::cout << "nEntries :" << nEntry << std::endl;

	// create output file and write histograms to it
	TFile* outputFile(0);
	outputFile = new TFile("output.root","RECREATE");

	mg->Add(g0);
	mg->Add(g1);
	mg->Add(g2);
	mg->Add(g3);
	mg->Add(g4);
	mg->Add(g5);
	mg->Add(g6);
	mg->Add(g7);
	mg->Add(g8);
	mg->Add(g9);

	mg->Write();

	mg1->Add(g10);
	mg1->Write();

	h_T0->Write();
	h_T1->Write();
	h_T2->Write();
	h_T3->Write();
	h_T4->Write();
	h_T5->Write();
	h_T6->Write();
	h_T7->Write();
	h_T8->Write();
	h_T9->Write();

	outputFile->Close();

	mg->Draw("same  L");
	mg->GetYaxis()->SetTitle("Temperature [C]");
	mg->GetYaxis()->SetTitleOffset(1);
	mg->GetXaxis()->SetTitle("Entries");
	mg->GetXaxis()->SetTitleOffset(1);

	leg = new TLegend(0.91,0.1,0.99,0.9);
	leg->SetNColumns(1);
	//leg->SetTextFont(72);
	leg->SetTextSize(0.04);
	leg->SetFillColor(0);
	leg->AddEntry(g0, "T0", "l");
	leg->AddEntry(g1, "T1", "l");
	leg->AddEntry(g2, "T2", "l");
	leg->AddEntry(g3, "T3", "l");
	leg->AddEntry(g4, "T4", "l");
	leg->AddEntry(g5, "T5", "l");
	leg->AddEntry(g6, "T6", "l");
	leg->AddEntry(g7, "T7", "l");
	leg->AddEntry(g8, "T8", "l");
	leg->AddEntry(g9, "T9", "l");
	leg->Draw();

	TCanvas* c2 = new TCanvas("c2","c2",700,500);
	c2->SetGrid();

	mg1->Draw("same l");
	mg1->GetYaxis()->SetTitle("Temperature [C]");
	mg1->GetYaxis()->SetTitleOffset(1);
	mg1->GetXaxis()->SetTitle("Entries");
	mg1->GetXaxis()->SetTitleOffset(1);

	leg1 = new TLegend(0.70,0.8,0.86,0.87);
	leg1->SetNColumns(1);
	leg1->SetTextSize(0.04);
	leg1->SetFillColor(0);
	leg1->AddEntry(g10, "T7-T6", "L");
	leg1->Draw("same");

	c1->SaveAs("T_all.gif");
	c2->SaveAs("DeltaT.gif");

	}

int main(){
thermo();
}
