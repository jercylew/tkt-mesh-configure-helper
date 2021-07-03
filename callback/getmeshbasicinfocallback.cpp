#include "getmeshbasicinfocallback.h"
#include "abstractcallbackinvoker.h"
#include "domain/mesh.h"

GetMeshBasicInfoCallback::GetMeshBasicInfoCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh, bool alert)
    : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_tktMesh=tktMesh;
    this->m_alert=alert;
}

void GetMeshBasicInfoCallback::taskStarted()
{
    if(m_alert)
    {
        this->alerter()->showPopupStatusText("获取中 ...", true);
    }
    else
    {
        this->alerter()->setStatusText("获取中...", "#000000");
    }
}

void GetMeshBasicInfoCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    if(m_alert)
    {
        this->alerter()->hidePopupStatusText();
    }
    if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
    {
        QJsonObject data=asyncApiRequestTask->responseJson()->value("data").toObject();
        Mesh mesh;
        mesh.setMeshUUID(data.value("mesh_uuid").toString());
        mesh.setMeshName(data.value("mesh_name").toString());
        mesh.setAutoScheduleIsOn(data.value("auto_schedule_is_on").toBool());
        mesh.setComName(data.value("com_name").toString());
        mesh.setComIsOpened(data.value("com_is_opened").toBool());
        mesh.setReadFrames(data.value("read_frames").toInt());
        mesh.setComIsEnabled(data.value("com_is_enabled").toBool());
        mesh.setBindComNameToMeshName(data.value("bind_com_name_to_mesh_name").toBool(false));
        mesh.setGatewayMeshAddress((quint8)(data.value("mesh_address").toInt()));
        mesh.setGatewayMeshName(data.value("mesh_name").toString());
        mesh.setGatewayMeshPassword(data.value("mesh_password").toString());
        mesh.setGatewayMeshLtk(data.value("mesh_ltk").toString());
        this->invoker()->loadMesh(&mesh);
        this->alerter()->setStatusText("获取默认Mesh信息成功!", "#0000FF");
    }
    else
    {
        this->alerter()->setStatusText("获取默认Mesh信息失败!", "#FF0000");
    }

    asyncApiRequestTask->destroy();
}
