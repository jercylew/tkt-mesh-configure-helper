#include "defaultcallback.h"

#include "abstractcallbackinvoker.h"
#include "domain/mesh.h"

DefaultCallback::DefaultCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh, const QString &successText, const QString &failedText)
    : AsyncApiRequestTaskCallback(invoker, alerter)
{
    this->m_tktMesh=tktMesh;
    this->m_successText=successText;
    this->m_failedText=failedText;
}

void DefaultCallback::taskStarted()
{
    if(this->alerter()!=NULL)
    {
        this->alerter()->showPopupStatusText("正在处理中 ...", true);
    }
}

void DefaultCallback::taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask)
{
    if(this->alerter()!=NULL)
    {
        this->alerter()->hidePopupStatusText();
        if(ok && asyncApiRequestTask->responseJson()->value("result").toString()=="success")
        {
            this->alerter()->setStatusText(m_successText, "#0000FF");
        }
        else
        {
            QString error=asyncApiRequestTask->responseJson()->value("data").toString();
            if(error.isEmpty())
            {
                this->alerter()->setStatusText(m_failedText, "#FF0000");
            }
            else
            {
                this->alerter()->setStatusText(QString("%1 [%2]").arg(m_failedText).arg(error), "#FF0000");
            }
        }
    }
    asyncApiRequestTask->destroy();
}
