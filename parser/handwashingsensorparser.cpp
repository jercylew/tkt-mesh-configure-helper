#include "handwashingsensorparser.h"
#include <domain/handwashingsensor.h>
#include "widget/sensorframe.h"

HandWashingSensorParser::HandWashingSensorParser()
{
    m_nLastUpdateTimestamMSecs = 0;
    m_nAccumulatedTimes = 0;
}

HandWashingSensorParser::~HandWashingSensorParser()
{

}

void HandWashingSensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame,
                                Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    qint64 nCurrentTimestampMSecs = QDateTime::currentMSecsSinceEpoch();

    //42 seconds, too long before getting the report
    if ((nCurrentTimestampMSecs-m_nLastUpdateTimestamMSecs) > 42000)
    {
        qDebug() << "HandWashingSensorParser::parseUserDataFrameToSensor"
                 << "Too long before getting the report, clear the counter";
        m_nAccumulatedTimes = 0;
    }
    else
    {
        m_nAccumulatedTimes++;
    }

    bool bWashing = false;
    if (m_nAccumulatedTimes >= 7)
    {
        qDebug() << "HandWashingSensorParser::parseUserDataFrameToSensor"
                 << "Report times reach 7, set it to true";
        bWashing = true;
        m_nAccumulatedTimes = 0;
    }

    HandWashingSensor *handWashingSensor = static_cast<HandWashingSensor*>(sensor);
    if(handWashingSensor)
    {
        handWashingSensor->setWashing(bWashing);
    }
    m_nLastUpdateTimestamMSecs = nCurrentTimestampMSecs;
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
