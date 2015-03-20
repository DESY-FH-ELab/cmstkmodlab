#ifndef APPLICATIONCONFIGREADER_H
#define APPLICATIONCONFIGREADER_H

#include <string>
#include <iostream>
#include <map>

#include <QObject>

class ApplicationConfigReader : public QObject
{

  Q_OBJECT

 public:
  ApplicationConfigReader( const std::string & filename );
  ~ApplicationConfigReader();

  void fill(std::map<std::string,std::string> &keyvalueMap);
  std::string getFileName( void ) { return inputFileName_; }

 private:

  std::string inputFileName_;
};

#endif // APPLICATIONCONFIGREADER_H
