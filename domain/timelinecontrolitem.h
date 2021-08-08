#ifndef TIMELINECONTROLITEM_H
#define TIMELINECONTROLITEM_H

#include <QString>
#include "timelinecontrolpolicy.h"
#include "basetimelinecontrolitem.h"
#include "nodegroup.h"
#include <QPair>

class QSqlQuery;

class TimeLineControlItem : public QObject
{
public:
    TimeLineControlItem();

    int id;
    int policySubId;
    quint8 groupId;

    QString templatePolicyId; //For template use only
    QString templateItemId;

    int startMinute;
    QString controlType;

    quint8 powerTypePower;

    bool motionTrigPowerByLuxSensor;
    quint8 motionTypeTriggerPower;
    quint8 motionTypeHoldPower;
    quint8 motionTypeBasePower;
    int motionTypeTriggerTime;
    int motionTypeHoldTime;
    QString motionTypeBindSensorId;

    bool luxTypeIsFeedBack;
    QString luxTypeBindSensorId;
    int luxTypeFeedBackLux;
    int luxTypeFeedBackFloatLux;
    int luxTypeFeedBackStepPower;
    int luxTypeNotFeedBackLuxOf0;
    int luxTypeNotFeedBackLuxOf100;

    bool gasTrnsdcrTypeTrigTypeOn;
    bool gasTrnsdcrTypeTrigTypeTimer;
    bool gasTrnsdcrTypeTrigModeUpperBottom;
    bool gasTrnsdcrTypeTrigModeBottomUpper;
    double gasTrnsdcrTypeMaxThreshold;
    double gasTrnsdcrTypeMinThreshold;
    int gasTrnsdcrTypeTrigHoldTime;
    int gasTrnsdcrTypeTrigShieldMSec;
    bool gasTrnsdcrTypeTrigBaseStatusOn;
    bool gasTrnsdcrTypeTrigBaseStatusOff;
    QString gasTrnsdcrTypeBindSensorId;

    int dimmingIntervalSec;
    int motionTypeTriggerShieldMSec;

    //eg: CO.125>50&&TH.123[TEMP]<30&&FOR.105>5||HW.125==1&&RTH.101[VOL]<3.5
    //Supported Operator: `>`, `<` and `==`
    QString compositeSensorTypeCmpExpr;
    int compositeSensorTypeTrigShieldMSec;

    virtual TimeLineControlItem *copyAll()=0;
    virtual void loadConfigData(TimeLineControlItem *controlItem);
    virtual void loadAll(TimeLineControlItem *controlItem);
    virtual NodeGroup::GroupType groupType()=0;

    virtual void controlDataFromJson(QJsonObject *controlDataJson);
    virtual void controlDataToJson(QJsonObject *controlDataJson);
    virtual void controlDataToPropertyPairList(QList<QPair<QString, QString> > *propertyPairList);
    virtual void controlDataFromPropertyPairList(QMap<QString, QString> *propertyMap);

private:

};

#endif // TIMELINECONTROLITEM_H
