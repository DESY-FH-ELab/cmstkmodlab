#ifndef LABCONTROLCOMMAND_H
#define LABCONTROLCOMMAND_H

class LabControlCommand
{
public:
    LabControlCommand(unsigned int time,
                      int type,
                      float parameter);

    enum CommandTypes {
       cmdSetTemp = 0,
       cmdSetPIDP,
       cmdSetPIDTv,
       cmdSetPIDTn,
       cmdCirculatorON,
       cmdCirculatorOFF,
       cmdStartDAQ,
       cmdStopDAQ,
       isBathTempBelow,
       isBathTempAbove,
       cmdSetU1,
       cmdSetI1,
       cmdSetU2,
       cmdSetI2,
       cmdVoltageON,
       cmdVoltageOFF,
       cmdMax
    };

    unsigned int time_;
    int type_;
    float parameter_;
};

inline bool operator<(const LabControlCommand &s1, const LabControlCommand &s2)
    { return (s1.time_ < s2.time_); }

#endif // LABCONTROLCOMMAND_H
