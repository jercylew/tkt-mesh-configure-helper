#ifndef KEEPMESHREMOTECONFIGOPENCALLBACK_H
#define KEEPMESHREMOTECONFIGOPENCALLBACK_H

#include "utils/asyncapirequesttask.h"
#include <QObject>

class AbstractCallbackInvoker;

class KeepMeshRemoteConfigOpenCallback : public AsyncApiRequestTaskCallback
{
    Q_OBJECT
public:
    KeepMeshRemoteConfigOpenCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh, bool firstInvoke=true);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

private:
    bool m_firstInvoke;

signals:
    void firstInvokeKeepMeshRemoteConfigOpenFinished(bool ok);
};

#endif // KEEPMESHREMOTECONFIGOPENCALLBACK_H
