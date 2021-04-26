#include "luxsensorparser.h"
#include "domain/luxsensor.h"
#include "model/meshmodel.h"
#include "model/sensorsmodel.h"
#include "utils/meshsavingbox.h"
#include "widget/sensorframe.h"

LuxSensorParser::LuxSensorParser()
{

}

void LuxSensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    LuxSensor *luxSensor=static_cast<LuxSensor*>(sensor);
    if(luxSensor)
    {
        luxSensor->setLux((quint16)(((quint8)dataFrame.at(12))*256+(quint8)dataFrame.at(13)));
    }
}

void LuxSensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor==NULL)
    {
        return;
    }
    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);
    LuxSensor *oldLuxSensor=static_cast<LuxSensor *>(oldSensor);
    LuxSensor *newLuxSensor=static_cast<LuxSensor *>(newSensor);
    if(meshModel->savingBox()->luxSensorShouldBeFiltered(oldLuxSensor->sensorId()))
    {
        oldLuxSensor->pushLux(newLuxSensor->getLux(), meshModel->savingBox()->luxSensorFilterCoefficient());
    }
    else
    {
        oldLuxSensor->setLux(newLuxSensor->getLux());
    }
}

void LuxSensorParser::updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(MeshModel *meshModel, Sensor *sensor)
{
    LuxSensor *luxSensor=static_cast<LuxSensor*>(sensor);
    if(luxSensor)
    {
        meshModel->sensorsModel()->executeRealLuxSensorSpecialLogicWhenUpdateReferredData(luxSensor);
    }
}

void LuxSensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    LuxSensor *luxSensor=static_cast<LuxSensor*>(sensor);
    if(luxSensor)
    {
        QString stateText=QString("%1(lux)").arg(luxSensor->getLux());
        sensorFrame->setStateText(stateText);
        sensorFrame->getIconLabel()->setProperty("type", "sensor_lux");
    }
}
