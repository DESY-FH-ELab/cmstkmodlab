/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef DEFOPOINTBIN_H
#define DEFOPOINTBIN_H

#include <vector>
#include "DefoPoint.h"

/**
  \brief Point binning class.
  \par After point recognition on the raw image, the points have to be indexed.
  This has to be performed by binning the points into distint groups. These
  groups will then later form a spline along the x or y axis.
  \par After the bins have been filled, an index can be assigned to all the
  points via setBinIndex. As indexing only requires sorting of the bins, no
  point sorting has to be performed.
  */
class DefoPointBin {

public:
    DefoPointBin(
        const DefoPoint::Axis& axis
      , DefoPoint* initialPoint
    );
    DefoPointBin& operator=(const DefoPointBin& pointBin);
    void addPoint(DefoPoint* point);

    typedef std::vector<DefoPoint*>::const_iterator const_iterator;
    const_iterator begin() const;
    const_iterator end() const;

    double getMin() const;
    double getMax() const;
    double getMean() const;

    void setHasReferencePoint(bool hasReference);
    bool hasReferencePoint() const;

    void setIndex(int index);

protected:
    const DefoPoint::Axis axis_;
    std::vector<DefoPoint*> points_;

    bool hasReferencePoint_;

    double minValue_;
    double maxValue_;

    // For calculation of the mean
    double totalValue_;

};

#endif // DEFOPOINTBIN_H
