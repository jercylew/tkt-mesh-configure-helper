#ifndef GETMESHSCHEDULEPARAMSCALLBACK_H
#define GETMESHSCHEDULEPARAMSCALLBACK_H

#include <QObject>
#include "utils/asyncapirequesttask.h"

class TKTMesh;
class AbstractCallbackInvoker;

class GetMeshScheduleParamsCallback : public AsyncApiRequestTaskCallback
{
    Q_OBJECT
public:
    explicit GetMeshScheduleParamsCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh, bool alert=true);

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

private:
    TKTMesh *m_tktMesh;
    bool m_alert;
};

#endif // GETMESHSCHEDULEPARAMSCALLBACK_H
