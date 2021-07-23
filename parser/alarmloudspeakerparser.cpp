#include "alarmloudspeakerparser.h"

#include "domain/alarmloudspeaker.h"
#include "model/meshmodel.h"
#include "model/sensorsmodel.h"
#include "model/executersmodel.h"
#include "model/nodegroupsmodel.h"
#include "widget/alarmloudspeakerframe.h"

AlarmLoudspeakerParser::AlarmLoudspeakerParser()
{
    m_alarmLoudspeakerFrame = nullptr;
}

bool AlarmLoudspeakerParser::parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame4BytesSection, Executer *executer)
{
    if(!ExecuterParser::parseOnlineStatus4BytesDataFrameToExecuter(dataFrame4BytesSection, executer))
    {
        return false;
    }
    if(executer->typeText() == AlarmLoudspeaker::staticTypeText())
    {
        AlarmLoudspeaker *one = static_cast<AlarmLoudspeaker*>(executer);
        if(one)
        {
            one->setOn(dataFrame4BytesSection.at(2)!=0);
        }
    }
    return true;
}

void AlarmLoudspeakerParser::updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter)
{
    ExecuterParser::updateExecuterFromOnlineStatus(meshModel, newExecuter, oldExecuter);
    AlarmLoudspeaker *newOne = static_cast<AlarmLoudspeaker*>(newExecuter);
    AlarmLoudspeaker *oldOne = static_cast<AlarmLoudspeaker*>(oldExecuter);

    if(newOne != NULL && oldOne != NULL)
    {
        oldOne->setOn(newOne->on());
    }
}

void AlarmLoudspeakerParser::updateExecuterFromUserDataNotify(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter)
{
    ExecuterParser::updateExecuterFromUserDataNotify(meshModel, newExecuter, oldExecuter);
    AlarmLoudspeaker *newOne = static_cast<AlarmLoudspeaker*>(newExecuter);
    AlarmLoudspeaker *oldOne = static_cast<AlarmLoudspeaker*>(oldExecuter);

    if (newOne != nullptr && oldOne != nullptr)
    {
        oldOne->setOn(newOne->on());
    }
}

QPixmap AlarmLoudspeakerParser::buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer)
{
    AlarmLoudspeaker *one = static_cast<AlarmLoudspeaker*>(executer);
    if(one == NULL)
    {
        return ExecuterParser::getSurfaceWithTouchInfo(meshModel, executer);
    }
    if(m_alarmLoudspeakerFrame == nullptr)
    {
        m_alarmLoudspeakerFrame = new AlarmLoudspeakerFrame;
    }
    m_alarmLoudspeakerFrame->setGroupColors(meshModel->nodeGroupsModel()->getColorList(executer->getGroupList()));
    m_alarmLoudspeakerFrame->setName(one->getName().isEmpty()?one->executerId():one->getName());
    if(one->isOnline())
    {
        if(one->on())
        {
            m_alarmLoudspeakerFrame->getIconLabel()->setProperty("status", "alarm_loudspeaker_online_on");
        }
        else
        {
            m_alarmLoudspeakerFrame->getIconLabel()->setProperty("status", "alarm_loudspeaker_online_off");
        }
    }
    else
    {
        m_alarmLoudspeakerFrame->getIconLabel()->setProperty("status", "alarm_loudspeaker_offline");
    }
    m_alarmLoudspeakerFrame->resetStyle();
    return m_alarmLoudspeakerFrame->grab();
}

