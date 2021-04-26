#ifndef REFRESHMESHNODESCALLBACK_H
#define REFRESHMESHNODESCALLBACK_H

#include "utils/asyncapirequesttask.h"

class TKTMeshModel;
class AbstractCallbackInvoker;

class RefreshMeshNodesCallback : public AsyncApiRequestTaskCallback
{
public:
    RefreshMeshNodesCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMeshModel *tktMeshModel);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

private:
    TKTMeshModel *m_tktMeshModel;
};
#endif // REFRESHMESHNODESCALLBACK_H
