#include "parsermanager.h"
#include <QMutexLocker>

//Devices
#include "domain/luxsensor.h"
#include "domain/realmotionsensor.h"
#include "domain/pmsensor.h"
#include "domain/co1sensor.h"
#include "domain/co2sensor.h"
#include "domain/realswitchsensor.h"
#include "domain/realswitchsensor2ch.h"
#include "domain/realmotionandluxcamerasensor.h"
#include "domain/virtualmotionsensor.h"
#include "domain/virtualswitchsensor.h"
#include "domain/temperaturehumiditysensor.h"
#include "domain/stresssensor.h"
#include "domain/formaldehydesensor.h"
#include "domain/ozonesensor.h"
#include "domain/refrgtemperaturehumiditysensor.h"
#include "domain/uvlightsensor.h"
#include "domain/handwashingsensor.h"
#include "domain/flammablegassensor.h"

#include "domain/onechannelluminaire.h"
#include "domain/onechannelrelay.h"
#include "domain/adboard.h"
#include "domain/uvsterilizer.h"
#include "domain/warninglight.h"
#include "domain/drycontactrelay.h"

//Parsers
#include "parser/luxsensorparser.h"
#include "parser/realmotionsensorparser.h"
#include "parser/pmsensorparser.h"
#include "parser/co1sensorparser.h"
#include "parser/co2sensorparser.h"
#include "parser/realswitchsensorparser.h"
#include "parser/realswitchsensorparser2ch.h"
#include "parser/realmotionandluxcamerasensorparser.h"
#include "parser/virtualmotionsensorparser.h"
#include "parser/virtualswitchsensorparser.h"
#include "parser/temperaturehumditysensorparser.h"
#include "parser/stresssensorparser.h"
#include "parser/formaldehydesensorparser.h"
#include "parser/refrgtemperaturehumditysensorparser.h"
#include "parser/uvlightsensorparser.h"
#include "parser/handwashingsensorparser.h"
#include "parser/onechannelluminaireparser.h"
#include "parser/onechannelrelayparser.h"
#include "parser/adboardparser.h"
#include "parser/uvsterilizerparser.h"
#include "parser/warninglightparser.h"
#include "parser/ozonesensorparser.h"
#include "parser/drycontactrelayparser.h"
#include "parser/flammablegassensorparser.h"

#include "mesh_define.h"

ParserManager *ParserManager::m_parserManager=NULL;
QMutex ParserManager::m_parserManagerMutex;

ParserManager *ParserManager::instance()
{
    if(m_parserManager==NULL)
    {
        QMutexLocker locker(&m_parserManagerMutex);
        if(m_parserManager==NULL)
        {
            m_parserManager=new ParserManager;
        }
    }
    return m_parserManager;
}

ExecuterParser *ParserManager::getExecuterParserByTypeText(const QString &typeText)
{
    return m_executerTypeTextToParserMap.value(typeText);
}

ExecuterParser *ParserManager::getExecuterParserByTypeCode(quint8 executerTypeCode)
{
    return m_executerTypeCodeToParserMap.value(executerTypeCode);
}

SensorParser *ParserManager::getSensorParserByTypeText(const QString &typeText)
{
    return m_sensorTypeTextToParserMap.value(typeText);
}

SensorParser *ParserManager::getSensorParserByType(Sensor::SensorType sensorType)
{
    return m_sensorTypeToParserMap.value(sensorType);
}

SensorParser *ParserManager::getSensorParserBySensorCode(quint8 sensorCode)
{
    return m_sensorCodeToParserMap.value(sensorCode);
}

