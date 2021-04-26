#include "timelinecontrolitem.h"
#include <QSqlQuery>
#include <QJsonObject>

TimeLineControlItem::TimeLineControlItem()
{
    id=0;
    groupId=0;
    policySubId = 0;

    startMinute=0;
    controlType="power";
    powerTypePower=0;
    motionTrigPowerByLuxSensor=false;
    motionTypeTriggerPower=0;
    motionTypeHoldPower=0;
    motionTypeBasePower=0;
    motionTypeTriggerTime=0;
    motionTypeHoldTime=0;
    motionTypeBindSensorId="";

    luxTypeIsFeedBack=false;
    luxTypeBindSensorId="";
    luxTypeFeedBackLux=0;
    luxTypeFeedBackFloatLux=0;
    luxTypeFeedBackStepPower=0;
    luxTypeNotFeedBackLuxOf0=0;
    luxTypeNotFeedBackLuxOf100=0;

    gasTrnsdcrTypeTrigTypeOn = false;
    gasTrnsdcrTypeTrigTypeTimer = true;
    gasTrnsdcrTypeTrigModeUpperBottom = true;
    gasTrnsdcrTypeTrigModeBottomUpper = false;
    gasTrnsdcrTypeMaxThreshold = 0.0;
    gasTrnsdcrTypeMinThreshold = 0.0;
    gasTrnsdcrTypeTrigHoldTime = 0;
    gasTrnsdcrTypeTrigShieldMSec = 0;
    gasTrnsdcrTypeTrigBaseStatusOn = false;
    gasTrnsdcrTypeTrigBaseStatusOff = true;
    gasTrnsdcrTypeBindSensorId = "";

    dimmingIntervalSec=10;
    motionTypeTriggerShieldMSec = 0;
}

void TimeLineControlItem::loadConfigData(TimeLineControlItem *controlItem)
{
    controlType=controlItem->controlType;

    powerTypePower=controlItem->powerTypePower;

    motionTrigPowerByLuxSensor=controlItem->motionTrigPowerByLuxSensor;
    motionTypeTriggerPower=controlItem->motionTypeTriggerPower;
    motionTypeHoldPower=controlItem->motionTypeHoldPower;
    motionTypeBasePower=controlItem->motionTypeBasePower;
    motionTypeTriggerTime=controlItem->motionTypeTriggerTime;
    motionTypeHoldTime=controlItem->motionTypeHoldTime;
    motionTypeBindSensorId=controlItem->motionTypeBindSensorId;

    luxTypeIsFeedBack=controlItem->luxTypeIsFeedBack;
    luxTypeBindSensorId=controlItem->luxTypeBindSensorId;
    luxTypeFeedBackLux=controlItem->luxTypeFeedBackLux;
    luxTypeFeedBackFloatLux=controlItem->luxTypeFeedBackFloatLux;
    luxTypeFeedBackStepPower=controlItem->luxTypeFeedBackStepPower;
    luxTypeNotFeedBackLuxOf0=controlItem->luxTypeNotFeedBackLuxOf0;
    luxTypeNotFeedBackLuxOf100=controlItem->luxTypeNotFeedBackLuxOf100;

    gasTrnsdcrTypeTrigTypeOn = controlItem->gasTrnsdcrTypeTrigTypeOn;
    gasTrnsdcrTypeTrigTypeTimer = controlItem->gasTrnsdcrTypeTrigTypeTimer;
    gasTrnsdcrTypeTrigModeUpperBottom = controlItem->gasTrnsdcrTypeTrigModeUpperBottom;
    gasTrnsdcrTypeTrigModeBottomUpper = controlItem->gasTrnsdcrTypeTrigModeBottomUpper;
    gasTrnsdcrTypeMaxThreshold = controlItem->gasTrnsdcrTypeMaxThreshold;
    gasTrnsdcrTypeMinThreshold = controlItem->gasTrnsdcrTypeMinThreshold;
    gasTrnsdcrTypeTrigHoldTime = controlItem->gasTrnsdcrTypeTrigHoldTime;
    gasTrnsdcrTypeTrigShieldMSec = controlItem->gasTrnsdcrTypeTrigShieldMSec;
    gasTrnsdcrTypeTrigBaseStatusOn = controlItem->gasTrnsdcrTypeTrigBaseStatusOn;
    gasTrnsdcrTypeTrigBaseStatusOff = controlItem->gasTrnsdcrTypeTrigBaseStatusOff;
    gasTrnsdcrTypeBindSensorId = controlItem->gasTrnsdcrTypeBindSensorId;

    dimmingIntervalSec=controlItem->dimmingIntervalSec;
    motionTypeTriggerShieldMSec=controlItem->motionTypeTriggerShieldMSec;
}

