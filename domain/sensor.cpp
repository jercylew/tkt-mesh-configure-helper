#include "sensor.h"

#define SENSOR_KIND_TEXT    "sensor"

Sensor::Sensor()
{

}

Sensor::~Sensor()
{

}

void Sensor::setSensorId(const QString &sensorId)
{
    this->m_sensorId=sensorId;
}

QString Sensor::sensorId()
{
    return this->m_sensorId;
}

QString Sensor::nodeId()
{
    return this->sensorId();
}

QString Sensor::nodeKindText()
{
    return SENSOR_KIND_TEXT;
}

QString Sensor::staticNodeKindText()
{
    return SENSOR_KIND_TEXT;
}

void Sensor::loadSensorBasicDataFromDB(Sensor *sensor)
{
    this->setSensorId(sensor->sensorId());
    this->setName(sensor->getName());
}

void Sensor::loadSensorBasicDataFromOnlineStatus(Sensor *sensor)
{
    this->setSensorId(sensor->sensorId());
    this->setOnline(sensor->isOnline());
    this->setBluetoothAddress(sensor->getBluetoothAddress());
}

void Sensor::loadSensorBasicDataFromUserData(Sensor *sensor)
{
    this->setSensorId(sensor->sensorId());
    this->setBluetoothAddress(sensor->getBluetoothAddress());
}

