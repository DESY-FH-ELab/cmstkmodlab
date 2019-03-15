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


using namespace std;


int main( int argc, char **argv ) {

  cout << endl;
  cout << " ** QuickPlot 0.0.1 beta" << endl;
  cout << " ** If you want colors, comment out the gStyle option and recompile." << endl;
  cout << " ** Exit with ctrl-c" << endl;
  cout << endl;
    

  TApplication theApp("App", &argc, argv);
  gROOT->Reset();
  gROOT->SetStyle( "Plain" );
  gStyle->SetFillColor( 0 );

   
  double buffer;
  vector<double> numbersVector;



  while( ! cin.eof() ) {
    cin >> buffer;
    if( cin.eof() ) break;
    numbersVector.push_back( buffer );
  }


  const int arraySize = numbersVector.size();
  
  double indexArray[arraySize - 1];
  double numbersArray[arraySize - 1];
  
  for( int i = 0; i < arraySize - 1; i++ ) {
    indexArray[i] = i + 1;
    numbersArray[i] = numbersVector[i];
  }

  TCanvas* canvas = new TCanvas( "QuickPlot", "QuickPlot" );
  canvas->Draw();

  TGraph* graph = new TGraph( arraySize - 1, indexArray, numbersArray );
  graph->SetTitle( "QuickPlot graph" );
  graph->SetMarkerStyle( 28 );
  graph->SetMarkerSize( 0.3 );
  graph->Draw( "AP" );
  
  theApp.Run(kTRUE);

}
