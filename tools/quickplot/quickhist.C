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
#include <vector>

#include <TGraph.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TH1.h>


using namespace std;

int main( int argc, char **argv ) {

  cout << endl;
  cout << " ** QuickHist 0.0.1 beta" << endl;
  cout << " ** If you want colors, comment out the gStyle option and recompile." << endl;
  cout << " ** Exit with ctrl-c" << endl;
  cout << endl;
    

  TApplication theApp("App", &argc, argv);
  gROOT->Reset();
  gROOT->SetStyle( "Plain" );
//  gStyle->SetFillColor( 0 );

   
  double buffer;
  vector<double> numbersVector;



  while( ! cin.eof() ) {
    cin >> buffer;
    if( cin.eof() ) break; // break if eof
    numbersVector.push_back( buffer );
  }


  double lowerLimit = 0.;
  double upperLimit = 0.;
  double lowerLimitBuf = numbersVector[0]; 
  double upperLimitBuf = numbersVector[0];

  for( unsigned int i = 0; i < numbersVector.size(); i++ ) {
    if( numbersVector[i] < lowerLimitBuf ) lowerLimitBuf = numbersVector[i];
    if( numbersVector[i] > upperLimitBuf ) upperLimitBuf = numbersVector[i];
  }
  
  // broadening the limits a little bit
  upperLimit = upperLimitBuf + .1 * ( upperLimitBuf - lowerLimitBuf );
  lowerLimit = lowerLimitBuf - .1 * ( upperLimitBuf - lowerLimitBuf );

  TCanvas* canvas = new TCanvas( "QuickHist", "QuickHist" );
  canvas->Draw();

  TH1D* histogram = new TH1D( "QuickHist", "QuickHist", 1000, lowerLimit, upperLimit );
  for( unsigned int i = 0; i < numbersVector.size(); i++ ) histogram->Fill( numbersVector[i] );
  histogram->Draw();


  theApp.Run( kTRUE );

}
