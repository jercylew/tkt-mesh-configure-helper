#include "drycontactrelayparser.h"

#include "domain/drycontactrelay.h"
#include "model/meshmodel.h"
#include "model/sensorsmodel.h"
#include "model/executersmodel.h"
#include "model/nodegroupsmodel.h"
#include "widget/drycontactrelayframe.h"

DryContactRelayParser::DryContactRelayParser()
{
    m_dryContactRelayFrame = nullptr;
}

bool DryContactRelayParser::parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame4BytesSection, Executer *executer)
{
    if(!ExecuterParser::parseOnlineStatus4BytesDataFrameToExecuter(dataFrame4BytesSection, executer))
    {
        return false;
    }
    if(executer->typeText()==DryContactRelay::staticTypeText())
    {
        DryContactRelay *one=static_cast<DryContactRelay*>(executer);
        if(one)
        {
            one->setOn(dataFrame4BytesSection.at(2)!=0);
        }
    }
    return true;
}

void DryContactRelayParser::updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter)
{
    ExecuterParser::updateExecuterFromOnlineStatus(meshModel, newExecuter, oldExecuter);
    DryContactRelay *newOne=static_cast<DryContactRelay*>(newExecuter);
    DryContactRelay *oldOne=static_cast<DryContactRelay*>(oldExecuter);

    if(newOne!=NULL && oldOne!=NULL)
    {
        oldOne->setOn(newOne->on());
    }
}

void DryContactRelayParser::updateExecuterFromUserDataNotify(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter)
{
    ExecuterParser::updateExecuterFromUserDataNotify(meshModel, newExecuter, oldExecuter);
    DryContactRelay *newOne = static_cast<DryContactRelay*>(newExecuter);
    DryContactRelay *oldOne = static_cast<DryContactRelay*>(oldExecuter);

    if (newOne != nullptr && oldOne != nullptr)
    {
        oldOne->setOn(newOne->on());
    }
}

QPixmap DryContactRelayParser::buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer)
{
    DryContactRelay *one=static_cast<DryContactRelay*>(executer);
    if(one==NULL)
    {
        return ExecuterParser::getSurfaceWithTouchInfo(meshModel, executer);
    }
    if(m_dryContactRelayFrame==nullptr)
    {
        m_dryContactRelayFrame = new DryContactRelayFrame;
    }
    m_dryContactRelayFrame->setGroupColors(meshModel->nodeGroupsModel()->getColorList(executer->getGroupList()));
    m_dryContactRelayFrame->setName(one->getName().isEmpty()?one->executerId():one->getName());
    if(one->isOnline())
    {
        if(one->on())
        {
            m_dryContactRelayFrame->getIconLabel()->setProperty("status", "dry_contact_relay_online_on");
        }
        else
        {
            m_dryContactRelayFrame->getIconLabel()->setProperty("status", "dry_contact_relay_online_off");
        }
    }
    else
    {
        m_dryContactRelayFrame->getIconLabel()->setProperty("status", "dry_contact_relay_offline");
    }
    m_dryContactRelayFrame->resetStyle();
    return m_dryContactRelayFrame->grab();
}
