
#ifndef _EOS550DSYSCALL_H
#define _EOS550DSYSCALL_H

#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <cstdio>

#include "EOS550D.h"


///
/// operate EOD 550 D
/// via os calls to gphoto2 exe
///
class EOS550DSyscall : public EOS550D {

 public:

  typedef std::pair<std::string,std::string> Command;
  typedef std::vector<Command> CommandList;

  EOS550DSyscall();
  virtual ~EOS550DSyscall() {}


  virtual bool setAperture( EOS550D::Aperture );
  virtual bool setShutterSpeed( EOS550D::ShutterSpeed );
  virtual bool setIso( EOS550D::Iso );

  virtual bool getAndSaveImage( const std::string& );
  void setGphoto2Path( std::string& p ) { gphoto2_ = p; } // for default see ctor impl.

  void test( void );

 private:

  std::string gphoto2_;
  bool executeCommandList( CommandList const& );
  bool executeCommandListReturnOutput( CommandList const&, std::string& );
  
};

#endif
