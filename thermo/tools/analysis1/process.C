#define process_cxx

#include <TFile.h>
#include <TTree.h>

void process()
{
  TFile *file = TFile::Open("20140728-1.root");

  TTree *T = (TTree*)file->Get("thermoDAQ");
  
  T->Process("TempVsTime.C+");
  //T->Process("Calibration.C+", "11500 11550  33000 33050  54700 54750");
  //T->Process("CalTempVsTime.C+");
  
  //T->Process("Analysis.C+", "15 7 0 22300 22350");
  //T->Process("Analysis.C+", "15 15 1 43900 43950");
  //T->Process("Analysis.C+", "15 15 2 65400 65450");
}
