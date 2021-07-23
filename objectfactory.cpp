#include "objectfactory.h"
#include <QMutexLocker>
#include "domain/luxsensor.h"
#include "domain/realmotionsensor.h"
#include "domain/virtualmotionsensor.h"
#include "domain/pmsensor.h"
#include "domain/co1sensor.h"
#include "domain/co2sensor.h"
#include "domain/realmotionandluxcamerasensor.h"
#include "domain/realswitchsensor.h"
#include "domain/realswitchsensor2ch.h"
#include "domain/virtualswitchsensor.h"
#include "domain/luminaire.h"
#include "domain/warninglight.h"
#include "mesh_define.h"
#include "domain/temperaturehumiditysensor.h"
#include "domain/stresssensor.h"
#include "domain/formaldehydesensor.h"
#include "domain/refrgtemperaturehumiditysensor.h"
#include "domain/uvlightsensor.h"
#include "domain/handwashingsensor.h"
#include "domain/ozonesensor.h"
#include "domain/flammablegassensor.h"

#include "domain/onechannelluminaire.h"
#include "domain/onechannelrelay.h"
#include "domain/adboard.h"
#include "domain/uvsterilizer.h"
#include "domain/drycontactrelay.h"
#include "domain/alarmloudspeaker.h"
#include <QDebug>

#include "domain/timelinecontrolitemofluminairegroup.h"
#include "domain/timelinecontrolitemofadboardgroup.h"
#include "domain/timelinecontrolitemofrelaygroup.h"

ObjectFactory *ObjectFactory::m_objectFactory=NULL;
QMutex ObjectFactory::m_objectFactoryMutex;

ObjectFactory *ObjectFactory::instance()
{
    if(m_objectFactory==NULL)
    {
        QMutexLocker locker(&m_objectFactoryMutex);
        if(m_objectFactory==NULL)
        {
            m_objectFactory=new ObjectFactory;
        }
    }
    return m_objectFactory;
}

bool ObjectFactory::isSensorNodeTypeCode(quint8 nodeTypeCode)
{
    return m_sensorNodeTypeSet.contains(nodeTypeCode);
}

bool ObjectFactory::isSensorUserDataTypeCode(quint8 userDataTypeCode)
{
    return m_sensorUserDataTypeCodeSet.contains(userDataTypeCode);
}

bool ObjectFactory::isExecuterNodeTypeCode(quint8 nodeTypeCode)
{
    return m_executerNodeTypeSet.contains(nodeTypeCode);
}

bool ObjectFactory::isExecuterUserDataTypeCode(quint8 userDataTypeCode)
{
    return m_executerUserDataTypeCodeSet.contains(userDataTypeCode);
}

Executer *ObjectFactory::createExecuterByTypeText(const QString &typeText)
{
    int id=QMetaType::type(typeText.toLatin1());
    if(id>0)
    {
        Executer *executer=static_cast<Executer*>(QMetaType::create(id));
        return executer;
    }
    else
    {
        return NULL;
    }
}

Executer *ObjectFactory::createExecuterByNodeTypeCode(quint8 nodeTypeCode)
{
    QString typeText=m_nodeTypeCodeToTypeTextMap.value(nodeTypeCode);
    if(typeText.isEmpty())
    {
        return NULL;
    }
    return createExecuterByTypeText(typeText);
}

Executer *ObjectFactory::createExecuterByNotifyDataCode(quint8 notifyCode)
{
    QString typeText=m_notifyCodeToTypeTextMap.value(notifyCode);
    if(typeText.isEmpty())
    {
        return NULL;
    }
    return createExecuterByTypeText(typeText);
}

TimeLineControlItem *ObjectFactory::createTimeLineControlItemByGroupType(NodeGroup::GroupType groupType)
{
    switch (groupType) {
    case NodeGroup::ADBoardGroup:
        return new TimeLineControlItemOfADBoardGroup;
        break;
    case NodeGroup::RelayGroup:
        return new TimeLineControlItemOfRelayGroup;
        break;
    default:
        break;
    }
    return new TimeLineControlItemOfLuminaireGroup;
}

