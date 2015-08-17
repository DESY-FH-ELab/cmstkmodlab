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
  int minPosIy(9999);
  double minPosY(9999);
  int maxNegIy(-9999);
  double maxNegY(-9999);

  TString dummy;
  dummy.ReadLine(ifile);

  std::map<int,double> minX, maxX, minY, maxY;
 
  // Gehe ueber alle Punkte und fuelle sie in 1D-Graphen laengs y
  while (ifile >> ix >> iy >> x >> y >> px >> py >> dx >> a1 >> dy >> a2 >> dxy >> corrx >> corry) {

    px -= 0.5*3456;
    py -= 0.5*5184;

    if (minX.find(iy)==minX.end()) minX[iy] = px;
    if (maxX.find(iy)==maxX.end()) maxX[iy] = px;
    if (minY.find(ix)==minY.end()) minY[ix] = py;
    if (maxY.find(ix)==maxX.end()) maxY[ix] = py;
    
    if (py>=0 && py < minPosY) {
      minPosY = py;
      minPosIy = iy;
    }
    
    if (py<0 && py > maxNegY) {
      maxNegY = py;
      maxNegIy = iy;
    }
       
    minX[iy] = std::min(px, minX[iy]);
    maxX[iy] = std::max(px, maxX[iy]);
    minY[ix] = std::min(py, minY[ix]);
    maxY[ix] = std::max(py, maxY[ix]);

    gr1->SetPoint(nPoints1++, px, py);
    
    if(ix > maxIx) maxIx = ix;
    if(ix < minIx) minIx = ix;
    if(iy > maxIy) maxIy = iy;
    if(iy < minIy) minIy = iy;
  }

  ifile.close();
  ifile.open(inputFileName.data());

  dummy.ReadLine(ifile);

  TGraph * gr2 = new TGraph();
  gr2->SetMarkerStyle(21);

  TGraph * gr3 = new TGraph();
  gr3->SetMarkerColor(2);
  gr3->SetMarkerStyle(21);
  
  while (ifile >> ix >> iy >> x >> y >> px >> py >> dx >> a1 >> dy >> a2 >> dxy >> corrx >> corry) {

    px -= 0.5*3456;
    py -= 0.5*5184;

    if (iy == minPosIy && ix == minIx) {
      gr2->SetPoint(0, px, py);
    }
    if (iy == maxNegIy && ix == minIx) {
      gr2->SetPoint(1, px, py);
    }

    if (iy == minPosIy && ix == maxIx) {
      gr2->SetPoint(2, px, py);
    }
    if (iy == maxNegIy && ix == maxIx) {
      gr2->SetPoint(3, px, py);
    }
  }
  
  double dX = gr2->GetX()[0] - gr2->GetX()[1];
  double dY = gr2->GetY()[0] - gr2->GetY()[1];
  double temp = gr2->GetY()[1] / dY;

  gr3->SetPoint(0, gr2->GetX()[1] - temp * dX, 0);
  double Xmin = gr2->GetX()[1] - temp * dX;

  dX = gr2->GetX()[2] - gr2->GetX()[3];
  dY = gr2->GetY()[2] - gr2->GetY()[3];
  temp = gr2->GetY()[3] / dY;

  gr3->SetPoint(1, gr2->GetX()[3] - temp * dX, 0);
  double Xmax = gr2->GetX()[3] - temp * dX;

  dX = (Xmax - Xmin) * 4.301697530864198e-03;
  
  double ImageScale = dX / 190.;

  std::cout << "image scale = " << ImageScale << std::endl;

  TCanvas * canvas;
  canvas = new TCanvas("CalibrationPoints", "CalibrationPoints", 500, 700);
  gr1->Draw("AP");
  gr2->Draw("P");
  gr3->Draw("P");
  
  gr1->GetHistogram()->GetXaxis()->SetTitle("x [mm]");
  gr1->GetYaxis()->SetTitleOffset(1.3);
  gr1->GetHistogram()->GetYaxis()->SetTitle("y [mm]");
  
  canvas->Print("CameraCalibrationPoints.pdf");
}
