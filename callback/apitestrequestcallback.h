#ifndef APITESTREQUESTCALLBACK_H
#define APITESTREQUESTCALLBACK_H

#include "utils/asyncapirequesttask.h"
#include "abstractcallbackinvoker.h"
#include "domain/tktmesh.h"

class ApiTestRequestCallback : public AsyncApiRequestTaskCallback
{
    Q_OBJECT
public:
    ApiTestRequestCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

private:
    TKTMesh *m_tktMesh;

signals:
    void getResponse(const QByteArray &response);
};

#endif // APITESTREQUESTCALLBACK_H
