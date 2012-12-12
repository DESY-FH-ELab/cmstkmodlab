
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

  QMessageBox::critical( 0, tr("[DefoConfigReader::issueKeyError]"),
	 QString("Failed to read value for key: \"%1\"\n from file: \"%2\".\n Fix immediately, otherwise you will get unexpected results!").arg( QString( key.c_str() ) ).arg( QString( inputFileName_.c_str() ) ),
	 QMessageBox::Ok );
  
}



