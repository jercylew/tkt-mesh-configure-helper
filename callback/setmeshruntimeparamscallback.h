#ifndef SETMESHRUNTIMEPARAMSCALLBACK_H
#define SETMESHRUNTIMEPARAMSCALLBACK_H

#include "utils/asyncapirequesttask.h"

class TKTMesh;
class AbstractCallbackInvoker;

class SetMeshRuntimeParamsCallback : public AsyncApiRequestTaskCallback
{
    Q_OBJECT
public:
    explicit SetMeshRuntimeParamsCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

private:
    TKTMesh *m_tktMesh;
};

#endif // SETMESHRUNTIMEPARAMSCALLBACK_H
