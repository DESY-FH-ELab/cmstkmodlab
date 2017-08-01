/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <QApplication>

#include <nqlogger.h>

#include "AssemblyVMarkerFinder.h"

AssemblyMarkerCircle::AssemblyMarkerCircle(float x, float y, float r, float q)
    : x_(x),
      y_(y),
      r_(r),
      q_(q)
{

}

AssemblyVMarkerFinder::AssemblyVMarkerFinder(QObject *parent)
    : QObject(parent)
{

}

AssemblyVMarkerFinder::~AssemblyVMarkerFinder()
{
    NQLog("AssemblyVMarkerFinder") << "delete";
}
