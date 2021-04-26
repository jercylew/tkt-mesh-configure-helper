#ifndef GETHOSTINFOCALLBACK_H
#define GETHOSTINFOCALLBACK_H

#include "utils/asyncapirequesttask.h"
#include "abstractcallbackinvoker.h"
#include "domain/tktmesh.h"

class GetHostInfoCallback : public AsyncApiRequestTaskCallback
{
public:
    GetHostInfoCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

private:
    TKTMesh *m_tktMesh;
};

#endif // GETHOSTINFOCALLBACK_H
