#include "getmeshallgroupcallback.h"

#include "domain/tktmesh.h"
#include "model/tktmeshmodel.h"
#include <QJsonArray>
#include "domain/nodegroup.h"

GetMeshAllGroupCallback::GetMeshAllGroupCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMeshModel *tktMeshModel)
    : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_tktMeshModel=tktMeshModel;
}

void GetMeshAllGroupCallback::taskStarted()
{
    this->alerter()->showPopupStatusText("获取中 ...", true);
}

void GetMeshAllGroupCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    this->alerter()->hidePopupStatusText();
    if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
    {
        QJsonArray array=asyncApiRequestTask->responseJson()->value("data").toArray();
        QList<NodeGroup*> groupList;
        for(int i=0; i<array.size(); i++)
        {
            QJsonObject obj=array.at(i).toObject();
            NodeGroup *nodeGroup=new NodeGroup;
            nodeGroup->setId(obj.value("group_id").toInt());
            nodeGroup->setName(obj.value("name").toString());
            nodeGroup->setColor(obj.value("color").toString());
            nodeGroup->setScheduleType(NodeGroup::getScheduleTypeByText(obj.value("schedule_type").toString()));
            groupList.push_back(nodeGroup);
        }
        this->invoker()->updateNodeGroups(groupList);
        this->alerter()->setStatusText("获取Mesh所有控制组列表成功!", "#0000FF");
    }
    else
    {
        this->alerter()->setStatusText("获取控制组列表失败!", "#FF0000");
    }

    asyncApiRequestTask->destroy();
}
