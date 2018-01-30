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

#ifndef ASSEMBLYASSEMBLER_H
#define ASSEMBLYASSEMBLER_H

#include <LStepExpressMotionManager.h>

#include <QObject>

class AssemblyAssembler : public QObject
{
 Q_OBJECT

  public:
    explicit AssemblyAssembler(const LStepExpressMotionManager*, QObject* parent=nullptr);
    virtual ~AssemblyAssembler() {}

    void reset();

  protected:

    const LStepExpressMotionManager* motion_manager_;

    bool motion_manager_enabled_;

    void enable_motion_manager(const bool);

    void    connect_motion_manager() { this->enable_motion_manager(true) ; }
    void disconnect_motion_manager() { this->enable_motion_manager(false); }

    double object_deltaX_;
    double object_deltaY_;
    double target_angle_deg_;

    double angle_max_dontIter_;
    double angle_max_complete_;

    int alignment_step;

    double posi_x1_, posi_y1_;
    double posi_x2_, posi_y2_;

    double obj_angle_deg_;

    bool only_measure_ang_;

  public slots:

    void start_alignment(const double, const double);
    void start_alignment(const double, const double, const double);

    void run_alignment(int, double, double, double);

    void launch_next_alignment_step();

    void moveRelative(const double, const double, const double, const double);

    void finish_motion();

  signals:

    void nextAlignmentStep(int, double, double, double);

    void move_relative(const double, const double, const double, const double);
    void motion_finished();

    void acquireImage();

    void object_angle(const double);

    void alignment_finished();
};

#endif // ASSEMBLYASSEMBLER_H
