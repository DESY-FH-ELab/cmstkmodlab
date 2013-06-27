#ifndef _DEFOCONFIGREADER_H
#define _DEFOCONFIGREADER_H

#include <string>
#include <iostream>
#include <map>

#include <QObject>

class DefoConfigReader : public QObject {

  Q_OBJECT

 public:
  DefoConfigReader( const std::string & filename );
  ~DefoConfigReader();

  void fill(std::map<std::string,std::string> &keyvalueMap);
  std::string getFileName( void ) { return inputFileName_; }

 private:
  std::string inputFileName_;

};

#endif
