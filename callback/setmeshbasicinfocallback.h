#ifndef SETMESHBASICINFOCALLBACK_H
#define SETMESHBASICINFOCALLBACK_H

#include "utils/asyncapirequesttask.h"

class TKTMesh;
class AbstractCallbackInvoker;

class SetMeshBasicInfoCallback : public AsyncApiRequestTaskCallback
{
public:
    SetMeshBasicInfoCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

private:
    TKTMesh *m_tktMesh;
};
#endif // SETMESHBASICINFOCALLBACK_H
