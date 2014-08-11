#define TempVsTime_cxx
// The class definition in TempVsTime.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("TempVsTime.C")
// Root > T->Process("TempVsTime.C","some options")
// Root > T->Process("TempVsTime.C+")
//

#include <iostream>

#include "TempVsTime.h"

#include <TH2.h>
#include <TStyle.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TF1.h>
#include <TLatex.h>

void TempVsTime::pushPoint(TGraph* gr, Double_t x, Double_t y)
{
  int nPoints = gr->GetN();
  gr->Set(nPoints+1);
  gr->SetPoint(nPoints, x, y);
}

void TempVsTime::pushPoint(TGraphErrors* gr, Double_t x, Double_t y, Double_t ex, Double_t ey)
{
  int nPoints = gr->GetN();
  gr->Set(nPoints+1);
  gr->SetPoint(nPoints, x, y);
  gr->SetPointError(nPoints, ex, ey);
}

void TempVsTime::checkTemperature(Double_t temp)
{
  if (temp<Tmin) Tmin = temp;
  if (temp>Tmax) Tmax = temp;
}

void TempVsTime::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  TString option = GetOption();
  
  Tmin =  1000;
  Tmax = -1000; 
  
  for (int i=1;i<=4;++i) {
    grTTop[i] = new TGraph();
    grTTop[i]->SetName(Form("top_%d", i));
    grTTop[i]->SetTitle(Form("top_%d", i));
    grTBottom[i] = new TGraph();
    grTBottom[i]->SetName(Form("bottom_%d", i));
    grTBottom[i]->SetTitle(Form("bottom_%d", i));
  }
  
  grCurrent = new TGraph();
  grVoltage = new TGraph();
  grBath = new TGraph();
  
  nCalEntries = 0;
  for (int i=0;i<8;++i) {
    cal[i] = 0;
  }

}

void TempVsTime::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  TString option = GetOption();
}

Bool_t TempVsTime::Process(Long64_t entry)
{
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // It can be passed to either TempVsTime::GetEntry() or TBranch::GetEntry()
  // to read either all or the required parts of the data. When processing
  // keyed objects with PROOF, the object is already loaded and is available
  // via the fObject pointer.
  //
  // This function should contain the "body" of the analysis. It can contain
  // simple or elaborate selection criteria, run algorithms on the data
  // of the event and typically fill histograms.
  //
  // The processing can be stopped by calling Abort().
  //
  // Use fStatus to set the return value of TTree::Process().
  //
  // The return value is currently not used.
  
  GetEntry(entry);

  static bool firstEntry = true;
  if (firstEntry) {
    minUTime = uTime;
    lastCurrent = current1;
    firstEntry = false;
  }
  
  uTime -= minUTime;
  maxUTime = uTime;

  temperature0 -= cal[0];
  temperature1 -= cal[1];
  temperature2 -= cal[2];
  temperature3 -= cal[3];
  temperature4 -= cal[4];
  temperature5 -= cal[5];
  temperature6 -= cal[6];
  temperature7 -= cal[7];

  checkTemperature(temperature0);
  checkTemperature(temperature1);
  checkTemperature(temperature2);
  checkTemperature(temperature3);
  checkTemperature(temperature4);
  checkTemperature(temperature5);
  checkTemperature(temperature6);
  checkTemperature(temperature7);

  TBottom1 = temperature0;
  TBottom2 = temperature1;
  TBottom3 = temperature2;
  TBottom4 = temperature3;
  
  TTop1 = temperature4;
  TTop2 = temperature5;
  TTop3 = temperature6;
  TTop4 = temperature7;

  pushPoint(grTTop[1], uTime, TTop1);
  pushPoint(grTTop[2], uTime, TTop2);
  pushPoint(grTTop[3], uTime, TTop3);
  pushPoint(grTTop[4], uTime, TTop4);

  pushPoint(grTBottom[1], uTime, TBottom1);
  pushPoint(grTBottom[2], uTime, TBottom2);
  pushPoint(grTBottom[3], uTime, TBottom3);
  pushPoint(grTBottom[4], uTime, TBottom4);

  pushPoint(grCurrent, uTime, current1);
  pushPoint(grVoltage, uTime, voltage1);
  if (current1!=lastCurrent) {
    std::cout << "current changed to " << current1 << " A (" << voltage1 << " V) @ " << uTime << std::endl;
  }
  lastCurrent = current1;

  pushPoint(grBath, uTime, bathTemperature);
  
  return kTRUE;
}

void TempVsTime::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.
  
}

void TempVsTime::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.
  
  TCanvas *c;
  TH1F* frame;
  
  c = new TCanvas("c1", "c1", 700, 500);
  frame = c->DrawFrame(0, -0.2,
                       maxUTime, 0.8);
  frame->GetXaxis()->SetTitle("Time [s]");
  frame->GetYaxis()->SetTitle("Current [A]");

  grCurrent->Draw("L");
  
  c->Print("CurrentVsTime.png");

  c = new TCanvas("c2", "c2", 700, 500);
  frame = c->DrawFrame(0, -0.2,
                       maxUTime, 8.2);
  frame->GetXaxis()->SetTitle("Time [s]");
  frame->GetYaxis()->SetTitle("Voltage [V]");

  grVoltage->Draw("L");
  
  c->Print("VoltageVsTime.png");
  
  c = new TCanvas("c3", "c3", 700, 500);
  frame = c->DrawFrame(0, 0.0,
                       maxUTime, 20.0);
  frame->GetXaxis()->SetTitle("Time [s]");
  frame->GetYaxis()->SetTitle("Bath [deg C]");

  grBath->Draw("L");
  
  c->Print("BathVsTime.png");

  c = new TCanvas("c4", "c4", 700, 500);
  
  frame = c->DrawFrame(0, Tmin-0.2*(Tmax-Tmin),
                       maxUTime, Tmax+0.2*(Tmax-Tmin));
  frame->GetXaxis()->SetTitle("Time [s]");
  frame->GetYaxis()->SetTitle("Temperature [K]");
  
  grTTop[4]->Draw("L");
  grTTop[3]->Draw("L");
  grTTop[2]->Draw("L");
  grTTop[1]->Draw("L");
  
  grTBottom[1]->Draw("L");
  grTBottom[2]->Draw("L");
  grTBottom[3]->Draw("L");
  grTBottom[4]->Draw("L");

  c->Print("TempVsTime.png");
}
