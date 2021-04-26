#include "luminaire.h"

Luminaire::Luminaire()
{
    brightness=0;
}

void Luminaire::setBrightness(int brightness)
{
    this->brightness=brightness;
}

int Luminaire::getBrightness()
{
    if(this->brightness>=99)
    {
        return 100;
    }
    else
    {
        return this->brightness;
    }
}

void Luminaire::addToGroup(quint8 groupId)
{
    groupMap.insert(groupId, true);
}

void Luminaire::removeFromGroup(quint8 groupId)
{
    groupMap.insert(groupId, false);
}

bool Luminaire::isMemberOfGroup(quint8 groupId)
{
    if(!groupMap.contains(groupId))
    {
        return false;
    }
    return groupMap.value(groupId);
}

bool Luminaire::isLuminaire()
{
    return true;
}

