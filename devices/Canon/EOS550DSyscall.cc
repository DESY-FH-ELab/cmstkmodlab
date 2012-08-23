
#include "EOS550DSyscall.h"



///
///
///
EOS550DSyscall::EOS550DSyscall() : gphoto2_( std::string( "/usr/bin/gphoto2" ) ) {
}



///
/// query the camera & return currently set aperture 
///
EOS550D::Aperture const& EOS550DSyscall::readAperture( void ) {

  CommandList list;
  Command aCmd;

  aCmd.first = "get-config";
  aCmd.second = "aperture";
  
  list.push_back( aCmd );
  std::string output;
  executeCommandListReturnOutput( list, output );

  // now parse the output
  std::stringstream s;
  s << output;
  
  // get current value;
  // scan the list that gphoto spits out,
  // get the value of "Current:" entry and compare with
  // the index:value list to get the index (->enum)

  // we can use doubles for any possible value of aperture
  std::string buf; double val, cmp; int index;
  while( std::string( "Current:" ) != buf ) s >> buf; // wind forward till "Current:"
  s >> val; // this is the current value;
  while( !s.eof() ) {
    s >> buf >> index >> cmp; // buf is garbage
    if( cmp == val ) {
      cfg_.aperture_ = static_cast<EOS550D::Aperture>( index );
      return cfg_.aperture_;
    }
  }
  
  cfg_.aperture_ = EOS550D::FINVALID;
  return cfg_.aperture_;

}



///
/// query the camera & return currently set speed
///
EOS550D::ShutterSpeed const& EOS550DSyscall::readShutterSpeed( void ) {

  CommandList list;
  Command aCmd;

  aCmd.first = "get-config";
  aCmd.second = "shutterspeed";
  
  list.push_back( aCmd );
  std::string output;
  executeCommandListReturnOutput( list, output );

  // now parse the output
  std::stringstream s;
  s << output;
  
  // get current value: (see readAperture comment);
  // here we have to use strings since values can be "bulb", "1/4", etc.
  std::string buf, val, cmp; int index;
  while( std::string( "Current:" ) != buf ) s >> buf; // wind forward till "Current:"
  s >> val; // this is the current value;
  while( !s.eof() ) {
    s >> buf >> index >> cmp; // buf is garbage
    if( cmp == val ) {
      cfg_.shutterSpeed_ = static_cast<EOS550D::ShutterSpeed>( index );
      return cfg_.shutterSpeed_;
    }
  }
  
  cfg_.shutterSpeed_ = EOS550D::SINVALID;
  return cfg_.shutterSpeed_;

}



///
/// query the camera & return currently set iso seeting
///
EOS550D::Iso const& EOS550DSyscall::readIso( void ) {

  CommandList list;
  Command aCmd;

  aCmd.first = "get-config";
  aCmd.second = "iso";
  
  list.push_back( aCmd );
  std::string output;
  executeCommandListReturnOutput( list, output );

  // now parse the output
  std::stringstream s;
  s << output;
  
  // get current value: (see readAperture comment);
  // here we have to use strings since value can be "auto"
  std::string buf, val, cmp; int index;
  while( std::string( "Current:" ) != buf ) s >> buf; // wind forward till "Current:"
  s >> val; // this is the current value;
  while( !s.eof() ) {
    s >> buf >> index >> cmp; // buf is garbage
    if( cmp == val ) {
      cfg_.iso_ = static_cast<EOS550D::Iso>( index );
      return cfg_.iso_;
    }
  }
  
  cfg_.iso_ = EOS550D::ISOINVALID;
  return cfg_.iso_;

}



///
/// read full cfg object in one go
/// (with one gphoto call)
///
EOS550D::EOS550DConfig const& EOS550DSyscall::readConfig( void ) {

  CommandList list;
  Command aCmd;
  
  aCmd.first = "get-config";
  aCmd.second = "aperture";
  list.push_back( aCmd );

  aCmd.first = "get-config";
  aCmd.second = "shutterspeed";
  list.push_back( aCmd );

  aCmd.first = "get-config";
  aCmd.second = "iso";
  list.push_back( aCmd );

  std::string output;
  executeCommandListReturnOutput( list, output );

  // now parse the output
  std::stringstream s;
  s << output;

  
  { // first aperture
    std::string buf; double val, cmp; int index;
    while( std::string( "Current:" ) != buf ) s >> buf; // wind forward till "Current:"
    s >> val; // this is the current aperture value
    do {
      s >> buf >> index >> cmp;
      if( cmp == val ) { cfg_.aperture_ = static_cast<EOS550D::Aperture>( index ); break; }
    } while( std::string( "Choice:" ) == buf );
    if( cmp != val ) cfg_.aperture_ = EOS550D::FINVALID; // just in case
  }

  {
    // now shutterspeed
    std::string buf, val, cmp; int index;
    while( std::string( "Current:" ) != buf ) s >> buf; // wind forward till "Current:"
    s >> val; // this is the current aperture value
    do {
      s >> buf >> index >> cmp;
      if( cmp == val ) { cfg_.shutterSpeed_ = static_cast<EOS550D::ShutterSpeed>( index ); break; }
    } while( std::string( "Choice:" ) == buf );
    if( cmp != val ) cfg_.shutterSpeed_ = EOS550D::SINVALID; // just in case
  }

  {
    // finally, iso
    std::string buf, val, cmp; int index;
    while( std::string( "Current:" ) != buf ) s >> buf; // wind forward till "Current:"
    s >> val; // this is the current aperture value
    do {
      s >> buf >> index >> cmp;
      if( cmp == val ) { cfg_.iso_ = static_cast<EOS550D::Iso>( index ); break; }
    } while( std::string( "Choice:" ) == buf );
    if( cmp != val ) cfg_.iso_ = EOS550D::ISOINVALID; // just in case
  }

  return cfg_;

}



///
///
///
bool EOS550DSyscall::writeAperture( EOS550D::Aperture aperture ) {

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
bool EOS550DSyscall::writeShutterSpeed( EOS550D::ShutterSpeed speed ) {

  CommandList list;
  Command aCmd;

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
bool EOS550DSyscall::writeIso( EOS550D::Iso iso ) {

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
/// set all known cfg parameters in one go
/// (one single gphoto2 call)
///
bool EOS550DSyscall::writeConfig( EOS550D::EOS550DConfig const& cfg ) {
  
  CommandList list;
  Command aCmd;
  std::stringstream s;

  s << "aperture=" << cfg.aperture_;
  aCmd.first = "set-config-index";
  aCmd.second = s.str();
  list.push_back( aCmd );

  s.str("");
  s << "shutterspeed=" << cfg.shutterSpeed_;
  aCmd.first = "set-config-index";
  aCmd.second = s.str();
  list.push_back( aCmd );

  s.str("");
  s << "iso=" << cfg.iso_;
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

  aCmd.first = "filename"; // specify filepath
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
  //  commandString += " 2>&1";
  std::cout << "LLL: " << commandString << std::endl; /////////////////////////////////


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
