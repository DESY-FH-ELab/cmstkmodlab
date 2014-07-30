#define CalTempVsTime_cxx
// The class definition in CalTempVsTime.h has been generated automatically
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
// Root > T->Process("CalTempVsTime.C")
// Root > T->Process("CalTempVsTime.C","some options")
// Root > T->Process("CalTempVsTime.C+")
//

#include <iostream>
#include <fstream>

#include "CalTempVsTime.h"

#include <TH2.h>
#include <TStyle.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TF1.h>
#include <TLatex.h>

CalibrationSet::CalibrationSet()
{
  minTime_ = 0;
  maxTime_ = 2147483647;
  for (int i=0;i<8;++i) {
    cal_[i] = 0.;
  }
}

void CalibrationSet::read(std::ifstream& ifile)
{
  ifile >> minTime_;
  ifile >> maxTime_;
  
  for (int i=0;i<8;++i) { 
    ifile >> cal_[i];
  }
  
  std::cout << minTime_ << " " << maxTime_ << std::endl;
}

void CalTempVsTime::pushPoint(TGraph* gr, Double_t x, Double_t y)
{
  int nPoints = gr->GetN();
  gr->Set(nPoints+1);
  gr->SetPoint(nPoints, x, y);
}

void CalTempVsTime::pushPoint(TGraphErrors* gr, Double_t x, Double_t y, Double_t ex, Double_t ey)
{
  int nPoints = gr->GetN();
  gr->Set(nPoints+1);
  gr->SetPoint(nPoints, x, y);
  gr->SetPointError(nPoints, ex, ey);
}

void CalTempVsTime::checkTemperature(Double_t temp)
{
  if (temp<Tmin) Tmin = temp;
  if (temp>Tmax) Tmax = temp;
}

void CalTempVsTime::Begin(TTree * /*tree*/)
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
  
  std::ifstream ifile("calibration.txt");
  int calcount;
  ifile >> calcount;
  for (int i=0;i<calcount;++i) {
    CalibrationSet * calset = new CalibrationSet();
    calset->read(ifile);
    calibrationSets_.Add(calset);
  }
}

void CalTempVsTime::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  TString option = GetOption();
}

Bool_t CalTempVsTime::Process(Long64_t entry)
{
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // It can be passed to either CalTempVsTime::GetEntry() or TBranch::GetEntry()
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
    firstEntry = false;
  }
  
  uTime -= minUTime;
  maxUTime = uTime;

  CalibrationSet * calset = 0;
  
  for (int idx = 0;idx<calibrationSets_.GetSize();++idx) {
    CalibrationSet *set = (CalibrationSet *)calibrationSets_.At(idx);
    if (uTime>=set->getMinTime() && uTime<set->getMaxTime()) {
      calset = set;
      break;
    }
  }
  
  if (calset!=0) {
    temperature0 -= calset->getCal(0);
    temperature1 -= calset->getCal(1);
    temperature2 -= calset->getCal(2);
    temperature3 -= calset->getCal(3);
    temperature4 -= calset->getCal(4);
    temperature5 -= calset->getCal(5);
    temperature6 -= calset->getCal(6);
    temperature7 -= calset->getCal(7);
  }
  
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
  
  return kTRUE;
}

void CalTempVsTime::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.
  
}

void CalTempVsTime::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.
  
  TCanvas *c;
  
  c = new TCanvas("c4", "c4", 700, 500);
  
  std::cout << Tmin << " " << Tmax << std::endl;
  
  TH1F* frame = c->DrawFrame(0, Tmin-0.2*(Tmax-Tmin),
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

  c->Print("CalTempVsTime.png");
}
