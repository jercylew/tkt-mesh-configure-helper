#ifndef SETHOSTINFOCALLBACK_H
#define SETHOSTINFOCALLBACK_H


#include "utils/asyncapirequesttask.h"
#include "abstractcallbackinvoker.h"
#include "domain/tktmesh.h"

class SetHostInfoCallback : public AsyncApiRequestTaskCallback
{
public:
    SetHostInfoCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

private:
    TKTMesh *m_tktMesh;
};

#endif // SETHOSTINFOCALLBACK_H
