#include "onechannelrelayparser.h"

#include "domain/onechannelrelay.h"
#include "model/meshmodel.h"
#include "model/sensorsmodel.h"
#include "model/executersmodel.h"
#include "model/nodegroupsmodel.h"
#include "widget/onechannelrelayframe.h"
#include <QPainter>

OneChannelRelayParser::OneChannelRelayParser()
{
    m_oneChannelRelayFrame=NULL;
}

bool OneChannelRelayParser::parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame4BytesSection, Executer *executer)
{
    if(!ExecuterParser::parseOnlineStatus4BytesDataFrameToExecuter(dataFrame4BytesSection, executer))
    {
        return false;
    }
    if(executer->typeText()==OneChannelRelay::staticTypeText())
    {
        OneChannelRelay *one=static_cast<OneChannelRelay*>(executer);
        if(one)
        {
            one->setOn(dataFrame4BytesSection.at(2)!=0);
        }
    }
    return true;
}

void OneChannelRelayParser::updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter)
{
    ExecuterParser::updateExecuterFromOnlineStatus(meshModel, newExecuter, oldExecuter);
    OneChannelRelay *newOne=static_cast<OneChannelRelay*>(newExecuter);
    OneChannelRelay *oldOne=static_cast<OneChannelRelay*>(oldExecuter);
    if(newOne!=NULL && oldOne!=NULL)
    {
        oldOne->setOn(newOne->on());
    }
}

QPixmap OneChannelRelayParser::buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer)
{
    OneChannelRelay *one=static_cast<OneChannelRelay*>(executer);
    if(one==NULL)
    {
        return ExecuterParser::getSurfaceWithTouchInfo(meshModel, executer);
    }
    if(m_oneChannelRelayFrame==NULL)
    {
        m_oneChannelRelayFrame=new OneChannelRelayFrame;
    }
    m_oneChannelRelayFrame->setGroupColors(meshModel->nodeGroupsModel()->getColorList(executer->getGroupList()));
    m_oneChannelRelayFrame->setName(one->getName().isEmpty()?one->executerId():one->getName());
    if(one->isOnline())
    {
        if(one->on())
        {
            m_oneChannelRelayFrame->getIconLabel()->setProperty("status", "1ch_relay_online_on");
        }
        else
        {
            m_oneChannelRelayFrame->getIconLabel()->setProperty("status", "1ch_relay_online_off");
        }
    }
    else
    {
        m_oneChannelRelayFrame->getIconLabel()->setProperty("status", "1ch_relay_offline");
    }
    m_oneChannelRelayFrame->resetStyle();
    return m_oneChannelRelayFrame->grab();
}
