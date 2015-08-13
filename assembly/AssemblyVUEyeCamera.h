#ifndef ASSEMBLYVUEYECAMERA_H
#define ASSEMBLYVUEYECAMERA_H

#include <QObject>

class AssemblyVUEyeCamera : public QObject
{
    Q_OBJECT
public:
    explicit AssemblyVUEyeCamera(QObject *parent);
    ~AssemblyVUEyeCamera();

public slots:

    virtual void aquireImage() = 0;

protected slots:

protected:

signals:

private:

};

#endif // ASSEMBLYVUEYECAMERA_H
