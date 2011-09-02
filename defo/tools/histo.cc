
/*

@ creates color channel histograms for an image file
(allowed types: all that can be read with QT QImage class)

@ usage:

histo <inputfile>

@ compile:

g++ -o histo histo.cc -I$ROOTSYS/include -I/usr/include/QtGui -I/usr/include/QtCore `root-config --libs` -L/usr/lib64 -lQtGui

*/



#include <iostream>
#include <utility>
#include <string>

#include "QImage"

#include "TCanvas.h"
#include "TH1.h"
#include "TApplication.h"


///
///
///
int main( int argc, char** argv ) {

  if( argc < 2 ) {
    std::cerr << " Usage: histo <filename>" << std::endl;
    return -1;
  }

  std::string filename( argv[1] );

  QImage image;
  if( ! image.load( filename.c_str() ) ) {
    std::cerr << " Cannot open: \"" << filename.c_str() << "\"." << std::endl;
    return -2;
  }

  TApplication theApp( "App", &argc, argv );
  
  TH1D* hRed   = new TH1D( "red", "red", 256, 0, 255 );
  TH1D* hGreen = new TH1D( "green", "green", 256, 0, 255 );
  TH1D* hBlue  = new TH1D( "blue", "blue", 256, 0, 255 );
  TH1D* hGray  = new TH1D( "gray", "gray", 256, 0, 255 );

  const std::pair<int,int> imageFormat = std::make_pair<int,int>( image.width(), image.height() );
  
  std::cout << " Image has: " << imageFormat.first << " x " << imageFormat.second << " pixels." << std::endl;
  
  for( unsigned int w = 0; w < imageFormat.first; ++w ) {
    for( unsigned int h = 0; h < imageFormat.second; ++h ) {
      hGray->Fill( qGray( image.pixel( w, h ) ) );
      hRed->Fill( qRed( image.pixel( w, h ) ) );
      hGreen->Fill( qGreen( image.pixel( w, h ) ) );
      hBlue->Fill( qBlue( image.pixel( w, h ) ) );
    }
  }


  hRed->SetFillColor( kRed );
  hRed->SetLineColor( kRed );
  hRed->SetStats( 0 );
  hGreen->SetFillColor( kGreen+2 );
  hGreen->SetLineColor( kGreen+2 );
  hGreen->SetStats( 0 );
  hBlue->SetFillColor( kBlue );
  hBlue->SetLineColor( kBlue );
  hBlue->SetStats( 0 );
  hGray->SetFillColor( kGray );
  hGray->SetLineColor( kGray );
  hGray->SetStats( 0 );

  TCanvas* canvas = new TCanvas( filename.c_str(), filename.c_str(), 1024, 768 );
  canvas->Divide( 2, 2 );
  canvas->SetFillColor( 0 );
  canvas->SetFrameFillColor( 0 );
  canvas->Draw();

  for( unsigned int i = 1; i <= 4; ++i ) {
    canvas->GetPad( i )->SetFillColor( 0 );
    canvas->GetPad( i )->SetFrameFillColor( 0 );
    canvas->GetPad( i )->SetBorderMode( 0 );
    canvas->GetPad( i )->SetFrameBorderMode( 0 );
    canvas->GetPad( i )->SetLogy();
    
  }

  canvas->cd( 1 );
  hRed->Draw();
  canvas->cd( 2 );
  hGreen->Draw();
  canvas->cd( 3 );
  hBlue->Draw();

  // now the 4th pad, grayscale + r,g,b as line histos

  TH1D* hRedL = new TH1D( *hRed );
  hRedL->SetFillStyle( 0 );
  TH1D* hGreenL = new TH1D( *hGreen );
  hGreenL->SetFillStyle( 0 );
  TH1D* hBlueL = new TH1D( *hBlue );
  hBlueL->SetFillStyle( 0 );

  canvas->cd( 4 );
  hGray->Draw();
  hRedL->Draw( "SAME" );
  hGreenL->Draw( "SAME" );
  hBlueL->Draw( "SAME" );

  theApp.Run( true );

}
