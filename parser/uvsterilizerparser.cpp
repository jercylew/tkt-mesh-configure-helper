#include "uvsterilizerparser.h"
#include "domain/uvsterilizer.h"
#include "model/meshmodel.h"
#include "model/sensorsmodel.h"
#include "model/executersmodel.h"
#include "model/nodegroupsmodel.h"
#include "widget/uvsterilizerframe.h"

UVSterilizerParser::UVSterilizerParser()
{
    m_ptrUVSterilizerFrame = nullptr;
}

bool UVSterilizerParser::parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame4BytesSection, Executer *executer)
{
    if(!ExecuterParser::parseOnlineStatus4BytesDataFrameToExecuter(dataFrame4BytesSection, executer))
    {
        return false;
    }

    //From light status??
//    if(executer->typeText() == UVSterilizer::staticTypeText())
//    {
//        UVSterilizer *one = static_cast<UVSterilizer*>(executer);
//        if(one != nullptr)
//        {
//            one->setWifiAddress(dataFrame4BytesSection.at(2));
//            one->setConfigState(dataFrame4BytesSection.at(2)!=0);
//        }
//    }
    return true;
}

void UVSterilizerParser::updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter)
{
    ExecuterParser::updateExecuterFromOnlineStatus(meshModel, newExecuter, oldExecuter);
    UVSterilizer *newOne = static_cast<UVSterilizer*>(newExecuter);
    UVSterilizer *oldOne = static_cast<UVSterilizer*>(oldExecuter);

    if (newOne != nullptr && oldOne != nullptr)
    {
        bool bOldConfigState = oldOne->configState();
        bool bOldSterilizeState = oldOne->sterilizeState();
        qint8 nOldWifiAddress = oldOne->wifiAddress();

        oldOne->setConfigState(newOne->configState());
        oldOne->setSterilizeState(newOne->sterilizeState());
        oldOne->setWifiAddress(newOne->wifiAddress());
        oldOne->setFuncCode(newOne->funcCode());

//        if (bOldConfigState != oldOne->configState() ||
//                bOldSterilizeState != oldOne->sterilizeState() ||
//                nOldWifiAddress != oldOne->wifiAddress())
//        {
//            oldOne->setDataChanged(true);
//        }
    }
}

void UVSterilizerParser::updateExecuterFromUserDataNotify(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter)
{
    ExecuterParser::updateExecuterFromUserDataNotify(meshModel, newExecuter, oldExecuter);
    UVSterilizer *newOne = static_cast<UVSterilizer*>(newExecuter);
    UVSterilizer *oldOne = static_cast<UVSterilizer*>(oldExecuter);

    if (newOne != nullptr && oldOne != nullptr)
    {
        bool bOldConfigState = oldOne->configState();
        bool bOldSterilizeState = oldOne->sterilizeState();
        qint8 nOldWifiAddress = oldOne->wifiAddress();

        oldOne->setConfigState(newOne->configState());
        oldOne->setSterilizeState(newOne->sterilizeState());
        oldOne->setWifiAddress(newOne->wifiAddress());
        oldOne->setFuncCode(newOne->funcCode());

//        if (bOldConfigState != oldOne->configState() ||
//                bOldSterilizeState != oldOne->sterilizeState() ||
//                nOldWifiAddress != oldOne->wifiAddress())
//        {
//            oldOne->setDataChanged(true);
//        }
    }
}

void UVSterilizerParser::parseUserDataFrameToExecuter(const QByteArray &dataFrame, Executer *executer)
{
    ExecuterParser::parseUserDataFrameToExecuter(dataFrame, executer);
    UVSterilizer *uvs = static_cast<UVSterilizer*>(executer);
    if(uvs != nullptr)
    {
        uvs->setWifiAddress(dataFrame.at(12));
        quint8 nFunCode = dataFrame.at(13);
        uvs->setFuncCode(nFunCode);
        if (nFunCode == 0x02)
        {
            uvs->setConfigState(dataFrame.at(14) != 0);
        }
        else if (nFunCode == 0x03)
        {
            uvs->setSterilizeState(dataFrame.at(14) != 0);
        }
        else
        {
            qWarning() << "UVSterilizerParser::parseUserDataFrameToExecuter"
                       << "Unknown function code:" << nFunCode;
        }
    }
}

QPixmap UVSterilizerParser::buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer)
{
    UVSterilizer *one=static_cast<UVSterilizer*>(executer);
    if(one == nullptr)
    {
        return ExecuterParser::getSurfaceWithTouchInfo(meshModel, executer);
    }

    if(m_ptrUVSterilizerFrame == nullptr)
    {
        m_ptrUVSterilizerFrame = new UVSterilizerFrame;
    }
    m_ptrUVSterilizerFrame->setGroupColors(meshModel->nodeGroupsModel()->getColorList(executer->getGroupList()));
    m_ptrUVSterilizerFrame->setName(one->getName().isEmpty()?one->executerId():one->getName());

    QString strProperty;
    if(one->isOnline())
    {
        qint8 nFuncCode = one->funcCode();
        if (nFuncCode == 0x02)
        {
            strProperty = one->configState() ?
                        "uv_sterilizer_online_appointed_ok" :
                        "uv_sterilizer_online_appointed_failed";

        }
        else if (nFuncCode == 0x03)
        {
            strProperty = one->sterilizeState() ?
                        "uv_sterilizer_online_sterilized" :
                        "uv_sterilizer_online_sterilizing";
        }
        else
        {
            strProperty = "uv_sterilizer_offline";
        }
    }
    else
    {
        strProperty = "uv_sterilizer_offline";
    }

    m_ptrUVSterilizerFrame->getIconLabel()->setProperty("status", strProperty);
    m_ptrUVSterilizerFrame->resetStyle();
    return m_ptrUVSterilizerFrame->grab();
}
