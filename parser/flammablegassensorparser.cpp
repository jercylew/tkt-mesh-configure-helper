#include "flammablegassensorparser.h"

#include "domain/flammablegassensor.h"
#include "utils/meshsavingbox.h"
#include "model/meshmodel.h"
#include "model/sensorsmodel.h"
#include "widget/sensorframe.h"
#include <QDebug>

FlammableGasSensorParser::FlammableGasSensorParser()
{

}

void FlammableGasSensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    FlammableGasSensor *flmmGasSensor=static_cast<FlammableGasSensor*>(sensor);
    if(flmmGasSensor)
    {
        double dVal = ((quint16)(((quint8)dataFrame.at(12))*256+(quint8)dataFrame.at(13))) / 10.0;
        flmmGasSensor->setConcentration(dVal);
    }
}

void FlammableGasSensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor==NULL)
    {
        return;
    }

    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);
    FlammableGasSensor *oldFlmmGasSensor=static_cast<FlammableGasSensor *>(oldSensor);
    FlammableGasSensor *newFlmmGasSensor=static_cast<FlammableGasSensor *>(newSensor);
    oldFlmmGasSensor->setConcentration(newFlmmGasSensor->concentration());
}

void FlammableGasSensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    FlammableGasSensor *flmmGasSensor = static_cast<FlammableGasSensor*>(sensor);
    if(flmmGasSensor)
    {
        QString stateText = QString("%1%").arg(flmmGasSensor->concentration());
        sensorFrame->setStateText(stateText);
        sensorFrame->getIconLabel()->setProperty("type", flmmGasSensor->typeText());
    }
}
