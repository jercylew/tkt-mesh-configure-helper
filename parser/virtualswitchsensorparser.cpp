#include "virtualswitchsensorparser.h"
#include "domain/virtualswitchsensor.h"
#include "widget/sensorframe.h"

VirtualSwitchSensorParser::VirtualSwitchSensorParser()
{

}

void VirtualSwitchSensorParser::parseSensorDataToSensor(const QString &sensorData, Sensor *sensor)
{
    QMap<int, QString> channelControlTypeTextMap;
    QList<quint8> logicGroupIDList;
    QString bindingSwitchSensorId;
    QList<QString> pairList=sensorData.split(";");
    for(int i=0; i<pairList.size(); i++)
    {
        QString pair=pairList.at(i).trimmed();
        if(pair.isEmpty())
        {
            continue;
        }
        if(pair.startsWith("@"))
        {
            if(pair.length()>1)
            {
                bindingSwitchSensorId=pair.mid(1).trimmed();
            }
        }
        else if(pair.startsWith("#"))
        {
            if(pair.length()>1)
            {
                QStringList groupIdList=pair.mid(1).split(",");
                for(int s=0; s<groupIdList.size(); s++)
                {
                    QString groupId=groupIdList.at(s).trimmed();
                    if(groupId.isEmpty())
                    {
                        continue;
                    }
                    logicGroupIDList.push_back(groupId.toInt());
                }
            }
        }
        else
        {
            int index=pair.indexOf(":");
            if(index>0 && pair.length()>(index+1))
            {
                QString trigValueSection=pair.left(index).trimmed();
                QString controlTypeSection=pair.mid(index+1).trimmed();
                bool ok=true;
                int trigValue=trigValueSection.toInt(&ok);
                if(ok)
                {
                    channelControlTypeTextMap.insert(trigValue, controlTypeSection);
                }
            }
        }
    }

    VirtualSwitchSensor *virtualSwitchSensor=static_cast<VirtualSwitchSensor*>(sensor);
    if(virtualSwitchSensor==NULL)
    {
        return;
    }
    virtualSwitchSensor->setBindingSwitchSensorId(bindingSwitchSensorId);
    virtualSwitchSensor->setChannelControlTypeTextMap(channelControlTypeTextMap);
    virtualSwitchSensor->setLogicGroupIDList(logicGroupIDList);
}

QString VirtualSwitchSensorParser::buildSensorDataFromSensor(Sensor *sensor)
{
    VirtualSwitchSensor *virtualSwitchSensor=static_cast<VirtualSwitchSensor*>(sensor);
    if(virtualSwitchSensor==NULL)
    {
        return "";
    }
    QMap<int, QString> channelControlTypeTextMap=virtualSwitchSensor->getChannelControlTypeTextMap();
    QList<int> keyList=channelControlTypeTextMap.keys();
    qSort(keyList.begin(), keyList.end());
    QString text;
    text.push_back(QString("@%1").arg(virtualSwitchSensor->getBindingSwitchSensorId()));
    text.push_back(";");

    QString logicalGroupIDsText;
    QList<quint8> logicalGroupIDsList=virtualSwitchSensor->getLogicGroupIDList();
    for(int i=0; i<logicalGroupIDsList.size(); i++)
    {
        if(i!=0)
        {
            logicalGroupIDsText.push_back(",");
        }
        logicalGroupIDsText.push_back(QString::number(logicalGroupIDsList.at(i)&0xFF));
    }
    text.push_back(QString("#%1").arg(logicalGroupIDsText));

    if(keyList.size()>0)
    {
        for(int i=0; i<keyList.size(); i++)
        {
            text.push_back(";");
            text.push_back(QString("%1:%2").arg(keyList.at(i)).arg(channelControlTypeTextMap.value(keyList.at(i))));
        }
    }

    return text;
}

void VirtualSwitchSensorParser::updateSensorFromDB(MeshModel *meshModel, Sensor *dbSensor, Sensor *oldSensor)
{
    SensorParser::updateSensorFromDB(meshModel, dbSensor, oldSensor);
    if(oldSensor==NULL)
    {
        return;
    }
    VirtualSwitchSensor *oldVirtualSwitchSensor=static_cast<VirtualSwitchSensor *>(oldSensor);
    VirtualSwitchSensor *dbVirtualSwitchSensor=static_cast<VirtualSwitchSensor *>(dbSensor);
    oldVirtualSwitchSensor->setBindingSwitchSensorId(dbVirtualSwitchSensor->getBindingSwitchSensorId());
    oldVirtualSwitchSensor->setChannelControlTypeTextMap(dbVirtualSwitchSensor->getChannelControlTypeTextMap());
    oldVirtualSwitchSensor->setLogicGroupIDList(dbVirtualSwitchSensor->getLogicGroupIDList());
}

void VirtualSwitchSensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    sensorFrame->setName(sensor->getName());
    sensorFrame->setId(sensor->sensorId());
    VirtualSwitchSensor *virtualSwitchSensor=static_cast<VirtualSwitchSensor*>(sensor);
    if(virtualSwitchSensor!=NULL)
    {
        sensorFrame->setStateText(QString("%1").arg(virtualSwitchSensor->currentTrigValue()));
        sensorFrame->getIconLabel()->setProperty("type", "virtual_switch_sensor");
    }
}
