#include "ozonesensorparser.h"
#include "domain/ozonesensor.h"
#include "model/meshmodel.h"
#include "utils/meshsavingbox.h"
#include "model/sensorsmodel.h"
#include "widget/sensorframe.h"

OzoneSensorParser::OzoneSensorParser()
{

}

void OzoneSensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    OzoneSensor *ozoneSensor = static_cast<OzoneSensor*>(sensor);
    if(ozoneSensor)
    {
        ozoneSensor->setOzone((quint16)(((quint8)dataFrame.at(12))*256+(quint8)dataFrame.at(13)));
    }
}

void OzoneSensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor == nullptr)
    {
        return;
    }
    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);
    OzoneSensor *oldOzoneSensor = static_cast<OzoneSensor *>(oldSensor);
    OzoneSensor *newOzoneSensor = static_cast<OzoneSensor *>(newSensor);
    oldOzoneSensor->setOzone(newOzoneSensor->ozone());
}

void OzoneSensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    OzoneSensor *ozoneSensor = static_cast<OzoneSensor*>(sensor);
    if(ozoneSensor)
    {
        QString stateText = QString("%1(ppb)").arg(ozoneSensor->ozone());
        sensorFrame->setStateText(stateText);
        sensorFrame->getIconLabel()->setProperty("type", ozoneSensor->typeText());
    }
}
