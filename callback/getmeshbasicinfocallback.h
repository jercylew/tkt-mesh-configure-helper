#ifndef GETDEFAULTMESHCALLBACK_H
#define GETDEFAULTMESHCALLBACK_H

#include "utils/asyncapirequesttask.h"

class TKTMesh;
class AbstractCallbackInvoker;

class GetMeshBasicInfoCallback : public AsyncApiRequestTaskCallback
{
public:
    GetMeshBasicInfoCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh, bool alert=true);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

private:
    TKTMesh *m_tktMesh;
    bool m_alert;
};

#endif // GETDEFAULTMESHCALLBACK_H
