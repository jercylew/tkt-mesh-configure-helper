#include "getmeshscheduleparamscallback.h"
#include "domain/meshruntimeparams.h"

GetMeshScheduleParamsCallback::GetMeshScheduleParamsCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh, bool alert)
    : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_tktMesh=tktMesh;
    this->m_alert=alert;
}

void GetMeshScheduleParamsCallback::taskStarted()
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

void GetMeshScheduleParamsCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    if(m_alert)
    {
        this->alerter()->hidePopupStatusText();
    }
    if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
    {
        QJsonObject data=asyncApiRequestTask->responseJson()->value("data").toObject();
        MeshRuntimeParams params;
        params.setCmdIntervalMsec(data.value("cmd_interval_msec").toInt());
        params.setTwoGroupsSchedulingTimeIntervalSec(data.value("two_groups_schedule_interval_sec").toInt());
        params.setTurnOffAutoNotificationWhenSchedulingIsStarted(data.value("turn_off_auto_notification_when_schedule_started").toBool());
        params.setAutoRefreshNodesDuringMeshInitialization(data.value("auto_refresh_when_mesh_init").toBool());
        params.setThresholdOfBufferedCommandNumberInBusyScheduling(data.value("threshold_of_buffered_command_number_in_busy_schedule").toInt());
        params.setLuminaireMinBrightness(data.value("luminaire_min_brightness").toInt());
        this->invoker()->loadMeshRuntimeParams(&params);
        this->alerter()->setStatusText("获取MESH自动运行参数成功!", "#0000FF");
    }
    else
    {
        this->alerter()->setStatusText("获取MESH自动运行参数失败!", "#FF0000");
    }

    asyncApiRequestTask->destroy();
}