void TimeLineControlItem::loadAll(TimeLineControlItem *controlItem)
{
    id = controlItem->id;
    policySubId = controlItem->policySubId;
    groupId = controlItem->groupId;

    templateItemId = controlItem->templateItemId;
    templatePolicyId = controlItem->templatePolicyId;

    startMinute = controlItem->startMinute;
    loadConfigData(controlItem);
}

void TimeLineControlItem::controlDataFromJson(QJsonObject *controlDataJson)
{
    startMinute = controlDataJson->value("start_minute").toInt();
    controlType = controlDataJson->value("control_type").toString();

    //Power
    powerTypePower=(quint8)(controlDataJson->value("power_type_power").toInt());

    //Motion Sensor
    motionTrigPowerByLuxSensor=(controlDataJson->value("motion_type_trigger_power").toInt()>=1000);
    motionTypeTriggerPower=(quint8)(controlDataJson->value("motion_type_trigger_power").toInt()%1000);
    motionTypeHoldPower=(quint8)(controlDataJson->value("motion_type_hold_power").toInt());
    motionTypeBasePower=(quint8)(controlDataJson->value("motion_type_base_power").toInt());
    motionTypeTriggerTime=controlDataJson->value("motion_type_trigger_time").toInt();
    motionTypeHoldTime=controlDataJson->value("motion_type_hold_time").toInt();
    motionTypeBindSensorId=controlDataJson->value("motion_type_bind_sensor_id").toString();

    //Lux Sensor
    luxTypeIsFeedBack=((controlDataJson->value("lux_type_is_feed_back").toInt()==1)?true:false);
    luxTypeBindSensorId=controlDataJson->value("lux_type_bind_sensor_id").toString();
    luxTypeFeedBackLux=controlDataJson->value("lux_type_feed_back_lux").toInt();
    luxTypeFeedBackFloatLux=controlDataJson->value("lux_type_feed_back_float_lux").toInt();
    luxTypeFeedBackStepPower=controlDataJson->value("lux_type_feed_back_step_power").toInt();
    luxTypeNotFeedBackLuxOf0=controlDataJson->value("lux_type_not_feed_back_lux_of_0").toInt();
    luxTypeNotFeedBackLuxOf100=controlDataJson->value("lux_type_not_feed_back_lux_of_100").toInt();

    //Gas Transducer
    gasTrnsdcrTypeTrigTypeOn = controlDataJson->value("gas_trnsdcr_type_trig_type_on").toBool();
    gasTrnsdcrTypeTrigTypeTimer = controlDataJson->value("gas_trnsdcr_type_trig_type_timer").toBool();
    gasTrnsdcrTypeTrigModeUpperBottom = controlDataJson->value("gas_trnsdcr_type_trig_mode_upper_bottom").toBool();
    gasTrnsdcrTypeTrigModeBottomUpper = controlDataJson->value("gas_trnsdcr_type_trig_mode_bottom_upper").toBool();
    gasTrnsdcrTypeMaxThreshold = controlDataJson->value("gas_trnsdcr_type_max_threshold").toDouble();
    gasTrnsdcrTypeMinThreshold = controlDataJson->value("gas_trnsdcr_type_min_threshold").toDouble();
    gasTrnsdcrTypeTrigHoldTime = controlDataJson->value("gas_trnsdcr_type_trig_hold_time").toInt();
    gasTrnsdcrTypeTrigShieldMSec = controlDataJson->value("gas_trnsdcr_type_trig_shield_msec").toInt();
    gasTrnsdcrTypeTrigBaseStatusOn = controlDataJson->value("gas_trnsdcr_type_trig_base_status_on").toBool();
    gasTrnsdcrTypeTrigBaseStatusOff = controlDataJson->value("gas_trnsdcr_type_trig_base_status_off").toBool();
    gasTrnsdcrTypeBindSensorId = controlDataJson->value("gas_trnsdcr_type_bind_sensor_id").toString();

    //Extra
    dimmingIntervalSec=controlDataJson->value("dim_interval_sec").toInt();
    if(dimmingIntervalSec<2)
    {
        dimmingIntervalSec=2;
    }
    motionTypeTriggerShieldMSec=controlDataJson->value("motion_type_trigger_shield_msec").toInt();
    if(motionTypeTriggerShieldMSec<0)
    {
        motionTypeTriggerShieldMSec=0;
    }
}

