#ifndef _DEFOCONFIGWRITER_H
#define _DEFOCONFIGWRITER_H

#include <string>
#include <iostream>
#include <map>

#include <QObject>

class DefoConfigWriter : public QObject {

  Q_OBJECT

 public:
  DefoConfigWriter( const std::string & filename );
  ~DefoConfigWriter();

  void write(std::map<std::string,std::string> &keyvalueMap);
  std::string getFileName( void ) { return outputFileName_; }

 private:
  std::string outputFileName_;
  
  void writeMerge(std::map<std::string,std::string> &keyvalueMap);
  void writeNew(std::map<std::string,std::string> &keyvalueMap);
};

#endif
