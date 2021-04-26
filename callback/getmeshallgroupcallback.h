#ifndef GETMESHALLGROUPCALLBACK_H
#define GETMESHALLGROUPCALLBACK_H

#include "utils/asyncapirequesttask.h"
#include "abstractcallbackinvoker.h"

class TKTMeshModel;

class GetMeshAllGroupCallback : public AsyncApiRequestTaskCallback
{
public:
    GetMeshAllGroupCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMeshModel *tktMeshModel);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

private:
    TKTMeshModel *m_tktMeshModel;
};

#endif // GETMESHALLGROUPCALLBACK_H
