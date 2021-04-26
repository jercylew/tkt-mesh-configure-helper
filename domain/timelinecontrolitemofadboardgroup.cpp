#include "timelinecontrolitemofadboardgroup.h"
#include <QJsonObject>

TimeLineControlItemOfADBoardGroup::TimeLineControlItemOfADBoardGroup()
{
    motionTypeTriggerPowerAsDisplayMode=0;
    motionTypeHoldPowerAsDisplayMode=0;
    motionTypeBasePowerAsDisplayMode=0;
}

TimeLineControlItem *TimeLineControlItemOfADBoardGroup::copyAll()
{
    TimeLineControlItemOfADBoardGroup *newControlItem=new TimeLineControlItemOfADBoardGroup;
    newControlItem->loadAll(this);
    return newControlItem;
}

void TimeLineControlItemOfADBoardGroup::loadConfigData(TimeLineControlItem *controlItem)
{
    TimeLineControlItem::loadConfigData(controlItem);
    TimeLineControlItemOfADBoardGroup *item=static_cast<TimeLineControlItemOfADBoardGroup*>(controlItem);
    if(item!=NULL)
    {
        this->motionTypeTriggerPowerAsDisplayMode=item->motionTypeTriggerPowerAsDisplayMode;
        this->motionTypeHoldPowerAsDisplayMode=item->motionTypeHoldPowerAsDisplayMode;
        this->motionTypeBasePowerAsDisplayMode=item->motionTypeBasePowerAsDisplayMode;
    }
}

NodeGroup::GroupType TimeLineControlItemOfADBoardGroup::groupType()
{
    return NodeGroup::ADBoardGroup;
}

void TimeLineControlItemOfADBoardGroup::controlDataFromJson(QJsonObject *controlDataJson)
{
    TimeLineControlItem::controlDataFromJson(controlDataJson);

    motionTypeTriggerPowerAsDisplayMode = (controlDataJson->value("motion_type_trigger_power_as_display_mode").toString()=="true"?true:false);
    motionTypeHoldPowerAsDisplayMode = (controlDataJson->value("motion_type_hold_power_as_display_mode").toString()=="true"?true:false);
    motionTypeBasePowerAsDisplayMode = (controlDataJson->value("motion_type_base_power_as_display_mode").toString()=="true"?true:false);
}

void TimeLineControlItemOfADBoardGroup::controlDataToJson(QJsonObject *controlDataJson)
{
    TimeLineControlItem::controlDataToJson(controlDataJson);

    controlDataJson->insert("motion_type_trigger_power_as_display_mode", (motionTypeTriggerPowerAsDisplayMode?"true":"false"));
    controlDataJson->insert("motion_type_hold_power_as_display_mode", (motionTypeHoldPowerAsDisplayMode?"true":"false"));
    controlDataJson->insert("motion_type_base_power_as_display_mode", (motionTypeBasePowerAsDisplayMode?"true":"false"));
}

void TimeLineControlItemOfADBoardGroup::controlDataToPropertyPairList(QList<QPair<QString, QString> > *propertyPairList)
{
    TimeLineControlItem::controlDataToPropertyPairList(propertyPairList);

    propertyPairList->push_back(QPair<QString, QString>("motion_type_trigger_power_as_display_mode",
                                                        (motionTypeTriggerPowerAsDisplayMode?"true":"false")));
    propertyPairList->push_back(QPair<QString, QString>("motion_type_hold_power_as_display_mode",
                                                        (motionTypeHoldPowerAsDisplayMode?"true":"false")));
    propertyPairList->push_back(QPair<QString, QString>("motion_type_base_power_as_display_mode",
                                                        (motionTypeBasePowerAsDisplayMode?"true":"false")));
}

void TimeLineControlItemOfADBoardGroup::controlDataFromPropertyPairList(QMap<QString, QString> *propertyMap)
{
    TimeLineControlItem::controlDataFromPropertyPairList(propertyMap);

    motionTypeTriggerPowerAsDisplayMode = ((propertyMap->value("motion_type_trigger_power_as_display_mode")=="true")?true:false);
    motionTypeHoldPowerAsDisplayMode = ((propertyMap->value("motion_type_hold_power_as_display_mode")=="true")?true:false);
    motionTypeBasePowerAsDisplayMode = ((propertyMap->value("motion_type_base_power_as_display_mode")=="true")?true:false);
}
