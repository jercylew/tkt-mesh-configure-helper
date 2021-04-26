#include "handwashingsensorparser.h"
#include <domain/handwashingsensor.h>
#include "widget/sensorframe.h"

HandWashingSensorParser::HandWashingSensorParser()
{

}

HandWashingSensorParser::~HandWashingSensorParser()
{

}

void HandWashingSensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame,
                                Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    HandWashingSensor *handWashingSensor = static_cast<HandWashingSensor*>(sensor);
    if(handWashingSensor)
    {
        handWashingSensor->setWashing((dataFrame.at(12)&0xFF) == 0x01);
    }
}

void HandWashingSensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel,
                                    Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor == nullptr)
    {
        return;
    }

    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);

    HandWashingSensor *oldHandWashingSensor = static_cast<HandWashingSensor*>(oldSensor);
    HandWashingSensor *newHandWashingSensor = static_cast<HandWashingSensor*>(newSensor);
    bool bOldWashing = oldHandWashingSensor->getWashing();
    oldHandWashingSensor->setWashing(newHandWashingSensor->getWashing());
    if(bOldWashing != oldHandWashingSensor->getWashing())
    {
//        oldHandWashingSensor->setDataChanged(true);
    }
}

void HandWashingSensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    HandWashingSensor *handWashingSensor = static_cast<HandWashingSensor*>(sensor);
    if(handWashingSensor)
    {
        QString stateText=QObject::tr("STATUS: %1")
                .arg(handWashingSensor->getWashing() ?
                         QObject::tr("HandWashingSensor.Washing") :
                         QObject::tr("HandWashingSensor.Stopped"));

        sensorFrame->setStateText(stateText);
        sensorFrame->getIconLabel()->setProperty("type", "hand_washing_sensor");
    }
}
