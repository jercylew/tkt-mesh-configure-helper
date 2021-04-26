#include "getmeshallnodescallback.h"

#include "domain/tktmesh.h"
#include "model/tktmeshmodel.h"
#include <QJsonArray>
#include "domain/meshdevicenode.h"

GetMeshAllNodesCallback::GetMeshAllNodesCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMeshModel *tktMeshModel)
    : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_tktMeshModel=tktMeshModel;
}

void GetMeshAllNodesCallback::taskStarted()
{
    this->alerter()->showPopupStatusText("获取中 ...", true);
}

void GetMeshAllNodesCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    this->alerter()->hidePopupStatusText();
    if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
    {
        QJsonArray array=asyncApiRequestTask->responseJson()->value("data").toArray();
        QList<MeshDeviceNode*> nodeList;
        for(int i=0; i<array.size(); i++)
        {
            QJsonObject obj=array.at(i).toObject();
            MeshDeviceNode *node=new MeshDeviceNode;
            node->setNodeId(obj.value("node_id").toString());
            node->setBluetoothAddress(obj.value("bluetooth_address").toInt());
            node->setTypeText(obj.value("type_text").toString());
            node->setOnline(obj.value("online").toBool());
            node->setRegistered(obj.value("registered").toBool());
            QJsonArray groupArray=obj.value("groups").toArray();
            QList<quint8> groupList;
            for(int j=0; j<groupArray.size(); j++)
            {
                groupList.push_back(groupArray.at(j).toInt());
            }
            node->setGroupList(groupList);
            node->setDataText(obj.value("data_text").toString());
            nodeList.push_back(node);
        }
        this->invoker()->updateMeshNodes(&nodeList);
        this->alerter()->setStatusText("获取Mesh所有节点列表成功!", "#0000FF");
    }
    else
    {
        this->alerter()->setStatusText("获取节点列表失败!", "#FF0000");
    }

    asyncApiRequestTask->destroy();
}
