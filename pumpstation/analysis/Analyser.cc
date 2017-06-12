#include <iostream>

#include <QtCore>

#include "Analyser.h"

Analyser::Analyser(QStringList& arguments)
 : arguments_(arguments)
{

}

void Analyser::analyse()
{
  QStringList::const_iterator constIterator;
      for (constIterator = arguments_.constBegin(); constIterator != arguments_.constEnd();
             ++constIterator)
          std::cout << (*constIterator).toLocal8Bit().constData() << std::endl;


  QCoreApplication::quit();
}
