
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
  std::vector<std::pair<DefoPoint,QColor> > output = recoImage.findPoints( image, area );

  for( std::vector<std::pair<DefoPoint,QColor> >::const_iterator it = output.begin(); it < output.end(); ++it ) {
    std::cout << "POINT"
	      << std::setw( 5 ) << it-output.begin()+1
	      << std::setw( 10 ) << it->first.getX()
	      << std::setw( 10 ) << it->first.getY()
	      << "  R " << std::setw( 3 ) << it->second.red()
	      << "  G " << std::setw( 3 ) << it->second.green()
	      << "  B " << std::setw( 3 ) << it->second.blue()
	      << std::endl;
  }
  

}
