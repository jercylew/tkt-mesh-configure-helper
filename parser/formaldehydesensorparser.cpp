#include "formaldehydesensorparser.h"
#include "domain/formaldehydesensor.h"
#include "model/meshmodel.h"
#include "utils/meshsavingbox.h"
#include <QDebug>
#include "model/sensorsmodel.h"
#include "widget/sensorframe.h"

FormaldehydeSensorParser::FormaldehydeSensorParser()
{

}

void FormaldehydeSensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    FormaldehydeSensor *formaldehydeSensor=static_cast<FormaldehydeSensor*>(sensor);
    if(formaldehydeSensor)
    {
        formaldehydeSensor->setFormaldehyde((quint16)(((quint8)dataFrame.at(12))*256+(quint8)dataFrame.at(13)));
    }
}

void FormaldehydeSensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor==NULL)
    {
        return;
    }
    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);
    FormaldehydeSensor *oldFormaldehydeSensor=static_cast<FormaldehydeSensor *>(oldSensor);
    FormaldehydeSensor *newFormaldehydeSensor=static_cast<FormaldehydeSensor *>(newSensor);
    oldFormaldehydeSensor->setFormaldehyde(newFormaldehydeSensor->formaldehyde());
}

void FormaldehydeSensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    FormaldehydeSensor *formaldehydeSensor=static_cast<FormaldehydeSensor*>(sensor);
    if(formaldehydeSensor)
    {
        QString stateText=QString("%1(ppb)").arg(formaldehydeSensor->formaldehyde());
        sensorFrame->setStateText(stateText);
        sensorFrame->getIconLabel()->setProperty("type", "formaldehyde_sensor");
    }
}

