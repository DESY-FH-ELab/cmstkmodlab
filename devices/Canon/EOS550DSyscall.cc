
#include "EOS550DSyscall.h"



///
///
///
EOS550DSyscall::EOS550DSyscall() : gphoto2_( std::string( "/usr/bin/gphoto2" ) ) {
}



///
///
///
bool EOS550DSyscall::setAperture( EOS550D::Aperture aperture ) {
}



///
///
///
bool EOS550DSyscall::setShutterSpeed( EOS550D::ShutterSpeed speed ) {


  // TESTING

  CommandList list;
  std::pair<std::string,std::string> aCmd;

  aCmd.first = "set-config";
  aCmd.second = "XYZ";
  
  list.push_back( aCmd );
  executeCommandList( list );

}



///
///
///
bool EOS550DSyscall::setIso( EOS550D::Iso iso ) {
}



///
///
///
bool EOS550DSyscall::getAndSaveImage( const std::string& filepath ) {
}



///
///
///
bool EOS550DSyscall::executeCommandList( CommandList const& list ) {

  std::string commandString = gphoto2_;

  for( CommandList::const_iterator it = list.begin(); it < list.end(); ++it ) {

    // build call
    commandString += " ";
    commandString += "--";
    commandString += it->first;

    // for commands with an option
    if( ! it->second.empty() ) {
      commandString += "=";
      commandString += it->second;
    }

  }

  std::cout << commandString << std::endl;

  return true;
  

}
