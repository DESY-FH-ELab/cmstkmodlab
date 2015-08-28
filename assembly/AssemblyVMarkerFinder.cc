#include <iostream>

#include <QApplication>

#include <nqlogger.h>

#include "AssemblyVMarkerFinder.h"

AssemblyVMarkerFinder::AssemblyVMarkerFinder(QObject *parent)
    : QObject(parent)
{

}

AssemblyVMarkerFinder::~AssemblyVMarkerFinder()
{
    NQLog("AssemblyVMarkerFinder") << "delete";
}
