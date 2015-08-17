#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>

#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TParameter.h"
#include "TMath.h"

Double_t fitFunc(Double_t* y, Double_t *par){
  // Warning This value needs to be specified twice, again below
  Double_t length(281.);
  
  Double_t func_left	= par[0]*(3.*(((y[0]+par[1])/length))-4.*(pow(((y[0]+par[1])/length),3)))+par[2];
  Double_t func_right	= par[0]*(-1.+9.*((y[0]+par[1])/length)-12.*(pow(((y[0]+par[1])/length),2))+4.*(pow(((y[0]+par[1])/length),3)))+par[2];

  Double_t Func(-999.);
  
  if(y[0]<((length/2)-par[1]))			Func = func_left;
  else if(y[0]>((length/2)-par[1]))		Func = func_right;
  
  return Func;
}

void plot(const std::string& inputFileName)
{
  gStyle->SetPalette(1);
  
  std::ifstream ifile(inputFileName.data());
  if(!(ifile.is_open())){
    std::cerr<<"ERROR! Requested file not found, file name: "<<inputFileName
             <<"\n...break\n"<<std::endl;
    exit(321);
  }

  TCanvas* canvas1;
  canvas1 = new TCanvas("canvas1");
  canvas1->cd();
    
  double x, y, px, py,dx, dy, dxy, corrx, corry;
  int ix, iy;
  int a1, a2;

  int maxIx(-9999);
  int minIx(9999);
  int maxIy(-9999);
  int minIy(9999);
  std::map<int, TGraph*> m_graph;
  
  double zMaxZ(-9999.);
  double yMaxZ(-9999.);
  int ixMaxZ(-999);
  int iyMaxZ(-999);
  double ymin(999);
  double ymax(-999);
  double ylength(0);
  double x_1(0);
  double x_2(0);
  double y_1(0);
  double y_2(0);
 
//####################################################  
// Important you must input the overall length of your DUT in mm!!!

  double length(281.);
  
//#####################################################

  TString dummy;
  dummy.ReadLine(ifile);
  
  // Gehe ueber alle Punkte und fuelle sie in 1D-Graphen laengs y
  while (ifile >> ix >> iy >> x >> y >> px >> py >> dx >> a1 >> dy >> a2 >> dxy >> corrx >> corry) {
    if(m_graph.find(ix) == m_graph.end()){
      dx/=1000.0;
      m_graph[ix] = new TGraph();
      m_graph[ix]->SetPoint(m_graph[ix]->GetN(), y, dx);
    }
    else{
      dx/=1000.0;
      m_graph[ix]->SetPoint(m_graph[ix]->GetN(), y, dx);
    }
    if(ix > maxIx) maxIx = ix;
    if(ix < minIx) minIx = ix;
    if(iy > maxIy) maxIy = iy;
    if(iy < minIy) minIy = iy;
    
    if(dx > zMaxZ){
      yMaxZ = y;
      zMaxZ = dx;
      ixMaxZ = ix;
      iyMaxZ = iy;
    }
    if(iy==maxIy){
      x_2=y;
      y_2=dx;
    }
    if(iy==(maxIy-1)){
      x_1=y;
      y_1=dx;
    }
    if(y < ymin){
      ymin = y;
    }
    if(y > ymax){
      ymax = y;
    }
  }
  ifile.close();
  
  
  if(ymin<0){
     if(ymax<0){
       ylength=ymax+ymin;
     }
     else{
       ylength=ymax-ymin;
     }
  }
  else{
    if(ymax<0){
      ylength=ymin+ymax;
    }
    else{
      ylength=ymin-ymax;
    }
  }
  
  cout<<"\nylength="<<ylength<<endl;
  cout<<"ymin="<<ymin<<endl;
  cout<<"ymax="<<ymax<<endl;
  cout<<"yMaxZ="<<yMaxZ<<endl;
  std::cout<<"Maximum z value (value, x index, y index): "<<zMaxZ<<" , "<<ixMaxZ<<" , "<<iyMaxZ<<"\n"; 
  
  
  double	m	= (y_2-y_1)/(x_2-x_1);
  double	y_corr 	= m*((length/2.)-(ymax-yMaxZ));
  cout<<"m="<<m<<"mm"<<endl;
  cout<<"y_corr="<<y_corr<<"mm"<<endl;
  TF1* fit_func = new TF1("fit_func", fitFunc, ymin+ymin*0.2, ymax+ymax*0.2, 3);
  fit_func->SetParameters((zMaxZ+y_corr), ((length/2)-yMaxZ), y_corr);
  fit_func->SetParLimits(3,yMaxZ-10., yMaxZ+10.);
  //(1/length) par 1
  //((length/2)-yMaxZ) par 2
 
  //show fitparameter in Plot
  gStyle->SetOptFit(11);
  gStyle->SetHistMinimumZero(kTRUE); // no zero-suppression on y-axis 
  // 1D graph fuer speziellen x-Wert (hier Maximum)
  int wahlIndex(ixMaxZ);
  if(m_graph.find(wahlIndex) != m_graph.end()){
    m_graph[wahlIndex]->GetXaxis()->SetTitle("x [mm]");
    m_graph[wahlIndex]->GetYaxis()->SetTitle("z [mm]");
    m_graph[wahlIndex]->GetYaxis()->SetTitleOffset(1.4);
    m_graph[wahlIndex]->Fit(fit_func, "R");
    m_graph[wahlIndex]->Draw("A*");
    fit_func->Draw("sames");
      
   }
  Double_t	p0 	= fit_func->GetParameter(0);
  Double_t	p2 	= fit_func->GetParameter(2);
  //double	dz	= zMaxZ-p2;
  cout<<"p0="<<p0<<endl;
  //cout<<setprecision(11)<<"cz="<<0.1/p0<<endl;
  cout<<"p2="<<p2<<endl;
  cout<<"zMaxZ="<<zMaxZ<<endl;
  //cout<<"yMaxZ="<<yMaxZ<<endl;
  //cout<<"\nyour overall dz="<<dz<<"mm\n"<<endl;
  cout<<setprecision(11)<<"your cz="<<0.1/p0<<"mm\n"<<endl;

  canvas1->Modified();
  canvas1->Update();
  TString outputFileName(inputFileName);
  outputFileName.ReplaceAll(".txt", "_fit.pdf");
  canvas1->Print(outputFileName);
  
  /*
  for(std::map<int, TGraph*>::const_iterator i_graph = m_graph.begin(); i_graph != m_graph.end(); ++i_graph){
    int indexX = i_graph->first;
    TGraph* graph = i_graph->second;
    std::stringstream outputFileNameX;
    outputFileNameX<<inputFileBase<<"_x"<<indexX;
    outputFileNameX<<".eps";
    std::cout<<outputFileNameX.str()<<"\n\n";
    
    TCanvas* canvas2;
    canvas2 = new TCanvas(outputFileNameX.str().data());
    canvas2->cd();
    
    graph->GetXaxis()->SetTitle("x [mm]");
    graph->Draw("A*");
    canvas2->Modified();
    canvas2->Update();
    canvas2->Print(outputFileNameX.str().data());
  }
  */
}
