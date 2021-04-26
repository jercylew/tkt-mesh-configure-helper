#include "virtualmotionsensorparser.h"
#include <QObject>
#include "domain/virtualmotionsensor.h"
#include "model/meshmodel.h"
#include "model/sensorsmodel.h"
#include <QStringList>
#include "widget/sensorframe.h"

VirtualMotionSensorParser::VirtualMotionSensorParser()
{

}

void VirtualMotionSensorParser::parseSensorDataToSensor(const QString &sensorData, Sensor *sensor)
{
    QList<QString> realMotionSensorIdList;
    QStringList list=sensorData.split(",");
    for(int i=0; i<list.size(); i++)
    {
        QString realSensorId=list.at(i).trimmed();
        realMotionSensorIdList.push_back(realSensorId);
    }

    VirtualMotionSensor *virtualMotionSensor=static_cast<VirtualMotionSensor*>(sensor);
    if(virtualMotionSensor==NULL)
    {
        return;
    }
    virtualMotionSensor->setBindingRealMotionSensorIdList(realMotionSensorIdList);
}

QString VirtualMotionSensorParser::buildSensorDataFromSensor(Sensor *sensor)
{
    VirtualMotionSensor *virtualMotionSensor=static_cast<VirtualMotionSensor*>(sensor);
    if(virtualMotionSensor==NULL)
    {
        return "";
    }
    QString sensorData;
    QList<QString> realMotionSensorIdList=virtualMotionSensor->bindingRealMotionSensorIdList();
    for(int i=0; i<realMotionSensorIdList.size(); i++)
    {
        if(i!=0)
        {
            sensorData.push_back(",");
        }
        sensorData.push_back(realMotionSensorIdList.at(i));
    }
    return sensorData;
}

void VirtualMotionSensorParser::updateSensorFromDB(MeshModel *meshModel, Sensor *dbSensor, Sensor *oldSensor)
{
    SensorParser::updateSensorFromDB(meshModel, dbSensor, oldSensor);
    if(oldSensor==NULL)
    {
        return;
    }
    VirtualMotionSensor *oldVirtualMotionSensor=static_cast<VirtualMotionSensor *>(oldSensor);
    VirtualMotionSensor *dbVirtualMotionSensor=static_cast<VirtualMotionSensor *>(dbSensor);
    oldVirtualMotionSensor->setBindingRealMotionSensorIdList(dbVirtualMotionSensor->bindingRealMotionSensorIdList());
}

void VirtualMotionSensorParser::updateReferredSensorDataAndActionAfterDBUpdate(MeshModel *meshModel, Sensor *sensor)
{
    VirtualMotionSensor *virtualMotionSensor=static_cast<VirtualMotionSensor*>(sensor);
    if(virtualMotionSensor==NULL)
    {
        return;
    }
    meshModel->sensorsModel()->updateRealMotionSensorToVirtualMotionSensorMap(virtualMotionSensor->sensorId(), virtualMotionSensor->bindingRealMotionSensorIdList());
}

void VirtualMotionSensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    sensorFrame->setName(sensor->getName());
    sensorFrame->setId(sensor->sensorId());
    VirtualMotionSensor *virtualMotionSensor=static_cast<VirtualMotionSensor*>(sensor);
    if(virtualMotionSensor!=NULL)
    {
        sensorFrame->setStateText(virtualMotionSensor->isChecked()?QObject::tr("SensorListViewModel.ManChecked"):QObject::tr("SensorListViewModel.ManNotChecked"));
        sensorFrame->getIconLabel()->setProperty("type", "virtual_motion_sensor");
    }
}

