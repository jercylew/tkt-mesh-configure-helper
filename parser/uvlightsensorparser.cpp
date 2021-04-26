#include "uvlightsensorparser.h"
#include "domain/uvlightsensor.h"
#include "widget/sensorframe.h"

UVLightSensorParser::UVLightSensorParser()
{

}

UVLightSensorParser::~UVLightSensorParser()
{

}

void UVLightSensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame,
                                Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    UVLightSensor *uvLightSensor = static_cast<UVLightSensor*>(sensor);
    if(uvLightSensor)
    {
        uvLightSensor->setSterilizing((dataFrame.at(12)&0xFF) == 0x01);
    }
}

void UVLightSensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel,
                                    Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor == nullptr)
    {
        return;
    }

    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);

    UVLightSensor *oldUVLightSensor = static_cast<UVLightSensor*>(oldSensor);
    UVLightSensor *newUVLightSensor = static_cast<UVLightSensor*>(newSensor);
    bool bOldSterilizing = oldUVLightSensor->getSterilizing();
    oldUVLightSensor->setSterilizing(newUVLightSensor->getSterilizing());
    if(bOldSterilizing != oldUVLightSensor->getSterilizing())
    {
//        oldUVLightSensor->setDataChanged(true);
    }
}

void UVLightSensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    UVLightSensor *uvLightSensor = static_cast<UVLightSensor*>(sensor);
    if(uvLightSensor)
    {
        QString stateText=QObject::tr("STATUS: %1")
                .arg(uvLightSensor->getSterilizing() ?
                         QObject::tr("UVLightSensor.Sterilizing") :
                         QObject::tr("UVLightSensor.Stopped"));

        sensorFrame->setStateText(stateText);
        sensorFrame->getIconLabel()->setProperty("type", "uvlight_sensor");
    }
}
