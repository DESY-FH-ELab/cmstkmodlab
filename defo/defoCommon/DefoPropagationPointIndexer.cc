#include <iostream>
#include <cmath>

#include "DefoPropagationPointIndexer.h"

DefoPropagationPointIndexer::DefoPropagationPointIndexer(QObject *parent) :
    DefoVPointIndexer(parent)
{

}

void DefoPropagationPointIndexer::indexPoints(DefoPointCollection *points, const QColor& seedColor) {

    for (DefoPointCollection::iterator it = points->begin();
         it != points->end();
         ++it) {
        it->unindex();
    }

    DefoPoint * seed = findSeed(points, seedColor);
    if (!seed) return;
    seed->setIndex(0, 0);

    double dx, dy, d;
    determineGrid(points, seed, dx, dy, d);

    PointVector horizontalSeeds;
    horizontalSeeds.push_back(seed);

    DefoPoint* currentPoint;
    DefoPoint* nextPoint;

    currentPoint = seed;
    nextPoint = findNeighbour(points, currentPoint, X, Forward, dy/2.0);
    while (nextPoint) {
        nextPoint->setIndex(currentPoint->getIndex().first+1, currentPoint->getIndex().second);
        horizontalSeeds.push_back(nextPoint);
        currentPoint = nextPoint;
        nextPoint = findNeighbour(points, currentPoint, X, Forward, dy/2.0);
    }

    currentPoint = seed;
    nextPoint = findNeighbour(points, currentPoint, X, Backward, dy/2.0);
    while (nextPoint) {
        nextPoint->setIndex(currentPoint->getIndex().first-1, currentPoint->getIndex().second);
        horizontalSeeds.push_back(nextPoint);
        currentPoint = nextPoint;
        nextPoint = findNeighbour(points, currentPoint, X, Backward, dy/2.0);
    }

    for (PointVector::iterator it = horizontalSeeds.begin();
         it != horizontalSeeds.end();
         ++it) {

        currentPoint = *it;
        nextPoint = findNeighbour(points, currentPoint, Y, Forward, dx/2.0);
        while (nextPoint) {
            nextPoint->setIndex(currentPoint->getIndex().first, currentPoint->getIndex().second+1);
            currentPoint = nextPoint;
            nextPoint = findNeighbour(points, currentPoint, Y, Forward, dx/2.0);
        }

        currentPoint = *it;
        nextPoint = findNeighbour(points, currentPoint, Y, Backward, dx/2.0);
        while (nextPoint) {
            nextPoint->setIndex(currentPoint->getIndex().first, currentPoint->getIndex().second-1);
            currentPoint = nextPoint;
            nextPoint = findNeighbour(points, currentPoint, Y, Backward, dx/2.0);
        }
    }
}

DefoPoint * DefoPropagationPointIndexer::findSeed(DefoPointCollection* points, const QColor& seedColor) {

    DefoPoint * temp = 0;
    for (DefoPointCollection::iterator it = points->begin();
         it != points->end();
         ++it) {
        if (it->hasReferenceColor(seedColor)) {
            temp = &(*it);
        }
    }

    return temp;
}

void DefoPropagationPointIndexer::determineGrid(DefoPointCollection* points, const DefoPoint* seed,
                                                double& dx, double& dy, double& d) {

    double tdx, tdy, td, mindx = 1.e6, mindy = 1.e6, mind = 1.e6;
    for (DefoPointCollection::iterator it = points->begin();
         it != points->end();
         ++it) {
        if (it->getX()==seed->getX() && it->getY()==seed->getY()) continue;

      td = seed->getDistanceXY(*it, tdx, tdy);
      if (std::fabs(tdx)<20.0 || std::fabs(tdy)<20.0) continue;
      if (std::fabs(tdx)<std::fabs(mindx) && std::fabs(tdy)<std::fabs(mindy)) {
        mindx = tdx;
        mindy = tdy;
        mind = td;
      }
    }

    dx = std::fabs(mindx);
    dy = std::fabs(mindy);
    d = mind;
}

DefoPoint * DefoPropagationPointIndexer::findNeighbour(DefoPointCollection* points, const DefoPoint* seed,
                                                       Axis axis, Direction direction, double window) {

    double min = 1.e6;
    double dx, dy, d;
    DefoPoint* nextPoint = 0;
    for (DefoPointCollection::iterator it = points->begin();
         it != points->end();
         ++it) {
        if (it->getX()==seed->getX() && it->getY()==seed->getY()) continue;

        d = seed->getDistanceXY(*it, dx, dy);

        if (axis==X) {
            if (std::fabs(dy)>window) continue;
            if (direction==Forward && dx>0) continue;
            if (direction==Backward && dx<0) continue;
            if (std::fabs(dx)<std::fabs(min)) {
                min = dx;
                nextPoint = &(*it);
            }
        } else {
            if (std::fabs(dx)>window) continue;
            if (direction==Forward && dy>0) continue;
            if (direction==Backward && dy<0) continue;
            if (std::fabs(dy)<std::fabs(min)) {
                min = dy;
                nextPoint = &(*it);
            }
        }
    }

    return nextPoint;
}
