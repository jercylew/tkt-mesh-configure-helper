#include "pmsensorparser.h"
#include "domain/pmsensor.h"
#include "widget/sensorframe.h"

PMSensorParser::PMSensorParser()
{

}

void PMSensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    PMSensor *pmSensor=static_cast<PMSensor*>(sensor);
    if(pmSensor)
    {
         pmSensor->setPm25((quint16)(((quint8)dataFrame.at(14))*256+(quint8)dataFrame.at(15)));
    }
}

void PMSensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor==NULL)
    {
        return;
    }
    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);
    PMSensor *oldPMSensor=static_cast<PMSensor*>(oldSensor);
    PMSensor *newPMSensor=static_cast<PMSensor*>(newSensor);
    oldPMSensor->setPm25(newPMSensor->getPm25());
}

void PMSensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    PMSensor *pmSensor=static_cast<PMSensor*>(sensor);
    if(pmSensor)
    {
        sensorFrame->setStateText(QString("%1(ug/m³)").arg(pmSensor->getPm25()));
        sensorFrame->getIconLabel()->setProperty("type", "pm_sensor");
    }
}
