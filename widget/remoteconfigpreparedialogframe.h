#ifndef REMOTECONFIGPREPAREDIALOGFRAME_H
#define REMOTECONFIGPREPAREDIALOGFRAME_H

#include <QFrame>
#include "callback/abstractcallbackinvoker.h"
#include "callback/abstractcallbackalerter.h"

class PrepareMeshModel;
class TKTMeshModel;

namespace Ui {
class RemoteConfigPrepareDialogFrame;
}

class RemoteConfigPrepareDialogFrame : public QFrame, public AbstractCallbackAlerter, public AbstractCallbackInvoker
{
    Q_OBJECT

public:
    explicit RemoteConfigPrepareDialogFrame(PrepareMeshModel *prepareMeshModel, QWidget *parent = 0);
    ~RemoteConfigPrepareDialogFrame();

    void setStatusText(const QString &text, const QString &color=QString("#000000"));

private:
    Ui::RemoteConfigPrepareDialogFrame *ui;
    PrepareMeshModel *m_prepareMeshModel;
    void setClosable(bool closable);

    quint64 dbDataTotal;
    QString dbMd5;
    quint64 currentOffset;

    void getMeshDBDataOnece();

    void finished(bool ok, const QString &error=QString());

signals:
    void accept();
    void reject();
    void dialogClosableChanged(bool closable);

private slots:
    void on_buttonClose_clicked();
    void doGetMeshDBInfoFinished(bool ok);
    void doGetMeshDBDataFinished(bool ok);
    void doStartGetMeshDBData();
};

#endif // REMOTECONFIGPREPAREDIALOGFRAME_H
