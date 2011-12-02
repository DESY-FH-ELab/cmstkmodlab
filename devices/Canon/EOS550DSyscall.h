
#ifndef _EOS550DSYSCALL_H
#define _EOS550DSYSCALL_H

#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <cstdio>
#include <pthread.h>


#include "EOS550D.h"


///
/// operate EOD 550 D
/// via os calls to gphoto2 exe
///
class EOS550DSyscall : public EOS550D {

 public:

  // a command is a command:option pair,
  // e.g. "set-config-index":"iso=XXX"
  typedef std::pair<std::string,std::string> Command;
  typedef std::vector<Command> CommandList;

  EOS550DSyscall();
  virtual ~EOS550DSyscall() {}

  virtual EOS550D::Aperture const& readAperture( void );
  virtual EOS550D::ShutterSpeed const& readShutterSpeed( void );
  virtual EOS550D::Iso const& readIso( void  );
  virtual EOS550D::EOS550DConfig const& readConfig( void );

  virtual bool writeAperture( EOS550D::Aperture );
  virtual bool writeShutterSpeed( EOS550D::ShutterSpeed );
  virtual bool writeIso( EOS550D::Iso );
  virtual bool writeConfig( EOS550DConfig const& );

  virtual bool getAndSaveImage( const std::string& );

  void setGphoto2Path( std::string& p ) { gphoto2_ = p; } // for default see ctor impl.

  void test( void );

 private:

  std::string gphoto2_;
  bool executeCommandList( CommandList const& );
  bool executeCommandListReturnOutput( CommandList const&, std::string& );
  
};

#endif
