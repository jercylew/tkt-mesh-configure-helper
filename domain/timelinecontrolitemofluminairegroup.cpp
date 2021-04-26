#include "timelinecontrolitemofluminairegroup.h"

TimeLineControlItemOfLuminaireGroup::TimeLineControlItemOfLuminaireGroup()
{

}

TimeLineControlItem *TimeLineControlItemOfLuminaireGroup::copyAll()
{
    TimeLineControlItemOfLuminaireGroup *newControlItem=new TimeLineControlItemOfLuminaireGroup;
    newControlItem->loadAll(this);
    return newControlItem;
}

NodeGroup::GroupType TimeLineControlItemOfLuminaireGroup::groupType()
{
    return NodeGroup::LuminaireGroup;
}


