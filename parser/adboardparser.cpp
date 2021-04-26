#include "adboardparser.h"
#include "domain/adboard.h"
#include "model/meshmodel.h"
#include "model/nodegroupsmodel.h"
#include "widget/adboardframe.h"

ADBoardParser::ADBoardParser()
{
    m_adBoardFrame=NULL;
}

bool ADBoardParser::parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame4BytesSection, Executer *executer)
{
    if(!ExecuterParser::parseOnlineStatus4BytesDataFrameToExecuter(dataFrame4BytesSection, executer))
    {
        return false;
    }
    if(executer->typeText()==ADBoard::staticTypeText())
    {
        ADBoard *ad=static_cast<ADBoard*>(executer);
        if(ad)
        {
            ad->setBrightness(dataFrame4BytesSection.at(2)&0xFF);
        }
    }
    return true;
}

void ADBoardParser::parseUserDataFrameToExecuter(const QByteArray &dataFrame, Executer *executer)
{
    ExecuterParser::parseUserDataFrameToExecuter(dataFrame, executer);
    ADBoard *ad=static_cast<ADBoard*>(executer);
    if(ad)
    {
        ad->setDisplayMode(dataFrame.at(12)&0xFF);
    }
}

QString ADBoardParser::getExecuterTooltipInExecuterListView(Executer *executer)
{
    ADBoard *ad=static_cast<ADBoard*>(executer);
    if(!ad)
    {
        return ExecuterParser::getExecuterTooltipInExecuterListView(executer);
    }

    return QString("%1\nAddress: %2\nBrightness: %3\nDisplay Mode: %4")
            .arg(QObject::tr(ad->displayEnglishTypeText().toLatin1().data()))
            .arg(ad->getBluetoothAddress()&0xFF)
            .arg(ad->getBrightness())
            .arg(ad->displayMode());
}

void ADBoardParser::updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter)
{
    ExecuterParser::updateExecuterFromOnlineStatus(meshModel, newExecuter, oldExecuter);
    ADBoard *newOne=static_cast<ADBoard*>(newExecuter);
    ADBoard *oldOne=static_cast<ADBoard*>(oldExecuter);
    if(newOne!=NULL && oldOne!=NULL)
    {
        oldOne->setBrightness(newOne->getBrightness());
    }
}

void ADBoardParser::updateExecuterFromUserDataNotify(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter)
{
    ExecuterParser::updateExecuterFromUserDataNotify(meshModel, newExecuter, oldExecuter);
    ADBoard *newOne=static_cast<ADBoard*>(newExecuter);
    ADBoard *oldOne=static_cast<ADBoard*>(oldExecuter);
    if(newOne!=NULL && oldOne!=NULL)
    {
        oldOne->setDisplayMode(newOne->displayMode());
    }
}

QPixmap ADBoardParser::buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer)
{
    ADBoard *one=static_cast<ADBoard*>(executer);
    if(one==NULL)
    {
        return ExecuterParser::getSurfaceWithTouchInfo(meshModel, executer);
    }
    if(m_adBoardFrame==NULL)
    {
        m_adBoardFrame=new ADBoardFrame;
    }
    m_adBoardFrame->setGroupColors(meshModel->nodeGroupsModel()->getColorList(executer->getGroupList()));

    if(one->getName().isEmpty())
    {
        m_adBoardFrame->setName(one->executerId());
    }
    else
    {
        m_adBoardFrame->setName(one->getName());
    }
    m_adBoardFrame->setExecuterId(one->executerId());
    if(one->isOnline())
    {
        if(one->getBrightness()==0)
        {
            m_adBoardFrame->getIconLabel()->setProperty("status", "adboard_online_off");
        }
        else
        {
            m_adBoardFrame->getIconLabel()->setProperty("status", "adboard_online_on");
        }
    }
    else
    {
        m_adBoardFrame->getIconLabel()->setProperty("status", "adboard_offline");
    }
    m_adBoardFrame->resetStyle();
    return m_adBoardFrame->grab();
}
