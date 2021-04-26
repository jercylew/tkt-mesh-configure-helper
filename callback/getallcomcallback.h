#ifndef GETALLCOMCALLBACK_H
#define GETALLCOMCALLBACK_H

#include "utils/asyncapirequesttask.h"

class TKTMesh;
class AbstractCallbackInvoker;

class GetAllComCallback : public AsyncApiRequestTaskCallback
{
public:
    GetAllComCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

private:
    TKTMesh *m_tktMesh;
};

#endif // GETALLCOMCALLBACK_H
