#ifndef GETLUMINAIRECALLBACK_H
#define GETLUMINAIRECALLBACK_H

#include "utils/asyncapirequesttask.h"
#include "abstractcallbackinvoker.h"
#include "domain/tktmesh.h"

class GetLuminaireCallback : public AsyncApiRequestTaskCallback
{
public:
    GetLuminaireCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

private:
    TKTMesh *m_tktMesh;
};

#endif // GETLUMINAIRECALLBACK_H
