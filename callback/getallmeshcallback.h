#ifndef GETALLMESHCALLBACK_H
#define GETALLMESHCALLBACK_H

#include "utils/asyncapirequesttask.h"

class TKTMesh;
class AbstractCallbackInvoker;

class GetAllMeshCallback : public AsyncApiRequestTaskCallback
{
public:
    GetAllMeshCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

private:
    TKTMesh *m_tktMesh;
};

#endif // GETALLMESHCALLBACK_H
