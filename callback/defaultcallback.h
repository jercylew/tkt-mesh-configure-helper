#ifndef DEFAULTCALLBACK_H
#define DEFAULTCALLBACK_H

#include "utils/asyncapirequesttask.h"

class TKTMesh;
class AbstractCallbackInvoker;

class DefaultCallback : public AsyncApiRequestTaskCallback
{
public:
    DefaultCallback(AbstractCallbackInvoker *invoker, AbstractCallbackAlerter *alerter, TKTMesh *tktMesh, const QString &successText=QString(), const QString &failedText=QString());

    void taskStarted();
    void taskFinished(bool ok, AsyncApiRequestTask *asyncApiRequestTask);

private:
    TKTMesh *m_tktMesh;
    QString m_successText;
    QString m_failedText;
};
#endif // DEFAULTCALLBACK_H
