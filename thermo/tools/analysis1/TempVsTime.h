//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jan 17 09:37:30 2014 by ROOT version 5.34/09
// from TTree thermoDAQ/thermoDAQ
// found on file: 20140116-1.root
//////////////////////////////////////////////////////////

#ifndef TempVsTime_h
#define TempVsTime_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TGraph.h>
#include <TGraphErrors.h>

// Header file for the classes stored in the TTree if any.
#include <TDatime.h>

// Fixed size dimensions of array or collections stored in the TTree if any.

class TempVsTime : public TSelector {
public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  
  // Declaration of leaf types
  UInt_t          uTime;
  //TDatime         *datime;
  UInt_t          fDatime;
  Float_t         bathTemperature;
  Float_t         workingTemperature;
  Int_t           circulator;
  Int_t           channelActive0;
  Float_t         temperature0;
  Int_t           channelActive1;
  Float_t         temperature1;
  Int_t           channelActive2;
  Float_t         temperature2;
  Int_t           channelActive3;
  Float_t         temperature3;
  Int_t           channelActive4;
  Float_t         temperature4;
  Int_t           channelActive5;
  Float_t         temperature5;
  Int_t           channelActive6;
  Float_t         temperature6;
  Int_t           channelActive7;
  Float_t         temperature7;
  Int_t           channelActive8;
  Float_t         temperature8;
  Int_t           channelActive9;
  Float_t         temperature9;
  Int_t           gaugeStatus1;
  Float_t         gaugePressure1;
  Int_t           gaugeStatus2;
  Float_t         gaugePressure2;
  Int_t           powerRemote;
  Int_t           powerOn;
  Int_t           cv1;
  Int_t           cv2;
  Float_t         setVoltage1;
  Float_t         setCurrent1;
  Float_t         setVoltage2;
  Float_t         setCurrent2;
  Float_t         voltage1;
  Float_t         current1;
  Float_t         voltage2;
  Float_t         current2;
  
  // List of branches
  TBranch        *b_uTime;   //!
  TBranch        *b_datime_fDatime;   //!
  TBranch        *b_bathTemperature;   //!
  TBranch        *b_workingTemperature;   //!
  TBranch        *b_circulator;   //!
  TBranch        *b_channelActive0;   //!
  TBranch        *b_temperature0;   //!
  TBranch        *b_channelActive1;   //!
  TBranch        *b_temperature1;   //!
  TBranch        *b_channelActive2;   //!
  TBranch        *b_temperature2;   //!
  TBranch        *b_channelActive3;   //!
  TBranch        *b_temperature3;   //!
  TBranch        *b_channelActive4;   //!
  TBranch        *b_temperature4;   //!
  TBranch        *b_channelActive5;   //!
  TBranch        *b_temperature5;   //!
  TBranch        *b_channelActive6;   //!
  TBranch        *b_temperature6;   //!
  TBranch        *b_channelActive7;   //!
  TBranch        *b_temperature7;   //!
  TBranch        *b_channelActive8;   //!
  TBranch        *b_temperature8;   //!
  TBranch        *b_channelActive9;   //!
  TBranch        *b_temperature9;   //!
  TBranch        *b_gaugeStatus1;   //!
  TBranch        *b_gaugePressure1;   //!
  TBranch        *b_gaugeStatus2;   //!
  TBranch        *b_gaugePressure2;   //!
  TBranch        *b_powerRemote;   //!
  TBranch        *b_powerOn;   //!
  TBranch        *b_cv1;   //!
  TBranch        *b_cv2;   //!
  TBranch        *b_setVoltage1;   //!
  TBranch        *b_setCurrent1;   //!
  TBranch        *b_setVoltage2;   //!
  TBranch        *b_setCurrent2;   //!
  TBranch        *b_voltage1;   //!
  TBranch        *b_current1;   //!
  TBranch        *b_voltage2;   //!
  TBranch        *b_current2;   //!
  
  TempVsTime(TTree * /*tree*/ =0) : fChain(0) { }
  virtual ~TempVsTime() { }
  virtual Int_t   Version() const { return 2; }
  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual Bool_t  Notify();
  virtual Bool_t  Process(Long64_t entry);
  virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate();
  virtual void    Terminate();
  
  void pushPoint(TGraph* gr,
                 Double_t x, Double_t y);
  void pushPoint(TGraphErrors* gr,
                 Double_t x, Double_t y,
                 Double_t ex, Double_t ey);
  void checkTemperature(Double_t temp);
  
