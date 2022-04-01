/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <array>

#include <nqlogger.h>
#include <HistoryFifo.h>

int main(int argc, char ** argv)
{
  NQLogger::instance()->addActiveModule("*");
  NQLogger::instance()->addDestiniation(stdout, NQLog::Debug);

  {
    HistoryFifo<double> fifo(5);

    fifo.push_back(QDateTime::fromString("2017-06-13T09:09:11", Qt::ISODate), 10);
    fifo.push_back(QDateTime::fromString("2017-06-13T09:09:12", Qt::ISODate), 20);
    fifo.push_back(QDateTime::fromString("2017-06-13T09:09:13", Qt::ISODate), 30);
    fifo.push_back(QDateTime::fromString("2017-06-13T09:09:14", Qt::ISODate), 40);
    fifo.push_back(QDateTime::fromString("2017-06-13T09:09:15", Qt::ISODate), 50);
    fifo.push_back(QDateTime::fromString("2017-06-13T09:09:16", Qt::ISODate), 60);
    fifo.push_back(QDateTime::fromString("2017-06-13T09:09:17", Qt::ISODate), 70);
    fifo.push_back(QDateTime::fromString("2017-06-13T09:09:18", Qt::ISODate), 80);
    fifo.push_back(QDateTime::fromString("2017-06-13T09:09:19", Qt::ISODate), 90);

    std::cout << "content of history fifo:" << std::endl;
    for (int i=0;i<fifo.size();++i) {
      std::cout << fifo.timeAt(i).toString(Qt::ISODate).toStdString() << " : " << fifo.valueAt(i) << std::endl;
    }

    if (fifo.timeBack().toString(Qt::ISODate)!="2017-06-13T09:09:15") {

      std::cout << "\nhistory fifo content check failed\n" << std::endl;

      std::cout << "expected date and time:  2017-06-13T09:09:15" << std::endl;
      std::cout << "content of history fifo: " << fifo.timeBack().toString(Qt::ISODate).toStdString() << std::endl;

      return 1;
    }

    /*
    for (int i=0;i<fifo.size();++i) {
      std::cout << fifo.timeAt(i).toString(Qt::ISODate).toStdString() << " : " << fifo.valueAt(i) << std::endl;
    }

    std::cout << fifo.deltaTime(0, 4) << std::endl;
    std::cout << fifo.delta(0, 4) << std::endl;
    std::cout << fifo.gradient(0, 4) << std::endl;
    */
  }

  return 0;
}
