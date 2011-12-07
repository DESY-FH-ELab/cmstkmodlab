

#ifndef _EOS550D_H
#define _EOS550D_H

#include <iostream>


///
/// Base class for EOS550D
///
class EOS550D {

 public:

  // this contains enums with the allowed
  // settings for Aperture, ShutterSpeed, Iso, ...
  #include "eosParameters.h"


  class EOS550DConfig {
  public:
    EOS550DConfig() {}
    ~EOS550DConfig() {}
    EOS550D::Aperture aperture_;
    EOS550D::ShutterSpeed shutterSpeed_;
    EOS550D::Iso iso_;
  };


  EOS550D() {}
  virtual ~EOS550D() {}

  virtual EOS550D::Aperture const& getAperture( void ) { return cfg_.aperture_; }
  virtual EOS550D::ShutterSpeed const& getShutterSpeed( void ) { return cfg_.shutterSpeed_; }
  virtual EOS550D::Iso const& getIso( void ) { return cfg_.iso_; }
  virtual EOS550D::EOS550DConfig const& getConfig( void ) { return cfg_; }

  virtual EOS550D::Aperture const& readAperture( void ) = 0;
  virtual EOS550D::ShutterSpeed const& readShutterSpeed( void ) = 0;
  virtual EOS550D::Iso const& readIso( void  ) = 0;
  virtual EOS550D::EOS550DConfig const& readConfig( void ) = 0;

  virtual bool writeAperture( EOS550D::Aperture ) = 0;
  virtual bool writeShutterSpeed( EOS550D::ShutterSpeed ) = 0;
  virtual bool writeIso( EOS550D::Iso ) = 0;
  virtual bool writeConfig( EOS550DConfig const& ) = 0;

  virtual bool getAndSaveImage( const std::string& ) = 0;

 protected: 

  EOS550D::EOS550DConfig cfg_;

};

#endif
