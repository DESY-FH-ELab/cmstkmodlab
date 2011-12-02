
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

  CommandList list;
  Command aCmd;

  std::stringstream s;
  s << "aperture=" << aperture;

  // we set by index which corresponds to the enum order
  aCmd.first = "set-config-index";
  aCmd.second = s.str();
  
  list.push_back( aCmd );
  executeCommandList( list );

}



///
///
///
bool EOS550DSyscall::setShutterSpeed( EOS550D::ShutterSpeed speed ) {

  CommandList list;
  std::pair<std::string,std::string> aCmd;

  std::stringstream s;
  s << "shutterspeed=" << speed;

  // we set by index which corresponds to the enum order
  aCmd.first = "set-config-index";
  aCmd.second = s.str();
  
  list.push_back( aCmd );
  executeCommandList( list );

}



///
///
///
bool EOS550DSyscall::setIso( EOS550D::Iso iso ) {

  CommandList list;
  Command aCmd;

  std::stringstream s;
  s << "iso=" << iso;

  // we set by index which corresponds to the enum order
  aCmd.first = "set-config-index";
  aCmd.second = s.str();
  
  list.push_back( aCmd );
  executeCommandList( list );

}



///
///
///
bool EOS550DSyscall::getAndSaveImage( const std::string& filepath ) {

  CommandList list;
  Command aCmd;

  aCmd.first = "capture-image-and-download";
  aCmd.second = "";
  list.push_back( aCmd );

  aCmd.first = "filename";
  aCmd.second = filepath.c_str();
  list.push_back( aCmd );

  aCmd.first = "force-overwrite"; // otherwise gphoto asks...
  aCmd.second = "";
  list.push_back( aCmd );

  executeCommandList( list );

}



///
/// execute a list off commands;
/// do basic success checks;
/// discard output
///
bool EOS550DSyscall::executeCommandList( CommandList const& list ) {

  // the gphoto2 exe
  std::string commandString = gphoto2_;

  // build call with options
  for( CommandList::const_iterator it = list.begin(); it < list.end(); ++it ) {

    // build call
    commandString += " ";
    commandString += "--";
    commandString += it->first;

    // for commands with a parameter
    if( ! it->second.empty() ) {
      commandString += "=";
      commandString += it->second;
    }

  }

  // catch stderr, too
  commandString += " 2>&1";

  // run command and get the output
  FILE* out = popen( commandString.c_str(), "r" ); // to be threaded
  char buffer[1024];
  while( fgets( buffer, sizeof( buffer ), out ) ) {
    //    std::cout << buffer; // here we'll do some checking
  }
  pclose( out );

  return true; //  no checking for the moment

}



///
/// execute a list of commands,
/// do basic success checks,
/// return the output;
///
/// output string passed by ref is cleared
/// 
///
bool EOS550DSyscall::executeCommandListReturnOutput( CommandList const& list, std::string& output ) {

    // the gphoto2 exe
  std::string commandString = gphoto2_;

  // build call with options
  for( CommandList::const_iterator it = list.begin(); it < list.end(); ++it ) {

    // build call
    commandString += " ";
    commandString += "--";
    commandString += it->first;

    // for commands with a parameter
    if( ! it->second.empty() ) {
      commandString += "=";
      commandString += it->second;
    }

  }

  // catch stderr, too
  commandString += " 2>&1";


  output.clear();

  // run command and check the output
  FILE* out = popen( commandString.c_str(), "r" ); // to be threaded
  char buffer[1024];
  while( fgets( buffer, sizeof( buffer ), out ) ) {
    //    std::cout << buffer; // here we'll do some checking
    output += buffer;
  }
  pclose( out );

  return true; //  no checking for the moment

}



///
/// testing only, will be removed
///
void EOS550DSyscall::test( void ) {
  
  CommandList list;
  std::pair<std::string,std::string> aCmd;

  std::stringstream s;
  s << "iso=" << EOS550D::ISO100;

  // we set by index which corresponds to the enum order
  aCmd.first = "set-config-index";
  aCmd.second = s.str();
  
  list.push_back( aCmd );
  std::string out;
  executeCommandListReturnOutput( list, out );
  
  std::cout << "========================" << std::endl;
  std::cout << out;
  std::cout << "========================" << std::endl;


}
