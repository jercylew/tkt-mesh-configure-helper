#include "onechannelluminaireparser.h"

#include "domain/onechannelluminaire.h"
#include "model/meshmodel.h"
#include "model/sensorsmodel.h"
#include "model/executersmodel.h"
#include "model/nodegroupsmodel.h"
#include "widget/onechannelluminaireframe.h"
#include <QPainter>

OneChannelLuminaireParser::OneChannelLuminaireParser()
{
    m_oneChannelLuminaireFrame=NULL;
}

bool OneChannelLuminaireParser::parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame4BytesSection, Executer *executer)
{
    if(!ExecuterParser::parseOnlineStatus4BytesDataFrameToExecuter(dataFrame4BytesSection, executer))
    {
        return false;
    }
    if(executer->typeText()==OneChannelLuminaire::staticTypeText())
    {
        OneChannelLuminaire *one=static_cast<OneChannelLuminaire*>(executer);
        if(one)
        {
            one->setBrightness(dataFrame4BytesSection.at(2)&0xFF);
        }
    }
    return true;
}

void OneChannelLuminaireParser::parseUserDataFrameToExecuter(const QByteArray &dataFrame, Executer *executer)
{
    ExecuterParser::parseUserDataFrameToExecuter(dataFrame, executer);
}

void OneChannelLuminaireParser::updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter)
{
    ExecuterParser::updateExecuterFromOnlineStatus(meshModel, newExecuter, oldExecuter);
    OneChannelLuminaire *newOne=static_cast<OneChannelLuminaire*>(newExecuter);
    OneChannelLuminaire *oldOne=static_cast<OneChannelLuminaire*>(oldExecuter);
    if(newOne!=NULL && oldOne!=NULL)
    {
        oldOne->setBrightness(newOne->getBrightness());
    }
}

QPixmap OneChannelLuminaireParser::buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer)
{
    OneChannelLuminaire *one=static_cast<OneChannelLuminaire*>(executer);
    if(one==NULL)
    {
        return ExecuterParser::getSurfaceWithTouchInfo(meshModel, executer);
    }
    if(m_oneChannelLuminaireFrame==NULL)
    {
        m_oneChannelLuminaireFrame=new OneChannelLuminaireFrame;
    }
    m_oneChannelLuminaireFrame->setGroupColors(meshModel->nodeGroupsModel()->getColorList(executer->getGroupList()));
    m_oneChannelLuminaireFrame->setName(one->getName().isEmpty()?one->executerId():one->getName());

    m_oneChannelLuminaireFrame->setPower(QString("%1W").arg(one->getPower()));
    if(one->isOnline())
    {
        if(one->getBrightness()==0)
        {
            m_oneChannelLuminaireFrame->getIconLabel()->setProperty("status", "light_online_off");
        }
        else
        {
            m_oneChannelLuminaireFrame->getIconLabel()->setProperty("status", "light_online_on");
        }
    }
    else
    {
        m_oneChannelLuminaireFrame->getIconLabel()->setProperty("status", "light_offline");
    }
    m_oneChannelLuminaireFrame->resetStyle();
    return m_oneChannelLuminaireFrame->grab();
}
