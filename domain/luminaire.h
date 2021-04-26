#ifndef LUMINAIRE_H
#define LUMINAIRE_H

#include "executer.h"

class Luminaire : public Executer
{
public:
    Luminaire();

    void setBrightness(int brightness);
    int getBrightness();
    void addToGroup(quint8 groupId);
    void removeFromGroup(quint8 groupId);
    bool isMemberOfGroup(quint8 groupId);
    bool isLuminaire();

private:
    int brightness;
    QMap<quint8, bool> groupMap;  // <groupId, 是否属于该组>
};

#endif // LUMINAIRE_H
