
#include "DefoConfigReader.h"

///
///
///
DefoConfigReader::DefoConfigReader( const char* inputFileName ) {

  inputFileName_ = std::string( inputFileName );

}



///
///
///
DefoConfigReader::~DefoConfigReader() {
}



///
///
///
void DefoConfigReader::issueKeyError( std::string const& key ) const {
  
  std::cerr << " [issueKeyError::getValue] ** ERROR: failed to get value for key: " << key << "." << std::endl;
  throw;
  
}



