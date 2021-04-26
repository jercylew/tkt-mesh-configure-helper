#include "getluminairecallback.h"
#include "domain/tktmesh.h"
#include "domain/luminaire.h"
#include <QJsonArray>

GetLuminaireCallback::GetLuminaireCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh)
    : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_tktMesh=tktMesh;
}

void GetLuminaireCallback::taskStarted()
{
    this->alerter()->showPopupStatusText("获取中 ...", true);
}

void GetLuminaireCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    this->alerter()->hidePopupStatusText();
    if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
    {
//        QJsonObject data=asyncApiRequestTask->responseJson()->value("data").toObject();
//        Luminaire luminaire;
//        luminaire.setNodeId(data.value("node_id").toString());
//        luminaire.setBluetoothAddress(data.value("bluetooth_address").toInt());
//        luminaire.setTypeText(data.value("type_text").toString());
//        luminaire.setOnline(data.value("online").toBool());
//        luminaire.setRegistered(data.value("registered").toBool());
//        QJsonArray groupArray=data.value("groups").toArray();
//        QList<quint8> groupList;
//        for(int j=0; j<groupArray.size(); j++)
//        {
//            groupList.push_back(groupArray.at(j).toInt());
//        }
//        luminaire.setGroupList(groupList);
//        luminaire.setIsOn(data.value("brightness").toInt()>0);
//        luminaire.setBrightness(data.value("brightness").toInt());
//        this->alerter()->setStatusText(QObject::tr("获取灯具信息成功!"));
//        this->invoker()->loadLuminaire(&luminaire);
    }
    else
    {
        this->alerter()->setStatusText("获取灯具信息失败!", "#FF0000");
    }

    asyncApiRequestTask->destroy();
}
