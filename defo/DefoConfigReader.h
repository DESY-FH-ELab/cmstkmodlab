
#ifndef _DEFOCONFIGREADER_H
#define _DEFOCONFIGREADER_H

#include <string>
#include <iostream>
#include <fstream>

class DefoConfigReader {

 public:
  DefoConfigReader( const char* );
  ~DefoConfigReader();

  ///
  ///
  ///
  template <class aType> aType getValue( std::string const& key ) const {

    std::ifstream file( inputFileName_.c_str(), std::ios::in );
    if( !file.good() ) {
      std::cerr << " [DefoConfigReader::openAndCheckFile] ** ERROR: failed to open file: " << inputFileName_ << "." << std::endl;
      throw;
    }

    bool isFound = false;
    aType returnValue = 0;
    std::string buffer;
    
    while( !file.eof() ) {
      file >> buffer;
      if( key == buffer ) {
	file >> returnValue; isFound = true;
      }
    }
    
    file.close();
    if( !isFound ) issueKeyError( key );
    return returnValue;
    
  }

 private:
  void issueKeyError( const std::string& ) const;
  std::string inputFileName_;

};

#endif
