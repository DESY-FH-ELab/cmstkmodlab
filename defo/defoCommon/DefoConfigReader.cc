#include <fstream>
#include <sstream>

#include <QMessageBox>

#include "DefoConfigReader.h"

///
///
///
DefoConfigReader::DefoConfigReader( const std::string & inputFileName ) {

  inputFileName_ = std::string( inputFileName );
}

///
///
///
DefoConfigReader::~DefoConfigReader() {

}

void DefoConfigReader::fill(std::map<std::string,std::string> &keyvalueMap) {

  std::ifstream file( inputFileName_.c_str(), std::ios::in );
  if( !file.good() ) {
    std::cerr << " [DefoConfigReader::openAndCheckFile] ** ERROR: failed to open file: "
	      << inputFileName_ << "." << std::endl;
    QMessageBox::critical( 0, tr("[DefoConfigReader::fill]"),
			   QString("Failed to open configuration file: \"%1\". No chance!").arg(QString(inputFileName_.c_str())),
			   QMessageBox::Abort );
    throw; // must abort
  }

  std::string Key;
  std::string Value;
  std::string buffer;
  
  while (std::getline(file, buffer)) {
    while (buffer[0]==' ') buffer = buffer.substr(1, buffer.length());
    if (buffer[0]=='\0') continue;
    if (buffer[0]=='#') continue;

    std::istringstream iss(buffer.c_str(), std::istringstream::in);
    iss >> Key;
    iss >> Value;

    keyvalueMap[Key] = Value;
  }
  
  file.close();
}
