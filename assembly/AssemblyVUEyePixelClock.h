#ifndef ASSEMBLYVUEYEPIXELCLOCK_H
#define ASSEMBLYVUEYEPIXELCLOCK_H

#include <vector>

#include <QObject>

class AssemblyVUEyePixelClock : public QObject
{
    Q_OBJECT

public:
    explicit AssemblyVUEyePixelClock(QObject *parent);
    ~AssemblyVUEyePixelClock();

    size_t getNumberOfPixelClocks() const { return pixelClocks_.size(); }
    const std::vector<unsigned int>& getPixelClockList() const { return pixelClocks_; }
    unsigned int getCurrentPixelClock() const;

public slots:

    virtual void updatePixelClocks(unsigned int, bool) = 0;

protected slots:

protected:

    std::vector<unsigned int> pixelClocks_;
    size_t currentPixelClockIndex_;

    size_t getCurrentPixelClockIndex(unsigned int pixelClock) const;

signals:

    void pixelClockChanged(unsigned int);
    void pixelClockListChanged(unsigned int);
};

#endif // ASSEMBLYVUEYEPIXELCLOCK_H
