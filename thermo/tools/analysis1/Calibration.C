#define Calibration_cxx
// The class definition in Calibration.h has been generated automatically
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
// Root > T->Process("Calibration.C")
// Root > T->Process("Calibration.C","some options")
// Root > T->Process("Calibration.C+")
//

#include <iostream>
#include <fstream>
#include <sstream>

#include "Calibration.h"
#include <TH2.h>
#include <TStyle.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TF1.h>
#include <TLatex.h>

CalibrationRange::CalibrationRange(UInt_t minTime, UInt_t maxTime)
  :minTime_(minTime),
   maxTime_(maxTime)
{
  for (int i=0;i<8;++i) {
    nCalEntries_[i] = 0;
    cal_[i] = 0.;
  }
}

void CalibrationRange::addCalEntry(UInt_t channel, Float_t value)
{
  cal_[channel] += value;
  nCalEntries_[channel]++;
}

void CalibrationRange::dump(std::ofstream& ofile, UInt_t minTime, UInt_t maxTime)
{
  ofile << minTime << std::endl;
  ofile << maxTime << std::endl;
  
  std::cout << "  // [" << minTime << ";" << maxTime << "]" << std::endl;
  std::cout << "  // minTime = " << minTime_ << std::endl;
  std::cout << "  // maxTime = " << maxTime_ << std::endl;

  for (int i=0;i<8;++i) { 
    if (nCalEntries_[i]>0) {
      std::cout << "  cal[" << i << "] = " << cal_[i]/nCalEntries_[i] << ";" << std::endl;
      ofile << cal_[i]/nCalEntries_[i] << std::endl;
    }
  }
  
  std::cout << std::endl;
}

void Calibration::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  TString options = GetOption();

  std::cout << options << std::endl;
  std::istringstream iss(options.Data());
  UInt_t min, max;
  while (iss >> min >> max) {
    std::cout << min << " " << max << std::endl;
    calibrationRanges_.Add(new CalibrationRange(min, max));
  }
}

void Calibration::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  TString option = GetOption();
}

Bool_t Calibration::Process(Long64_t entry)
{
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // It can be passed to either Calibration::GetEntry() or TBranch::GetEntry()
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

  TIter next(&calibrationRanges_);
  CalibrationRange *range;
  while ((range = (CalibrationRange *)next())) {
    if (range->getMinTime()<=uTime && range->getMaxTime()>=uTime) {
      range->addCalEntry(0, temperature0);
      range->addCalEntry(1, temperature1);
      range->addCalEntry(2, temperature2);
      range->addCalEntry(3, temperature3);
      range->addCalEntry(4, temperature4);
      range->addCalEntry(5, temperature5);
      range->addCalEntry(6, temperature6);
      range->addCalEntry(7, temperature7);
    }
  }
  
  return kTRUE;
}

void Calibration::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.
  
}

void Calibration::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.
  
  std::ofstream ofile("calibration.txt");
  ofile << calibrationRanges_.GetSize() << std::endl;
  
  for (int idx = 0;idx<calibrationRanges_.GetSize();++idx) {
    CalibrationRange *range = (CalibrationRange *)calibrationRanges_.At(idx);
    if (idx==0) {
      if (calibrationRanges_.GetSize()==1) {
        range->dump(ofile, 0, -1);
      } else {
        CalibrationRange *orange = (CalibrationRange *)calibrationRanges_.At(idx+1);
        range->dump(ofile, 0, orange->getMinTime());
      }
    } else if (idx==calibrationRanges_.GetSize()-1 && calibrationRanges_.GetSize()>1) {
        range->dump(ofile, range->getMinTime(), 2147483647);
    } else {
        CalibrationRange *orange = (CalibrationRange *)calibrationRanges_.At(idx+1);
        range->dump(ofile, range->getMinTime(), orange->getMinTime());
    }
  }
}
