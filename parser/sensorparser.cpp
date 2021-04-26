#include "sensorparser.h"
#include "domain/sensor.h"
#include "widget/sensorframe.h"
#include <QPainter>

SensorParser::SensorParser()
{
    m_sensorFrame=NULL;
}

void SensorParser::parseDBDataToSensor(const QSqlQuery &query, Sensor *sensor)
{
    QString id=query.value("sensor_id").toString();
    QString name=query.value("sensor_name").toString();
    QString data=query.value("sensor_data").toString();
    sensor->setSensorId(id);
    sensor->setName(name);
    parseSensorDataToSensor(data, sensor);
}

void SensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    sensor->setBluetoothAddress((quint8)dataFrame.at(3));
    sensor->setSensorId(QString("%1.%2").arg(sensor->shortTypeText()).arg(sensor->getBluetoothAddress()&0xFF));
    sensor->setName(sensor->sensorId());
}

bool SensorParser::parseOnlineStatus4BytesDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    if(dataFrame.size()!=4)
    {
        return false;
    }
    quint8 bluetoothAddress=dataFrame.at(0);
    sensor->setSensorId(QString("%1.%2").arg(sensor->shortTypeText()).arg(bluetoothAddress&0xFF));
    sensor->setName(sensor->sensorId());
    sensor->setBluetoothAddress(bluetoothAddress);
    return true;
}

void SensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor==NULL)
    {
        return;
    }
    if(oldSensor->getBluetoothAddress()==0)
    {
        oldSensor->setBluetoothAddress(newSensor->getBluetoothAddress());
    }
    if(!oldSensor->isTouched())
    {
        oldSensor->setTouched(true);
    }
}

void SensorParser::updateSensorFromOnlineStatus(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor==NULL)
    {
        return;
    }
    if(oldSensor->getBluetoothAddress()==0)
    {
        oldSensor->setBluetoothAddress(newSensor->getBluetoothAddress());
    }
    if(!oldSensor->isTouched())
    {
        oldSensor->setTouched(true);
    }
}

void SensorParser::updateSensorFromDB(MeshModel *meshModel, Sensor *dbSensor, Sensor *oldSensor)
{
    if(oldSensor==NULL)
    {
        return;
    }
    oldSensor->setName(dbSensor->getName());
}

void SensorParser::updateReferredSensorDataAndActionAfterDBUpdate(MeshModel *meshModel, Sensor *sensor)
{

}

void SensorParser::updateReferredSensorDataAndNotifyAfterOnlineStatusUpdate(MeshModel *meshModel, Sensor *sensor)
{

}

void SensorParser::updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(MeshModel *meshModel, Sensor *sensor)
{

}

void SensorParser::parseSensorDataToSensor(const QString &sensorData, Sensor *sensor)
{

}

QString SensorParser::buildSensorDataFromSensor(Sensor *sensor)
{
    return "";
}

QString SensorParser::getSensorTooltipInSensorListView(Sensor *sensor)
{
    return QString("Address: %1").arg(sensor->getBluetoothAddress());
}

QPixmap SensorParser::getSurfaceWithTouchInfo(MeshModel *meshModel, Sensor *sensor)
{
    QPixmap surface=buildSurfaceWithNoTouchInfo(meshModel, sensor);
    if(sensor->isVirtual())
    {
        return surface;
    }
    else
    {
        QPainter painter(&surface);
        if(sensor->isTouched())
        {
            painter.drawImage(surface.width()-30, 0, QImage(":/images/touch_yes.png"));
        }
        else
        {
            painter.drawImage(surface.width()-30, 0, QImage(":/images/touch_no.png"));
        }
        return surface;
    }
}

void SensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    sensorFrame->setName(sensor->getName().isEmpty()?sensor->sensorId():sensor->getName());
    sensorFrame->setId(sensor->sensorId());
}

SensorFrame *SensorParser::getSensorFrame()
{
    if(m_sensorFrame==NULL)
    {
        m_sensorFrame=new SensorFrame;
    }
    return m_sensorFrame;
}

QPixmap SensorParser::buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Sensor *sensor)
{
    SensorFrame *sensorFrame=getSensorFrame();
    parseSensorToSensorFrame(sensor, sensorFrame);
    sensorFrame->resetStyle();
    return sensorFrame->grab();
}




