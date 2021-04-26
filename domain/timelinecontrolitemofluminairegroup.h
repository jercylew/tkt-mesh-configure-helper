#ifndef TIMELINECONTROLITEMOFLUMINAIREGROUP_H
#define TIMELINECONTROLITEMOFLUMINAIREGROUP_H

#include "timelinecontrolitem.h"

class TimeLineControlItemOfLuminaireGroup : public TimeLineControlItem
{
public:
    TimeLineControlItemOfLuminaireGroup();

    TimeLineControlItem *copyAll();
    NodeGroup::GroupType groupType();
};

#endif // TIMELINECONTROLITEMOFLUMINAIREGROUP_H
