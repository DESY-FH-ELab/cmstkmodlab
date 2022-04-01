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
#include <vector>

#include <nqlogger.h>
#include <ApplicationConfig.h>

int main(int argc, char ** argv)
{
  NQLogger::instance()->addActiveModule("*");
  NQLogger::instance()->addDestiniation(stdout, NQLog::Debug);

  {
    ApplicationConfig * config = ApplicationConfig::instance();

    config->append(std::string(Config::CMSTkModLabBasePath) + "/common_test/test1.cfg", "test1");
    config->append(std::string(Config::CMSTkModLabBasePath) + "/common_test/test2.cfg", "test2");

    const ApplicationConfig::storage_t& kvmap = config->getKeyValueMap();
    for (auto & kv : kvmap) {
      std::cout << kv.first.alias << "." << kv.first.key << ":";
      for (auto & v : kv.second) {
        std::cout << " " << v;
      }
      std::cout << std::endl;
    }

    std::vector<int> integer1{ 1, 2, 3, 4 };
    double double1 = 1.2;
    std::vector<int> integer2{ 5, 6, 7, 8 };
    double double2 = 3.4;

    std::vector<int> integer1c = config->getValueVector<int>("test1", "integer1");
    double double1c = config->getValue<double>("test1", "double1");
    std::vector<int> integer2c = config->getValueVector<int>("test2", "integer2");
    double double2c = config->getValue<double>("test2", "double2");

    if (integer1!=integer1c ||
        double1!=double1c ||
        integer2!=integer2c ||
        double2!=double2c) {
      std::cout << "\nConfig content check failed\n" << std::endl;

      std::cout << "Expected:" << std::endl;
      std::cout << "integer1 ";
      for (auto & v : integer1) {
        std::cout << " " << v;
      }
      std::cout << std::endl;
      std::cout << "double1   " << double1 << std::endl;
      std::cout << "integer2 ";
      for (auto & v : integer2) {
        std::cout << " " << v;
      }
      std::cout << std::endl;
      std::cout << "double2   " << double2 << std::endl;

      std::cout << "Read from config files:" << std::endl;
      std::cout << "integer1 ";
      for (auto & v : integer1c) {
        std::cout << " " << v;
      }
      std::cout << std::endl;
      std::cout << "double1   " << double1c << std::endl;
      std::cout << "integer2 ";
      for (auto & v : integer2c) {
        std::cout << " " << v;
      }
      std::cout << std::endl;
      std::cout << "double2   " << double2c << std::endl;

      return 1;
    }
  }

  return 0;
}
