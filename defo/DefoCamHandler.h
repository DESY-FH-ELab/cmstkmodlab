

#ifndef _DEFOCAMHANDLER_H
#define _DEFOCAMHANDLER_H

#include <iostream>
#include <string>

#include <QThread>

#include "devices/Canon/EOS550DSyscall.h"


///
/// threaded for handling cam access
///
class DefoCamHandler : public QThread {

 Q_OBJECT
 
 public:

  enum Action { GETCFG, SETCFG, GETIMAGE, INVALID };

  DefoCamHandler() {}
  ~DefoCamHandler() {}
  void setAction( DefoCamHandler::Action a ) { action_ = a; }
  EOS550D::EOS550DConfig const& getConfig( void ) { return cfg_; }
  void setCfg( EOS550D::EOS550DConfig const& c ) { cfg_ = c; }
  void setFilePath( std::string const& p ) { filePath_ = p; }
  void setShutterSpeed( EOS550D::ShutterSpeed s ) { cfg_.shutterSpeed_ = s; }
  void setAperture( EOS550D::Aperture a ) { cfg_.aperture_ = a; }
  void setIso( EOS550D::Iso a ) { cfg_.iso_ = a; }

 private:
  EOS550DSyscall camera_;
  std::string filePath_;
  DefoCamHandler::Action action_;
  EOS550D::EOS550DConfig cfg_;
  
 protected:
  void run();

 signals:
  void actionFinished( DefoCamHandler::Action );

};

#endif
