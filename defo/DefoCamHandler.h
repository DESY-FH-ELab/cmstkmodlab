

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
  void setCfg( EOS550D::EOS550DConfig const& c ) { cfg_ = c; }
  void setFilePath( std::string const& p ) { filePath_ = p; }

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
