#include "warninglightparser.h"
#include "domain/warninglight.h"
#include "model/meshmodel.h"
#include "model/sensorsmodel.h"
#include "model/executersmodel.h"
#include "model/nodegroupsmodel.h"
#include "widget/warninglightframe.h"
#include <QPainter>

WarningLightParser::WarningLightParser()
{
    m_warningLightFrame=NULL;
}

bool WarningLightParser::parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame4BytesSection, Executer *executer)
{
    if(!ExecuterParser::parseOnlineStatus4BytesDataFrameToExecuter(dataFrame4BytesSection, executer))
    {
        return false;
    }
    if(executer->typeText()==WarningLight::staticTypeText())
    {
        WarningLight *one=static_cast<WarningLight*>(executer);
        if(one)
        {
            one->setBrightness(dataFrame4BytesSection.at(2)&0xFF);
        }
    }
    return true;
}

void WarningLightParser::parseUserDataFrameToExecuter(const QByteArray &dataFrame, Executer *executer)
{
    ExecuterParser::parseUserDataFrameToExecuter(dataFrame, executer);
}

void WarningLightParser::updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter)
{
    ExecuterParser::updateExecuterFromOnlineStatus(meshModel, newExecuter, oldExecuter);
    WarningLight *newOne=static_cast<WarningLight*>(newExecuter);
    WarningLight *oldOne=static_cast<WarningLight*>(oldExecuter);
    if(newOne!=NULL && oldOne!=NULL)
    {
        oldOne->setBrightness(newOne->getBrightness());
    }
}

QPixmap WarningLightParser::buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer)
{
    WarningLight *one=static_cast<WarningLight*>(executer);
    if(one==NULL)
    {
        return ExecuterParser::getSurfaceWithTouchInfo(meshModel, executer);
    }
    if(m_warningLightFrame==NULL)
    {
        m_warningLightFrame=new WarningLightFrame;
    }
    m_warningLightFrame->setGroupColors(meshModel->nodeGroupsModel()->getColorList(executer->getGroupList()));
    m_warningLightFrame->setName(one->getName().isEmpty()?one->executerId():one->getName());

    m_warningLightFrame->setPower(QString("%1W").arg(one->getPower()));
    if(one->isOnline())
    {
        if(one->getBrightness()==0)
        {
            m_warningLightFrame->getIconLabel()->setProperty("status", "warning_light_online_off");
        }
        else
        {
            m_warningLightFrame->getIconLabel()->setProperty("status", "warning_light_online_on");
        }
    }
    else
    {
        m_warningLightFrame->getIconLabel()->setProperty("status", "warning_light_offline");
    }
    m_warningLightFrame->resetStyle();
    return m_warningLightFrame->grab();
}

