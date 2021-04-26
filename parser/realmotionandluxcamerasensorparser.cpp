#include "realmotionandluxcamerasensorparser.h"
#include "domain/realmotionandluxcamerasensor.h"
#include "model/meshmodel.h"
#include "model/sensorsmodel.h"
#include "widget/sensorframe.h"

RealMotionAndLuxCameraSensorParser::RealMotionAndLuxCameraSensorParser()
{

}

void RealMotionAndLuxCameraSensorParser::parseUserDataFrameToSensor(const QByteArray &dataFrame, Sensor *sensor)
{
    SensorParser::parseUserDataFrameToSensor(dataFrame, sensor);
    RealMotionAndLuxCameraSensor *realCameraSensor=static_cast<RealMotionAndLuxCameraSensor*>(sensor);
    if(realCameraSensor)
    {
        char motionTrigByte=dataFrame.at(12);
        realCameraSensor->setMotionChannelTrigged(1, (motionTrigByte&0x01));
        realCameraSensor->setMotionChannelTrigged(2, (motionTrigByte&0x02));
        realCameraSensor->setMotionChannelTrigged(3, (motionTrigByte&0x04));
        realCameraSensor->setMotionChannelTrigged(4, (motionTrigByte&0x08));
        realCameraSensor->setLuxChannelValue(1, dataFrame.at(14));
        realCameraSensor->setLuxChannelValue(2, dataFrame.at(15));
        realCameraSensor->setLuxChannelValue(3, dataFrame.at(16));
        realCameraSensor->setLuxChannelValue(4, dataFrame.at(17));
        if(dataFrame.at(18)==(char)0x0A)
        {
            realCameraSensor->setApEnabled(true);
        }
        else
        {
            realCameraSensor->setApEnabled(false);
        }
    }
}

void RealMotionAndLuxCameraSensorParser::updateSensorFromUserDataNotify(MeshModel *meshModel, Sensor *newSensor, Sensor *oldSensor)
{
    if(oldSensor==NULL)
    {
        return;
    }
    SensorParser::updateSensorFromUserDataNotify(meshModel, newSensor, oldSensor);
    RealMotionAndLuxCameraSensor *oldRealCameraSensor=static_cast<RealMotionAndLuxCameraSensor*>(oldSensor);
    RealMotionAndLuxCameraSensor *newRealCameraSensor=static_cast<RealMotionAndLuxCameraSensor*>(newSensor);
    oldRealCameraSensor->setMotionChannelTrigMap(newRealCameraSensor->motionChannelTrigMap());
    oldRealCameraSensor->setMotionChannelLuxMap(newRealCameraSensor->motionChannelLuxMap());
    oldRealCameraSensor->setApEnabled(newRealCameraSensor->apEnabled());
}

void RealMotionAndLuxCameraSensorParser::updateReferredSensorDataAndNotifyAfterOnlineStatusUpdate(MeshModel *meshModel, Sensor *sensor)
{
    RealMotionAndLuxCameraSensor *realMotionLuxCamera=static_cast<RealMotionAndLuxCameraSensor*>(sensor);
    if(sensor)
    {
        meshModel->sensorsModel()->executeRealMotionLuxCameraSpecialLogicWhenUpdateReferredData(realMotionLuxCamera);
    }
}

void RealMotionAndLuxCameraSensorParser::updateReferredSensorDataAndActionAfterUserDataNotifyUpdate(MeshModel *meshModel, Sensor *sensor)
{
    RealMotionAndLuxCameraSensor *realMotionLuxCamera=static_cast<RealMotionAndLuxCameraSensor*>(sensor);
    if(sensor)
    {
        meshModel->sensorsModel()->executeRealMotionLuxCameraSpecialLogicWhenUpdateReferredData(realMotionLuxCamera);
    }
}

void RealMotionAndLuxCameraSensorParser::parseSensorToSensorFrame(Sensor *sensor, SensorFrame *sensorFrame)
{
    SensorParser::parseSensorToSensorFrame(sensor, sensorFrame);
    RealMotionAndLuxCameraSensor *realCameraSensor=static_cast<RealMotionAndLuxCameraSensor*>(sensor);
    if(realCameraSensor)
    {
        sensorFrame->setStateText(QString("%1 %2 %3 %4")
                                  .arg(realCameraSensor->motionChannelTrigged(4)?1:0)
                                  .arg(realCameraSensor->motionChannelTrigged(3)?1:0)
                                  .arg(realCameraSensor->motionChannelTrigged(2)?1:0)
                                  .arg(realCameraSensor->motionChannelTrigged(1)?1:0));
        sensorFrame->getIconLabel()->setProperty("type", "real_motion_and_lux_camera_sensor");
    }
}
