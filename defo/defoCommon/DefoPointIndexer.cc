#include "DefoPointIndexer.h"

DefoPointIndexer::DefoPointIndexer(QObject *parent) :
    DefoVPointIndexer(parent)
{
  // TODO
}

void DefoPointIndexer::indexPoints(DefoPointCollection *points, const QColor& seedColor) {

  // Index x axis
  indexPointsAxis(points, seedColor, DefoPoint::X);
  // ... and y axis
  indexPointsAxis(points, seedColor, DefoPoint::Y);
}

void DefoPointIndexer::indexPointsAxis(
    DefoPointCollection* points
  , const QColor& seedColor
  , const DefoPoint::Axis& axis
) {

  // Fills bins along axis
  fillBins(points, seedColor, axis);

  // Find reference point
  QList<DefoPointBin>::iterator referenceBin = bins_[axis].begin();
  while ( referenceBin < bins_[axis].end() && !referenceBin->hasReferencePoint() )
    ++referenceBin;

  // FIXME Decent solution to no reference found
  if ( referenceBin == bins_[axis].end() )
    throw;

  // Index all bins (and points)
  for (
      QList<DefoPointBin>::iterator it = bins_[axis].begin()
    ; it < bins_[axis].end()
    ; ++it
  )
    it->setIndex( it - referenceBin );

}

/**
  Fills the bins with points in the given collection according to their
  positions along the given axis.
  */
void DefoPointIndexer::fillBins(
    DefoPointCollection* points
  , const QColor& seedColor
  , const DefoPoint::Axis& axis
) {

  // Create bin list and position cache
  QList<DefoPointBin> bins;
  QList<double> binPositions;

  int referenceBinIndex = -1;

  // Loop over all points
  for ( DefoPointCollection::iterator it = points->begin()
      ; it < points->end()
      ; ++it
  ) {

    // FIXME clean up the reference/derefenence mess
    DefoPoint* point = &(*it);

    // Find matching bin or create new bin
    int i = 0;
    while (
           i < bins.size()
        && std::abs( point->getX() - binPositions[i] ) > maxSpacing_
    )
      ++i;

    if ( i == bins.size() ) {
      // reached end without finding bin, create new
      bins.push_back( DefoPointBin( axis, point ) );
      binPositions.push_back( bins[i].getMean() );
    }
    else {
      // matching bin was found
      bins[i].addPoint( point );
      binPositions[i] = bins[i].getMean();
    }


    // Should only occur once!
    // FIXME decent reference point recognition
    if ( point->hasReferenceColor(seedColor) )
      referenceBinIndex = i;

  }

  // Keep track of reference bin
  bins[referenceBinIndex].setHasReferencePoint(true);

  // Sort bins
  qSort( bins.begin(), bins.end(), binPrecedes );

  // Store the filled bins
  bins_[axis] = bins;

}

/// Returns left.getMean() < right.getMean()
bool DefoPointIndexer::binPrecedes(
    const DefoPointBin &left
  , const DefoPointBin &right
) {
  return left.getMean() < right.getMean();
}
