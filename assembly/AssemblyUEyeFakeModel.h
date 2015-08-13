#ifndef ASSEMBLYUEYEFAKEMODEL_H
#define ASSEMBLYUEYEFAKEMODEL_H

#include "AssemblyVUEyeModel.h"

class AssemblyUEyeFakeModel : public AssemblyVUEyeModel
{
    Q_OBJECT
public:
    explicit AssemblyUEyeFakeModel(int updateInterval = 60,
                                   QObject *parent = 0);
    ~AssemblyUEyeFakeModel();

public slots:

    void updateInformation();

protected slots:

protected:

};

#endif // ASSEMBLYUEYEFAKEMODEL_H