void TimeLineControlItem::controlDataToJson(QJsonObject *controlDataJson)
{
    controlDataJson->insert("start_minute", startMinute);
    controlDataJson->insert("control_type", controlType);

    //Power
    controlDataJson->insert("power_type_power", powerTypePower);

    //Motion Sensor
    controlDataJson->insert("motion_type_trigger_power", motionTrigPowerByLuxSensor ?
                                ((quint16)motionTypeTriggerPower+1000) : (quint16)motionTypeTriggerPower);
    controlDataJson->insert("motion_type_hold_power", motionTypeHoldPower);
    controlDataJson->insert("motion_type_base_power", motionTypeBasePower);
    controlDataJson->insert("motion_type_trigger_time", motionTypeTriggerTime);
    controlDataJson->insert("motion_type_hold_time", motionTypeHoldTime);
    controlDataJson->insert("motion_type_bind_sensor_id", motionTypeBindSensorId);

    //Lux Sensor
    controlDataJson->insert("lux_type_is_feed_back", luxTypeIsFeedBack?1:0);
    controlDataJson->insert("lux_type_bind_sensor_id", luxTypeBindSensorId);
    controlDataJson->insert("lux_type_feed_back_lux", luxTypeFeedBackLux);
    controlDataJson->insert("lux_type_feed_back_float_lux", luxTypeFeedBackFloatLux);
    controlDataJson->insert("lux_type_feed_back_step_power", luxTypeFeedBackStepPower);
    controlDataJson->insert("lux_type_not_feed_back_lux_of_0", luxTypeNotFeedBackLuxOf0);
    controlDataJson->insert("lux_type_not_feed_back_lux_of_100", luxTypeNotFeedBackLuxOf100);

    //Gas Transducer
    controlDataJson->insert("gas_trnsdcr_type_trig_type_on", gasTrnsdcrTypeTrigTypeOn);
    controlDataJson->insert("gas_trnsdcr_type_trig_type_timer", gasTrnsdcrTypeTrigTypeTimer);
    controlDataJson->insert("gas_trnsdcr_type_trig_mode_upper_bottom", gasTrnsdcrTypeTrigModeUpperBottom);
    controlDataJson->insert("gas_trnsdcr_type_trig_mode_bottom_upper", gasTrnsdcrTypeTrigModeBottomUpper);
    controlDataJson->insert("gas_trnsdcr_type_max_threshold", gasTrnsdcrTypeMaxThreshold);
    controlDataJson->insert("gas_trnsdcr_type_min_threshold", gasTrnsdcrTypeMinThreshold);
    controlDataJson->insert("gas_trnsdcr_type_trig_hold_time", gasTrnsdcrTypeTrigHoldTime);
    controlDataJson->insert("gas_trnsdcr_type_trig_shield_msec", gasTrnsdcrTypeTrigShieldMSec);
    controlDataJson->insert("gas_trnsdcr_type_trig_base_status_on", gasTrnsdcrTypeTrigBaseStatusOn);
    controlDataJson->insert("gas_trnsdcr_type_trig_base_status_off", gasTrnsdcrTypeTrigBaseStatusOff);
    controlDataJson->insert("gas_trnsdcr_type_bind_sensor_id", gasTrnsdcrTypeBindSensorId);

    //Extra
    controlDataJson->insert("dim_interval_sec", dimmingIntervalSec);
    controlDataJson->insert("motion_type_trigger_shield_msec", motionTypeTriggerShieldMSec);
}

