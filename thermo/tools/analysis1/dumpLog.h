//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Sep 17 19:33:03 2014 by ROOT version 5.34/19
// from TTree thermoLog/thermoLog
// found on file: 20140903-1.root
//////////////////////////////////////////////////////////

#ifndef dumpLog_h
#define dumpLog_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <TDatime.h>
#include <string>

// Fixed size dimensions of array or collections stored in the TTree if any.

class dumpLog {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   UInt_t          uTime;
   TDatime         datime;
   UInt_t          fDatime;
   string          *message;

   // List of branches
   TBranch        *b_uTime;   //!
   TBranch        *b_datime;   //!
   TBranch        *b_datime_fDatime;   //!
   TBranch        *b_message;   //!

   dumpLog(std::string filename, TTree *tree=0);
   virtual ~dumpLog();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef dumpLog_cxx
dumpLog::dumpLog(std::string filename, TTree *tree) : fChain(0) 
{
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(filename.c_str());
      if (!f || !f->IsOpen()) {
         f = new TFile(filename.c_str());
      }
      f->GetObject("thermoLog",tree);

   }
   Init(tree);
}

dumpLog::~dumpLog()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t dumpLog::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t dumpLog::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void dumpLog::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   message = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("uTime", &uTime, &b_uTime);
   fChain->SetBranchAddress("datime", &datime, &b_datime);
   fChain->SetBranchAddress("fDatime", &fDatime, &b_datime_fDatime);
   fChain->SetBranchAddress("message", &message, &b_message);
   Notify();
}

Bool_t dumpLog::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void dumpLog::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t dumpLog::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef dumpLog_cxx
