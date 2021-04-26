#ifndef TIMELINECONTROLITEMOFADBOARDGROUP_H
#define TIMELINECONTROLITEMOFADBOARDGROUP_H

#include "timelinecontrolitem.h"

class TimeLineControlItemOfADBoardGroup : public TimeLineControlItem
{
public:
    TimeLineControlItemOfADBoardGroup();

    bool motionTypeTriggerPowerAsDisplayMode;
    bool motionTypeHoldPowerAsDisplayMode;
    bool motionTypeBasePowerAsDisplayMode;

    TimeLineControlItem *copyAll();
    void loadConfigData(TimeLineControlItem *controlItem);

    NodeGroup::GroupType groupType();

    void controlDataFromJson(QJsonObject *controlDataJson);
    void controlDataToJson(QJsonObject *controlDataJson);
    void controlDataToPropertyPairList(QList<QPair<QString, QString> > *propertyPairList);
    void controlDataFromPropertyPairList(QMap<QString, QString> *propertyMap);
};


#endif // TIMELINECONTROLITEMOFADBOARDGROUP_H
