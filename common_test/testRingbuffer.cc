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
#include <Ringbuffer.h>

int main(int argc, char ** argv)
{
  NQLogger::instance()->addActiveModule("*");
  NQLogger::instance()->addDestiniation(stdout, NQLog::Debug);

  {
    Ringbuffer<int, 5> rb;
    
    for (auto i = 0;i<=7;++i) {
      rb.push_back(i);
    }

    std::cout << "content of ring buffer:";
    for (auto & v : rb) {
      std::cout << " " << v;
    }
    std::cout << std::endl;

    std::array<int, 5> rbref{ 7, 6, 5, 4, 3 };

    for (size_t i = 0;i<rbref.size();++i) {
      if (rbref[i]!=rb[i]) {

        std::cout << "\nring buffer content check failed\n" << std::endl;

        std::cout << "expected content of ring buffer:";
        for (auto & v : rbref) {
          std::cout << " " << v;
        }
        std::cout << std::endl;

        std::cout << "content of ring buffer:         ";
        for (auto & v : rb) {
          std::cout << " " << v;
        }
        std::cout << std::endl;

        return 1;
      }
    }
  }

  return 0;
}