TimeLineControlItem *ObjectFactory::createTimeLineControlItemByGroupTypeText(const QString &groupTypeText)
{
    NodeGroup::GroupType type=NodeGroup::getGroupTypeByText(groupTypeText);
    return createTimeLineControlItemByGroupType(type);
}

Sensor *ObjectFactory::createSensorByTypeText(const QString &typeText)
{
    int id=QMetaType::type(typeText.toLatin1());
    if(id>0)
    {
        Sensor *sensor=static_cast<Sensor*>(QMetaType::create(id));
        return sensor;
    }
    else
    {
        return NULL;
    }
}

Sensor *ObjectFactory::createSensorByNodeTypeCode(quint8 nodeTypeCode)
{
    QString typeText=m_nodeTypeCodeToTypeTextMap.value(nodeTypeCode);
    if(typeText.isEmpty())
    {
        return NULL;
    }
    return createSensorByTypeText(typeText);
}

Sensor *ObjectFactory::createSensorByNotifyDataCode(quint8 notifyCode)
{
    QString typeText=m_notifyCodeToTypeTextMap.value(notifyCode);
    if(typeText.isEmpty())
    {
        return NULL;
    }
    return createSensorByTypeText(typeText);
}

ObjectFactory::ObjectFactory(QObject *parent) : QObject(parent)
{
    loadMetaObjectInfo();
}

