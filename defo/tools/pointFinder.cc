
#include <iostream>
#include <iomanip>

#include <QFileInfo>

#include "DefoRecoImage.h"
#include "DefoRawImage.h"


int main( int argc, char** argv ) {

  if( argc != 2 ) {
    std::cerr << " [pointFinder] ** Usage: pointFinder <image file>" << std::endl;
    return -1;
  }

  QFileInfo file( argv[1] );
  if( !file.exists() ) {
    std::cerr << " [pointFinder] ** Error: cannot open file: \"" << argv[1] << "\"." << std::endl;
    return -2;
  }

  // load image from file
  DefoRawImage image( argv[1] );

  // define a pseudo-area (whole image)
  QRect rect( 0, 0, image.getImage().width(), image.getImage().height() );
  DefoArea area( rect );

  DefoRecoImage recoImage( "pointFinder.cfg" );
  std::pair<DefoPointCollection,DefoRawImage> output = recoImage.reconstruct( image, area );

  for( DefoPointCollection::const_iterator it = output.first.begin(); it < output.first.end(); ++it ) {
    std::cout << "POINT"
	      << std::setw( 5 ) << it-output.first.begin()+1
	      << std::setw( 10 ) << it->getX()
	      << std::setw( 10 ) << it->getY()
	      << std::endl;
  }

}
