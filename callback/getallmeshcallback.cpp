#include "getallmeshcallback.h"
#include <QJsonArray>
#include "domain/mesh.h"

GetAllMeshCallback::GetAllMeshCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh)
    : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_tktMesh=tktMesh;
}

void GetAllMeshCallback::taskStarted()
{
    this->alerter()->showPopupStatusText("获取中 ...", true);
}

void GetAllMeshCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    this->alerter()->hidePopupStatusText();

    if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
    {
        QJsonArray jsonArray=asyncApiRequestTask->responseJson()->value("data").toArray();
        QList<Mesh *> meshList;
        for(int i=0; i<jsonArray.size(); i++)
        {
            QJsonObject jsonObj=jsonArray.at(i).toObject();
            QString meshUUID=jsonObj.value("mesh_uuid").toString();
            if(meshUUID.isEmpty())
            {
                continue;
            }
            Mesh *mesh=new Mesh;
            mesh->setMeshUUID(meshUUID);
            mesh->setMeshName(jsonObj.value("mesh_name").toString());
            mesh->setAutoScheduleIsOn(jsonObj.value("auto_schedule_is_on").toBool());
            mesh->setComName(jsonObj.value("com_name").toString());
            mesh->setComIsOpened(jsonObj.value("com_is_opened").toBool());
            mesh->setComIsEnabled(jsonObj.value("com_is_enabled").toBool());
            mesh->setBindComNameToMeshName(jsonObj.value("bind_com_name_to_mesh_name").toBool(false));
            mesh->setGatewayMeshAddress((quint8)(jsonObj.value("mesh_address").toInt()));
            mesh->setGatewayMeshName(jsonObj.value("mesh_name").toString());
            mesh->setGatewayMeshPassword(jsonObj.value("mesh_password").toString());
            mesh->setGatewayMeshLtk(jsonObj.value("mesh_ltk").toString());
            meshList.push_back(mesh);
        }
        this->invoker()->loadMeshList(meshList);
        this->alerter()->setStatusText("获取默认Mesh信息成功!", "#0000FF");
    }
    else
    {
        this->alerter()->setStatusText("获取默认Mesh信息失败!", "#FF0000");
    }

    asyncApiRequestTask->destroy();
}
