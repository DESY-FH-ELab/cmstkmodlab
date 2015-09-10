#ifndef DEFOKEITHLEYMODEL_H
#define DEFOKEITHLEYMODEL_H

#include <KeithleyModel.h>

class DefoKeithleyModel : public KeithleyModel
{
    Q_OBJECT

public:
 
  explicit DefoKeithleyModel(const char* port,
			     int updateInterval = 60,
			     QObject *parent = 0);

protected slots:

  void scanTemperatures();
};

#endif // DEFOKEITHLEYMODEL_H
