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
#include <Fifo.h>

int main(int argc, char ** argv)
{
  NQLogger::instance()->addActiveModule("*");
  NQLogger::instance()->addDestiniation(stdout, NQLog::Debug);

  {
    Fifo<int> fifo(5);

    for (auto i = 0;i<=8;++i) {
      fifo.push_back(i);
    }

    std::cout << "content of fifo:";
    for (auto & v : fifo) {
      std::cout << " " << v;
    }
    std::cout << std::endl;

    std::array<int, 5> fiforef{ 8, 7, 6, 5, 4 };

    for (size_t i = 0;i<fiforef.size();++i) {
      if (fiforef[i]!=fifo[i]) {

        std::cout << "\nfifo content check failed\n" << std::endl;

        std::cout << "expected content of fifo:";
        for (auto & v : fiforef) {
          std::cout << " " << v;
        }
        std::cout << std::endl;

        std::cout << "content of fifo:         ";
        for (auto & v : fifo) {
          std::cout << " " << v;
        }
        std::cout << std::endl;

        return 1;
      }
    }
  }

  return 0;
}