void TimeLineControlItem::controlDataToPropertyPairList(QList<QPair<QString, QString> > *propertyPairList)
{
    propertyPairList->push_back(QPair<QString, QString>("power_type_power", QString::number(powerTypePower&0xFF)));

    propertyPairList->push_back(QPair<QString, QString>("motion_type_trigger_power_by_luxsensor", motionTrigPowerByLuxSensor?"true":"false"));
    propertyPairList->push_back(QPair<QString, QString>("motion_type_trigger_power", QString::number(motionTypeTriggerPower&0xFF)));
    propertyPairList->push_back(QPair<QString, QString>("motion_type_hold_power", QString::number(motionTypeHoldPower&0xFF)));
    propertyPairList->push_back(QPair<QString, QString>("motion_type_base_power", QString::number(motionTypeBasePower&0xFF)));
    propertyPairList->push_back(QPair<QString, QString>("motion_type_trigger_time", QString::number(motionTypeTriggerTime)));
    propertyPairList->push_back(QPair<QString, QString>("motion_type_hold_time", QString::number(motionTypeHoldTime)));
    propertyPairList->push_back(QPair<QString, QString>("motion_type_bind_sensor_id", motionTypeBindSensorId));

    propertyPairList->push_back(QPair<QString, QString>("lux_type_is_feed_back", luxTypeIsFeedBack?"true":"false"));
    propertyPairList->push_back(QPair<QString, QString>("lux_type_bind_sensor_id", luxTypeBindSensorId));
    propertyPairList->push_back(QPair<QString, QString>("lux_type_feed_back_lux", QString::number(luxTypeFeedBackLux)));
    propertyPairList->push_back(QPair<QString, QString>("lux_type_feed_back_float_lux", QString::number(luxTypeFeedBackFloatLux)));
    propertyPairList->push_back(QPair<QString, QString>("lux_type_feed_back_step_power", QString::number(luxTypeFeedBackStepPower)));
    propertyPairList->push_back(QPair<QString, QString>("lux_type_not_feed_back_lux_of_0", QString::number(luxTypeNotFeedBackLuxOf0)));
    propertyPairList->push_back(QPair<QString, QString>("lux_type_not_feed_back_lux_of_100", QString::number(luxTypeNotFeedBackLuxOf100)));

    propertyPairList->push_back(QPair<QString, QString>("gas_trnsdcr_type_trig_type_on", gasTrnsdcrTypeTrigTypeOn?"true":"false"));
    propertyPairList->push_back(QPair<QString, QString>("gas_trnsdcr_type_trig_type_timer", gasTrnsdcrTypeTrigTypeTimer?"true":"false"));
    propertyPairList->push_back(QPair<QString, QString>("gas_trnsdcr_type_trig_mode_upper_bottom", gasTrnsdcrTypeTrigModeUpperBottom?"true":"false"));
    propertyPairList->push_back(QPair<QString, QString>("gas_trnsdcr_type_trig_mode_bottom_upper", gasTrnsdcrTypeTrigModeBottomUpper?"true":"false"));
    propertyPairList->push_back(QPair<QString, QString>("gas_trnsdcr_type_max_threshold", QString::number(gasTrnsdcrTypeMaxThreshold)));
    propertyPairList->push_back(QPair<QString, QString>("gas_trnsdcr_type_min_threshold", QString::number(gasTrnsdcrTypeMinThreshold)));
    propertyPairList->push_back(QPair<QString, QString>("gas_trnsdcr_type_trig_hold_time", QString::number(gasTrnsdcrTypeTrigHoldTime)));
    propertyPairList->push_back(QPair<QString, QString>("gas_trnsdcr_type_trig_shield_msec", QString::number(gasTrnsdcrTypeTrigShieldMSec)));
    propertyPairList->push_back(QPair<QString, QString>("gas_trnsdcr_type_trig_base_status_on", gasTrnsdcrTypeTrigBaseStatusOn?"true":"false"));
    propertyPairList->push_back(QPair<QString, QString>("gas_trnsdcr_type_trig_base_status_off", gasTrnsdcrTypeTrigBaseStatusOff?"true":"false"));
    propertyPairList->push_back(QPair<QString, QString>("gas_trnsdcr_type_bind_sensor_id", gasTrnsdcrTypeBindSensorId));

    propertyPairList->push_back(QPair<QString, QString>("dimming_interval_sec", QString::number(dimmingIntervalSec)));
    propertyPairList->push_back(QPair<QString, QString>("motion_type_trigger_shield_msec", QString::number(motionTypeTriggerShieldMSec)));
}

