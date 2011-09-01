#ifndef LABCONTROLPREFERENCES_H
#define LABCONTROLPREFERENCES_H

#include <string>
#include <utility>

class LabControlPreferences
{
public:
    LabControlPreferences();

    void read();

    const std::string & dataDirectory() const { return dataDirectory_; }
    const std::pair<float,float> calibration(unsigned int channel) const;

protected:

    std::string dataDirectory_;
    std::pair<float,float> calibration_[10];
};

#endif // LABCONTROLPREFERENCES_H