void ObjectFactory::loadMetaObjectInfo()
{
    /************** Register Meta Type ****************/
    //Sensor
    qRegisterMetaType<LuxSensor>(LuxSensor::staticTypeText().toLatin1().data());
    qRegisterMetaType<RealMotionSensor>(RealMotionSensor::staticTypeText().toLatin1().data());
    qRegisterMetaType<VirtualMotionSensor>(VirtualMotionSensor::staticTypeText().toLatin1().data());
    qRegisterMetaType<RealSwitchSensor>(RealSwitchSensor::staticTypeText().toLatin1().data());
    qRegisterMetaType<RealSwitchSensor2CH>(RealSwitchSensor2CH::staticTypeText().toLatin1().data());
    qRegisterMetaType<VirtualSwitchSensor>(VirtualSwitchSensor::staticTypeText().toLatin1().data());
    qRegisterMetaType<PMSensor>(PMSensor::staticTypeText().toLatin1().data());
    qRegisterMetaType<CO1Sensor>(CO1Sensor::staticTypeText().toLatin1().data());
    qRegisterMetaType<CO2Sensor>(CO2Sensor::staticTypeText().toLatin1().data());
    qRegisterMetaType<RealMotionAndLuxCameraSensor>(RealMotionAndLuxCameraSensor::staticTypeText().toLatin1());
    qRegisterMetaType<TemperatureHumiditySensor>(TemperatureHumiditySensor::staticTypeText().toLatin1());
    qRegisterMetaType<StressSensor>(StressSensor::staticTypeText().toLatin1());
    qRegisterMetaType<FormaldehydeSensor>(FormaldehydeSensor::staticTypeText().toLatin1());
    qRegisterMetaType<OzoneSensor>(OzoneSensor::staticTypeText().toLatin1());
    qRegisterMetaType<FlammableGasSensor>(FlammableGasSensor::staticTypeText().toLatin1());
    qRegisterMetaType<RefrgTemperatureHumiditySensor>(RefrgTemperatureHumiditySensor::staticTypeText().toLatin1());
    qRegisterMetaType<UVLightSensor>(UVLightSensor::staticTypeText().toLatin1());
    qRegisterMetaType<HandWashingSensor>(HandWashingSensor::staticTypeText().toLatin1());
    qRegisterMetaType<OneChannelLuminaire>(OneChannelLuminaire::staticTypeText().toLatin1());
    qRegisterMetaType<WarningLight>(WarningLight::staticTypeText().toLatin1());
    qRegisterMetaType<OneChannelRelay>(OneChannelRelay::staticTypeText().toLatin1());
    qRegisterMetaType<ADBoard>(ADBoard::staticTypeText().toLatin1());
    qRegisterMetaType<UVSterilizer>(UVSterilizer::staticTypeText().toLatin1());
    qRegisterMetaType<DryContactRelay>(DryContactRelay::staticTypeText().toLatin1());
    qRegisterMetaType<AlarmLoudspeaker>(AlarmLoudspeaker::staticTypeText().toLatin1());

    /************** TypeCode To TypeText *****************/
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_LUX_SENSOR, LuxSensor::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_MOTION_SENSOR, RealMotionSensor::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_SWITCH_SENSOR, RealSwitchSensor::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_SWITCH_SENSOR_2CH, RealSwitchSensor2CH::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_PM_SENSROR, PMSensor::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_CO1_SENSOR, CO1Sensor::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_CO2_SENSOR, CO2Sensor::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_MOTION_AND_LUX_CAMERA_SENSOR, RealMotionAndLuxCameraSensor::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_TEMPERATURE_HUMIDITY_SENSOR, TemperatureHumiditySensor::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_STRESS_SENSOR, StressSensor::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_FORMALDEHYDE_SENSOR, FormaldehydeSensor::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_OZONE, OzoneSensor::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_REFRG_TEMPERATURE_HUMIDITY_SENSOR, RefrgTemperatureHumiditySensor::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_UVLIGHT_SENSOR, UVLightSensor::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_HAND_WASHING_SENSOR, HandWashingSensor::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_LIGHT, OneChannelLuminaire::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_ADBOARD, ADBoard::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_UV_STERILIZER, UVSterilizer::staticTypeText());
    m_notifyCodeToTypeTextMap.insert(USER_NOTIFY_DATA_OF_FLAMMABLE_GAS, FlammableGasSensor::staticTypeText());

    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_LUX_SENSOR, LuxSensor::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_MOTION_SENSOR, RealMotionSensor::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_SWITCH_SENSOR, RealSwitchSensor::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_SWITCH_SENSOR_2CH, RealSwitchSensor2CH::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_PM_SENSROR, PMSensor::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_CO1_SENSOR, CO1Sensor::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_CO2_SENSOR, CO2Sensor::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_MOTION_AND_LUX_CAMERA_SENSOR, RealMotionAndLuxCameraSensor::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_TEMPERATURE_HUMIDITY_SENSOR, TemperatureHumiditySensor::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_STRESS_SENSOR, StressSensor::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_FORMALDEHYDE_SENSOR, FormaldehydeSensor::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_OZONE_SENSOR, OzoneSensor::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_REFRG_TEMPERATURE_HUMIDITY_SENSOR, RefrgTemperatureHumiditySensor::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_ULTRAVIOLET_LIGHT_SENSOR, UVLightSensor::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_HAND_WASHING_SENSOR, HandWashingSensor::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_LIGHT, OneChannelLuminaire::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_WARNING_LIGHT, WarningLight::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_1CH_RELAY, OneChannelRelay::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_DRY_CONTACT_RELAY, DryContactRelay::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_ALARM_LOUDSPEAKER, AlarmLoudspeaker::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_ADBOARD, ADBoard::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_UV_STERILIZER, UVSterilizer::staticTypeText());
    m_nodeTypeCodeToTypeTextMap.insert(NODE_TYPE_OF_FLAMMABLE_GAS_SENSOR, FlammableGasSensor::staticTypeText());

    m_sensorNodeTypeSet.insert(NODE_TYPE_OF_LUX_SENSOR);
    m_sensorNodeTypeSet.insert(NODE_TYPE_OF_MOTION_SENSOR);
    m_sensorNodeTypeSet.insert(NODE_TYPE_OF_SWITCH_SENSOR);
    m_sensorNodeTypeSet.insert(NODE_TYPE_OF_SWITCH_SENSOR_2CH);
    m_sensorNodeTypeSet.insert(NODE_TYPE_OF_PM_SENSROR);
    m_sensorNodeTypeSet.insert(NODE_TYPE_OF_CO1_SENSOR);
    m_sensorNodeTypeSet.insert(NODE_TYPE_OF_CO2_SENSOR);
    m_sensorNodeTypeSet.insert(NODE_TYPE_OF_MOTION_AND_LUX_CAMERA_SENSOR);
    m_sensorNodeTypeSet.insert(NODE_TYPE_OF_TEMPERATURE_HUMIDITY_SENSOR);
    m_sensorNodeTypeSet.insert(NODE_TYPE_OF_STRESS_SENSOR);
    m_sensorNodeTypeSet.insert(NODE_TYPE_OF_FORMALDEHYDE_SENSOR);
    m_sensorNodeTypeSet.insert(NODE_TYPE_OF_OZONE_SENSOR);
    m_sensorNodeTypeSet.insert(NODE_TYPE_OF_FLAMMABLE_GAS_SENSOR);
    m_sensorNodeTypeSet.insert(NODE_TYPE_OF_REFRG_TEMPERATURE_HUMIDITY_SENSOR);
    m_sensorNodeTypeSet.insert(NODE_TYPE_OF_ULTRAVIOLET_LIGHT_SENSOR);
    m_sensorNodeTypeSet.insert(NODE_TYPE_OF_HAND_WASHING_SENSOR);

    m_sensorUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_LUX_SENSOR);
    m_sensorUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_MOTION_SENSOR);
    m_sensorUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_SWITCH_SENSOR);
    m_sensorUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_SWITCH_SENSOR_2CH);
    m_sensorUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_PM_SENSROR);
    m_sensorUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_CO1_SENSOR);
    m_sensorUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_CO2_SENSOR);
    m_sensorUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_MOTION_AND_LUX_CAMERA_SENSOR);
    m_sensorUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_TEMPERATURE_HUMIDITY_SENSOR);
    m_sensorUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_STRESS_SENSOR);
    m_sensorUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_FORMALDEHYDE_SENSOR);
    m_sensorUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_OZONE);
    m_sensorUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_FLAMMABLE_GAS);
    m_sensorUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_REFRG_TEMPERATURE_HUMIDITY_SENSOR);
    m_sensorUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_UVLIGHT_SENSOR);
    m_sensorUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_HAND_WASHING_SENSOR);

    m_executerNodeTypeSet.insert(NODE_TYPE_OF_LIGHT);
    m_executerNodeTypeSet.insert(NODE_TYPE_OF_WARNING_LIGHT);
    m_executerNodeTypeSet.insert(NODE_TYPE_OF_4CH_RELAY);
    m_executerNodeTypeSet.insert(NODE_TYPE_OF_SOLAR_1CH_LUMINAIRE);
    m_executerNodeTypeSet.insert(NODE_TYPE_OF_1CH_RELAY);
    m_executerNodeTypeSet.insert(NODE_TYPE_OF_ADBOARD);
    m_executerNodeTypeSet.insert(NODE_TYPE_OF_UV_STERILIZER);
    m_executerNodeTypeSet.insert(NODE_TYPE_OF_DRY_CONTACT_RELAY);
    m_executerNodeTypeSet.insert(NODE_TYPE_OF_ALARM_LOUDSPEAKER);

    m_executerUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_LIGHT);
//    m_executerUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_WARNING_LIGHT);
    m_executerUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_4CH_RELAY);
    m_executerUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_SOLAR_1CH_LUMINAIRE);
//    m_executerUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_SOLAR_1CH_LUMINAIRE);
    m_executerUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_ADBOARD);
    m_executerUserDataTypeCodeSet.insert(USER_NOTIFY_DATA_OF_UV_STERILIZER);
}
