/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

{
//////////////////////////////////////////////////////////
//   This file has been automatically generated 
//     (Mon Jul 17 14:57:51 2017 by ROOT version6.10/02)
//   from TTree pumpstation/pumpstation
//   found on file: ../SavingValve_1.0.root
//////////////////////////////////////////////////////////


//Reset ROOT and connect tree file
   gROOT->Reset();
   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../SavingValve_1.0.root");
   if (!f) {
      f = new TFile("../SavingValve_1.0.root");
   }
    f->GetObject("pumpstation",tree);

//Declaration of leaves types
   UInt_t          uTime;
   UInt_t          fDatime;
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
   pumpstation->SetBranchAddress("uTime",&uTime);
   pumpstation->SetBranchAddress("datime",&datime);
   pumpstation->SetBranchAddress("fDatime",&fDatime);
   pumpstation->SetBranchAddress("switchState0",&switchState0);
   pumpstation->SetBranchAddress("switchState1",&switchState1);
   pumpstation->SetBranchAddress("switchState2",&switchState2);
   pumpstation->SetBranchAddress("switchState3",&switchState3);
   pumpstation->SetBranchAddress("switchState4",&switchState4);
   pumpstation->SetBranchAddress("gaugeState0",&gaugeState0);
   pumpstation->SetBranchAddress("pressure0",&pressure0);
   pumpstation->SetBranchAddress("gaugeState1",&gaugeState1);
   pumpstation->SetBranchAddress("pressure1",&pressure1);
   pumpstation->SetBranchAddress("gaugeState2",&gaugeState2);
   pumpstation->SetBranchAddress("pressure2",&pressure2);

//     This is the loop skeleton
//       To read only selected branches, Insert statements like:
// pumpstation->SetBranchStatus("*",0);  // disable all branches
// TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

   Long64_t nentries = pumpstation->GetEntries();

   Long64_t nbytes = 0;
//   for (Long64_t i=0; i<nentries;i++) {
//      nbytes += pumpstation->GetEntry(i);
//   }
}
