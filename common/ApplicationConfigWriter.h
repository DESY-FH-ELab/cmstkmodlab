#ifndef APPLICATIONCONFIGWRITER_H
#define APPLICATIONCONFIGWRITER_H

#include <string>
#include <iostream>
#include <map>

#include <QObject>

class ApplicationConfigWriter : public QObject
{
  Q_OBJECT

 public:

  ApplicationConfigWriter( const std::string & filename );
  ~ApplicationConfigWriter();

  void write(std::multimap<std::string,std::string> &keyvalueMap);
  std::string getFileName( void ) { return outputFileName_; }

 private:

  std::string outputFileName_;
  
  void writeMerge(std::multimap<std::string,std::string> &keyvalueMap);
  void writeNew(std::multimap<std::string,std::string> &keyvalueMap);
};

#endif // APPLICATIONCONFIGWRITER_H
