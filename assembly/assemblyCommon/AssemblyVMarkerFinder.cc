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
