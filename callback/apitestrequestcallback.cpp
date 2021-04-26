#include "apitestrequestcallback.h"
#include "utils/tools.h"
#include <QJsonArray>

ApiTestRequestCallback::ApiTestRequestCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter)
    : AsyncApiRequestTaskCallback(invoker, alerter)
{
}

void ApiTestRequestCallback::taskStarted()
{
    this->alerter()->showPopupStatusText("执行中...", true);
}

void ApiTestRequestCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    this->alerter()->hidePopupStatusText();
    emit getResponse(Tools::getDataArrayOfJsonObject(asyncApiRequestTask->responseJson()));
    if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
    {
        this->alerter()->setStatusText("执行成功!", "#0000FF");
    }
    else
    {
        this->alerter()->setStatusText("执行失败!", "#FF0000");
    }

    asyncApiRequestTask->destroy();
}