  UInt_t minUTime;
  UInt_t maxUTime;
  int nCalEntries;
  Float_t cal[8];

  Float_t Tmin, Tmax;  
  Float_t TTop1, TTop2, TTop3, TTop4;
  Float_t TBottom1, TBottom2, TBottom3, TBottom4;
  
  TGraph* grTTop[5];
  TGraph* grTBottom[5];
  
  Float_t lastCurrent;
  TGraph* grCurrent;
  TGraph* grVoltage;
  TGraph* grBath;
  
  ClassDef(TempVsTime,0);
};

#endif

#ifdef TempVsTime_cxx
void TempVsTime::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).
  
  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fChain->SetMakeClass(1);
  
  fChain->SetBranchAddress("uTime", &uTime, &b_uTime);
  fChain->SetBranchAddress("fDatime", &fDatime, &b_datime_fDatime);
  fChain->SetBranchAddress("bathTemperature", &bathTemperature, &b_bathTemperature);
  fChain->SetBranchAddress("workingTemperature", &workingTemperature, &b_workingTemperature);
  fChain->SetBranchAddress("circulator", &circulator, &b_circulator);
  fChain->SetBranchAddress("channelActive0", &channelActive0, &b_channelActive0);
  fChain->SetBranchAddress("temperature0", &temperature0, &b_temperature0);
  fChain->SetBranchAddress("channelActive1", &channelActive1, &b_channelActive1);
  fChain->SetBranchAddress("temperature1", &temperature1, &b_temperature1);
  fChain->SetBranchAddress("channelActive2", &channelActive2, &b_channelActive2);
  fChain->SetBranchAddress("temperature2", &temperature2, &b_temperature2);
  fChain->SetBranchAddress("channelActive3", &channelActive3, &b_channelActive3);
  fChain->SetBranchAddress("temperature3", &temperature3, &b_temperature3);
  fChain->SetBranchAddress("channelActive4", &channelActive4, &b_channelActive4);
  fChain->SetBranchAddress("temperature4", &temperature4, &b_temperature4);
  fChain->SetBranchAddress("channelActive5", &channelActive5, &b_channelActive5);
  fChain->SetBranchAddress("temperature5", &temperature5, &b_temperature5);
  fChain->SetBranchAddress("channelActive6", &channelActive6, &b_channelActive6);
  fChain->SetBranchAddress("temperature6", &temperature6, &b_temperature6);
  fChain->SetBranchAddress("channelActive7", &channelActive7, &b_channelActive7);
  fChain->SetBranchAddress("temperature7", &temperature7, &b_temperature7);
  fChain->SetBranchAddress("channelActive8", &channelActive8, &b_channelActive8);
  fChain->SetBranchAddress("temperature8", &temperature8, &b_temperature8);
  fChain->SetBranchAddress("channelActive9", &channelActive9, &b_channelActive9);
  fChain->SetBranchAddress("temperature9", &temperature9, &b_temperature9);
  fChain->SetBranchAddress("gaugeStatus1", &gaugeStatus1, &b_gaugeStatus1);
  fChain->SetBranchAddress("gaugePressure1", &gaugePressure1, &b_gaugePressure1);
  fChain->SetBranchAddress("gaugeStatus2", &gaugeStatus2, &b_gaugeStatus2);
  fChain->SetBranchAddress("gaugePressure2", &gaugePressure2, &b_gaugePressure2);
  fChain->SetBranchAddress("powerRemote", &powerRemote, &b_powerRemote);
  fChain->SetBranchAddress("powerOn", &powerOn, &b_powerOn);
  fChain->SetBranchAddress("cv1", &cv1, &b_cv1);
  fChain->SetBranchAddress("cv2", &cv2, &b_cv2);
  fChain->SetBranchAddress("setVoltage1", &setVoltage1, &b_setVoltage1);
  fChain->SetBranchAddress("setCurrent1", &setCurrent1, &b_setCurrent1);
  fChain->SetBranchAddress("setVoltage2", &setVoltage2, &b_setVoltage2);
  fChain->SetBranchAddress("setCurrent2", &setCurrent2, &b_setCurrent2);
  fChain->SetBranchAddress("voltage1", &voltage1, &b_voltage1);
  fChain->SetBranchAddress("current1", &current1, &b_current1);
  fChain->SetBranchAddress("voltage2", &voltage2, &b_voltage2);
  fChain->SetBranchAddress("current2", &current2, &b_current2);
}

Bool_t TempVsTime::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.
  
  return kTRUE;
}

#endif // #ifdef TempVsTime_cxx
