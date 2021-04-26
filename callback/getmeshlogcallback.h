#ifndef GETMESHLOGCALLBACK_H
#define GETMESHLOGCALLBACK_H

#include <QObject>
#include "utils/asyncapirequesttask.h"

class TKTMesh;
class AbstractCallbackInvoker;

class GetMeshLogCallback : public AsyncApiRequestTaskCallback
{
public:
    GetMeshLogCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh, bool alert=true);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

private:
    TKTMesh *m_tktMesh;
    bool m_alert;
};

#endif // GETMESHLOGCALLBACK_H