ParserManager::ParserManager(QObject *parent) : QObject(parent)
{
    SensorParser *sensorParser=new LuxSensorParser;
    m_sensorTypeTextToParserMap.insert(LuxSensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::LuxSensor, sensorParser);
    m_sensorCodeToParserMap.insert(USER_NOTIFY_DATA_OF_LUX_SENSOR, sensorParser);

    sensorParser=new RealMotionSensorParser;
    m_sensorTypeTextToParserMap.insert(RealMotionSensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::RealMotionSensor, sensorParser);
    m_sensorCodeToParserMap.insert(USER_NOTIFY_DATA_OF_MOTION_SENSOR, sensorParser);

    sensorParser=new PMSensorParser;
    m_sensorTypeTextToParserMap.insert(PMSensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::PMSensor, sensorParser);
    m_sensorCodeToParserMap.insert(USER_NOTIFY_DATA_OF_PM_SENSROR, sensorParser);

    sensorParser=new CO1SensorParser;
    m_sensorTypeTextToParserMap.insert(CO1Sensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::CO1Sensor, sensorParser);
    m_sensorCodeToParserMap.insert(USER_NOTIFY_DATA_OF_CO1_SENSOR, sensorParser);

    sensorParser=new CO2SensorParser;
    m_sensorTypeTextToParserMap.insert(CO2Sensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::CO2Sensor, sensorParser);
    m_sensorCodeToParserMap.insert(USER_NOTIFY_DATA_OF_CO2_SENSOR, sensorParser);

    sensorParser=new RealSwitchSensorParser;
    m_sensorTypeTextToParserMap.insert(RealSwitchSensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::RealSwitchSensor, sensorParser);
    m_sensorCodeToParserMap.insert(USER_NOTIFY_DATA_OF_SWITCH_SENSOR, sensorParser);

    sensorParser=new RealSwitchSensorParser2CH;
    m_sensorTypeTextToParserMap.insert(RealSwitchSensor2CH::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::RealSwitchSensor2CH, sensorParser);
    m_sensorCodeToParserMap.insert(USER_NOTIFY_DATA_OF_SWITCH_SENSOR_2CH, sensorParser);

    sensorParser=new RealMotionAndLuxCameraSensorParser;
    m_sensorTypeTextToParserMap.insert(RealMotionAndLuxCameraSensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::RealMotionAndLuxCameraSensor, sensorParser);
    m_sensorCodeToParserMap.insert(USER_NOTIFY_DATA_OF_MOTION_AND_LUX_CAMERA_SENSOR, sensorParser);

    sensorParser=new VirtualMotionSensorParser;
    m_sensorTypeTextToParserMap.insert(VirtualMotionSensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::VirtualMotionSensor, sensorParser);

    sensorParser=new VirtualSwitchSensorParser;
    m_sensorTypeTextToParserMap.insert(VirtualSwitchSensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::VirtualSwitchSensor, sensorParser);

    sensorParser=new TemperatureHumditySensorParser;
    m_sensorTypeTextToParserMap.insert(TemperatureHumiditySensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::TemperatureHumiditySensor, sensorParser);
    m_sensorCodeToParserMap.insert(USER_NOTIFY_DATA_OF_TEMPERATURE_HUMIDITY_SENSOR, sensorParser);

    sensorParser=new StressSensorParser;
    m_sensorTypeTextToParserMap.insert(StressSensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::StressSensor, sensorParser);
    m_sensorCodeToParserMap.insert(USER_NOTIFY_DATA_OF_STRESS_SENSOR, sensorParser);

    sensorParser=new FormaldehydeSensorParser;
    m_sensorTypeTextToParserMap.insert(FormaldehydeSensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::FormaldehydeSensor, sensorParser);
    m_sensorCodeToParserMap.insert(USER_NOTIFY_DATA_OF_FORMALDEHYDE_SENSOR, sensorParser);

    sensorParser = new OzoneSensorParser;
    m_sensorTypeTextToParserMap.insert(OzoneSensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::OzoneSensor, sensorParser);
    m_sensorCodeToParserMap.insert(USER_NOTIFY_DATA_OF_OZONE, sensorParser);

    sensorParser = new FlammableGasSensorParser;
    m_sensorTypeTextToParserMap.insert(FlammableGasSensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::FlammableGasSensor, sensorParser);
    m_sensorCodeToParserMap.insert(USER_NOTIFY_DATA_OF_FLAMMABLE_GAS, sensorParser);

    sensorParser=new RefrgTemperatureHumditySensorParser;
    m_sensorTypeTextToParserMap.insert(RefrgTemperatureHumiditySensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::RefrgTemperatureHumiditySensor, sensorParser);
    m_sensorCodeToParserMap.insert(USER_NOTIFY_DATA_OF_REFRG_TEMPERATURE_HUMIDITY_SENSOR, sensorParser);

    sensorParser=new UVLightSensorParser;
    m_sensorTypeTextToParserMap.insert(UVLightSensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::UVLightSensor, sensorParser);
    m_sensorCodeToParserMap.insert(USER_NOTIFY_DATA_OF_UVLIGHT_SENSOR, sensorParser);

    sensorParser=new HandWashingSensorParser;
    m_sensorTypeTextToParserMap.insert(HandWashingSensor::staticTypeText(), sensorParser);
    m_sensorTypeToParserMap.insert(Sensor::HandWashingSensor, sensorParser);
    m_sensorCodeToParserMap.insert(USER_NOTIFY_DATA_OF_HAND_WASHING_SENSOR, sensorParser);

    ExecuterParser *executerParser=new OneChannelLuminaireParser;
    m_executerTypeCodeToParserMap.insert(NODE_TYPE_OF_LIGHT, executerParser);
    m_executerTypeTextToParserMap.insert(OneChannelLuminaire::staticTypeText(), executerParser);

    executerParser=new OneChannelRelayParser;
    m_executerTypeCodeToParserMap.insert(NODE_TYPE_OF_1CH_RELAY, executerParser);
    m_executerTypeTextToParserMap.insert(OneChannelRelay::staticTypeText(), executerParser);

    executerParser=new DryContactRelayParser;
    m_executerTypeCodeToParserMap.insert(NODE_TYPE_OF_DRY_CONTACT_RELAY, executerParser);
    m_executerTypeTextToParserMap.insert(DryContactRelay::staticTypeText(), executerParser);

    executerParser=new ADBoardParser;
    m_executerTypeCodeToParserMap.insert(NODE_TYPE_OF_ADBOARD, executerParser);
    m_executerTypeTextToParserMap.insert(ADBoard::staticTypeText(), executerParser);

    executerParser=new WarningLightParser;
    m_executerTypeCodeToParserMap.insert(NODE_TYPE_OF_WARNING_LIGHT, executerParser);
    m_executerTypeTextToParserMap.insert(WarningLight::staticTypeText(), executerParser);

    executerParser=new UVSterilizerParser;
    m_executerTypeCodeToParserMap.insert(NODE_TYPE_OF_UV_STERILIZER, executerParser);
    m_executerTypeTextToParserMap.insert(UVSterilizer::staticTypeText(), executerParser);
}
