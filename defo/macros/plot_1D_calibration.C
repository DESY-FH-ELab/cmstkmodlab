#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>

#include "TCanvas.h"
#include "TF1.h"
#include <TH1F.h>
#include "TGraph.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TParameter.h"
#include "TMath.h"
#include "TString.h"


void plot(const std::string& inputFileName)
{
  gStyle->SetPalette(1);
  
  std::ifstream ifile(inputFileName.data());
  if(!(ifile.is_open())){
    std::cerr<<"ERROR! Requested file not found, file name: "<<inputFileName
             <<"\n...break\n"<<std::endl;
    exit(321);
  }
   
  TGraph *gr1 = new TGraph();
  gr1->SetMarkerStyle(25);
  
  int nPoints1 = 0;
  double x, y, px, py, dx, dy, dxy, corrx, corry;
  int i, ix, iy;
  int a1, a2;
  int maxIx(-9999);
  int minIx(9999);
  int maxIy(-9999);
  int minIy(9999);
  float summinX(0.);
  float summaxX(0.);
  float summinY(0.);
  float summaxY(0.);

  TString dummy;
  dummy.ReadLine(ifile);

  std::map<int,double> minX, maxX, minY, maxY;
 
  // Gehe ueber alle Punkte und fuelle sie in 1D-Graphen laengs y
  while (ifile >> ix >> iy >> x >> y >> px >> py >> dx >> a1 >> dy >> a2 >> dxy >> corrx >> corry) {

    if (minX.find(iy)==minX.end()) minX[iy] = x;
    if (maxX.find(iy)==maxX.end()) maxX[iy] = x;
    if (minY.find(ix)==minY.end()) minY[ix] = y;
    if (maxY.find(ix)==maxX.end()) maxY[ix] = y;

    minX[iy] = std::min(x, minX[iy]);
    maxX[iy] = std::max(x, maxX[iy]);
    minY[ix] = std::min(y, minY[ix]);
    maxY[ix] = std::max(y, maxY[ix]);
    
    gr1->SetPoint(nPoints1++, x, y);
    
    if(ix > maxIx) maxIx = ix;
    if(ix < minIx) minIx = ix;
    if(iy > maxIy) maxIy = iy;
    if(iy < minIy) minIy = iy;
  }
  ifile.close();
  
    
   TH1F * dX = new TH1F("dX", "dX", 4*(240-120), 120, 240);
  for (std::map<int,double>::iterator it = minX.begin();
       it != minX.end();
       ++it) {
    //std::cout << it->first << ":" << it->second << " - " << maxX[it->first] << std::endl;
    dX->Fill(maxX[it->first]-it->second);
  }

  TH1F * dY = new TH1F("dY", "dY", 4*(290-170), 170, 290);
  for (std::map<int,double>::iterator it = minY.begin();
       it != minY.end();
       ++it) {
    //std::cout << it->first << ":" << it->second << " - " << maxY[it->first] << std::endl;
    dY->Fill(maxY[it->first]-it->second);
  }

  double AnzIx = maxIx-minIx;
  double AnzIy = maxIy-minIy;
  //cout<<"AnzIx="<<AnzIx<<endl;
  //cout<<"AnzIy="<<AnzIy<<endl;
  //cout<<"minIy="<<minIy<<endl;
  //cout<<"maxIy="<<maxIy<<endl;
  
  for(i=minIy; i<=maxIy; i++){
      summinX=minX[i]+summinX;
  }
  //cout<<"summinX="<<summinX/(AnzIy+1)<<endl;
  
  for(i=minIy; i<=maxIy; i++){
      summaxX=maxX[i]+summaxX;
  }
  //cout<<"summaxX="<<summaxX/(AnzIy+1)<<endl;
  
  for(i=minIx; i<=maxIx; i++){
      summinY=minY[i]+summinY;
  }
  //cout<<"summinY="<<summinY/(AnzIx+1)<<endl;
  
  for(i=minIx; i<=maxIx; i++){
      summaxY=maxY[i]+summaxY;
  }
  //cout<<"summaxY="<<summaxY/(AnzIx+1)<<endl;
  
  float calibrationX = (190.)/((summaxX/(AnzIy+1))-(summinX/(AnzIy+1)));
  float calibrationY = (260.)/((summaxY/(AnzIx+1))-(summinY/(AnzIx+1)));
  const char* Title = ("Calibration Points");
  cout<<setprecision(15)<<"\nCalibration Faktor cx="<<calibrationX<<endl;
  cout<<setprecision(15)<<"\nCalibration Faktor cy="<<calibrationY<<endl<<endl;
  TCanvas * canvas;
  canvas = new TCanvas("CalibrationPoints", "CalibrationPoints", 500, 700);
  gr1->Draw("AP");
  
  gr1->GetHistogram()->GetXaxis()->SetTitle("x [mm]");
  gr1->GetYaxis()->SetTitleOffset(1.3);
  gr1->GetHistogram()->GetYaxis()->SetTitle("y [mm]");
  gr1->SetTitle(Title);
  
  canvas->Print("CalibrationPoints.pdf");

  return;
}
