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

#include <iostream>
#include <limits>

#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TH1.h>
#include <TDatime.h>

void plotPressure(const char *filename = 0)
{
  if (filename==0) return;

  TFile * ifile = new TFile(filename);
  TTree * tree = (TTree*)ifile->Get("thermoDAQ");
  // tree->Print();

  unsigned int uTime;
  float p1, p2;

  tree->SetBranchAddress("uTime", &uTime);
  tree->SetBranchAddress("gaugePressure1", &p1);
  tree->SetBranchAddress("gaugePressure2", &p2);

  TGraph * grP1 = new TGraph();
  grP1->SetLineWidth(2);
  grP1->SetLineColor(kBlue-5);
  TGraph * grP2 = new TGraph();
  grP2->SetLineWidth(2);
  grP2->SetLineColor(kGreen-9);
  unsigned int minT, maxT = 0;
  TDatime dtMin, dtMax;

  for (int e=0;e<tree->GetEntries();++e) {
    tree->GetEntry(e);

    if (e==0) minT = uTime;
    maxT = TMath::Max(uTime, maxT);

    grP1->SetPoint(e, uTime-minT, p1);
    grP2->SetPoint(e, uTime-minT, p2);
  }

  gStyle->SetTimeOffset(minT);

   TCanvas * c = new TCanvas("c", "c", 700, 500);
   c->SetBottomMargin(0.125);
   c->SetLeftMargin(0.125);
   c->SetRightMargin(0.05);
   c->SetTopMargin(0.05);
   c->SetLogy();
   TH1F * frame = c->DrawFrame(-600, 1e-6, maxT-minT+600, 5000);
   frame->GetXaxis()->SetTimeDisplay(1);
   frame->GetXaxis()->SetNdivisions(505);
   frame->GetXaxis()->SetTimeFormat("#splitline{%d/%m/%y}{%H:%M:%S}");
   frame->GetXaxis()->SetLabelOffset(0.04);

   frame->GetYaxis()->SetTitle("pressure [mbar]");
   frame->GetYaxis()->SetTitleOffset(1.3);
   frame->GetYaxis()->CenterTitle();

   grP1->Draw("L");
   //grP2->Draw("L");
}