void TimeLineControlItem::controlDataFromPropertyPairList(QMap<QString, QString> *propertyMap)
{
    powerTypePower = (quint8)(propertyMap->value("power_type_power").toInt());

    motionTypeTriggerPower=(quint8)(propertyMap->value("motion_type_trigger_power").toInt()%1000);
    motionTypeHoldPower=(quint8)(propertyMap->value("motion_type_hold_power").toInt());
    motionTypeBasePower=(quint8)(propertyMap->value("motion_type_base_power").toInt());
    motionTypeTriggerTime=propertyMap->value("motion_type_trigger_time").toInt();
    motionTypeHoldTime=propertyMap->value("motion_type_hold_time").toInt();
    motionTypeBindSensorId=propertyMap->value("motion_type_bind_sensor_id");
    motionTrigPowerByLuxSensor=(propertyMap->value("motion_type_trigger_power_by_luxsensor")=="true"?true:false);

    luxTypeIsFeedBack=((propertyMap->value("lux_type_is_feed_back")=="true")?true:false);
    luxTypeBindSensorId=propertyMap->value("lux_type_bind_sensor_id");
    luxTypeFeedBackLux=propertyMap->value("lux_type_feed_back_lux").toInt();
    luxTypeFeedBackFloatLux=propertyMap->value("lux_type_feed_back_float_lux").toInt();
    luxTypeFeedBackStepPower=propertyMap->value("lux_type_feed_back_step_power").toInt();
    luxTypeNotFeedBackLuxOf0=propertyMap->value("lux_type_not_feed_back_lux_of_0").toInt();
    luxTypeNotFeedBackLuxOf100=propertyMap->value("lux_type_not_feed_back_lux_of_100").toInt();

    gasTrnsdcrTypeTrigTypeOn = (propertyMap->value("gas_trnsdcr_type_trig_type_on") == "true") ? true : false;
    gasTrnsdcrTypeTrigTypeTimer = (propertyMap->value("gas_trnsdcr_type_trig_type_timer") == "true") ? true : false;
    gasTrnsdcrTypeTrigModeUpperBottom = (propertyMap->value("gas_trnsdcr_type_trig_mode_upper_bottom") == "true") ? true : false;
    gasTrnsdcrTypeTrigModeBottomUpper = (propertyMap->value("gas_trnsdcr_type_trig_mode_bottom_upper") == "true") ? true : false;
    gasTrnsdcrTypeMaxThreshold = propertyMap->value("gas_trnsdcr_type_max_threshold").toDouble();
    gasTrnsdcrTypeMinThreshold = propertyMap->value("gas_trnsdcr_type_min_threshold").toDouble();
    gasTrnsdcrTypeTrigHoldTime = propertyMap->value("gas_trnsdcr_type_trig_hold_time").toInt();
    gasTrnsdcrTypeTrigShieldMSec = propertyMap->value("gas_trnsdcr_type_trig_shield_msec").toInt();
    gasTrnsdcrTypeTrigBaseStatusOn = (propertyMap->value("gas_trnsdcr_type_trig_base_status_on") == "true") ? true : false;
    gasTrnsdcrTypeTrigBaseStatusOff = (propertyMap->value("gas_trnsdcr_type_trig_base_status_off") == "true") ? true : false;
    gasTrnsdcrTypeBindSensorId = propertyMap->value("gas_trnsdcr_type_bind_sensor_id");

    dimmingIntervalSec=propertyMap->value("dimming_interval_sec").toInt();
    motionTypeTriggerShieldMSec=propertyMap->value("motion_type_trigger_shield_msec").toInt();
}

