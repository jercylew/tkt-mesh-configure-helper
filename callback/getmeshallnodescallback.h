#ifndef GETMESHALLNODESCALLBACK_H
#define GETMESHALLNODESCALLBACK_H

#include "utils/asyncapirequesttask.h"
#include "abstractcallbackinvoker.h"
#include "domain/tktmesh.h"

class TKTMeshModel;

class GetMeshAllNodesCallback : public AsyncApiRequestTaskCallback
{
public:
    GetMeshAllNodesCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMeshModel *tktMeshModel);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

private:
    TKTMeshModel *m_tktMeshModel;
};

#endif // GETMESHALLNODESCALLBACK_H
