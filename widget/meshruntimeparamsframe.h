#ifndef MESHRUNTIMEPARAMSFRAME_H
#define MESHRUNTIMEPARAMSFRAME_H

#include <QFrame>
#include "domain/meshruntimeparams.h"
#include "callback/abstractcallbackinvoker.h"

class TKTMeshModel;
class AbstractCallbackAlerter;

namespace Ui {
class MeshRuntimeParamsFrame;
}

class MeshRuntimeParamsFrame : public QFrame, public AbstractCallbackInvoker
{
    Q_OBJECT

public:
    explicit MeshRuntimeParamsFrame(TKTMeshModel *tktMeshModel, const QString &meshUUID, AbstractCallbackAlerter *alerter, QWidget *parent = 0);
    ~MeshRuntimeParamsFrame();

    void loadMeshRuntimeParams(MeshRuntimeParams *params);

private:
    Ui::MeshRuntimeParamsFrame *ui;
    TKTMeshModel *m_tktMeshModel;
    QString m_meshUUID;
    AbstractCallbackAlerter *m_alerter;

signals:
    void backToTKTMeshPage();

private slots:
    void on_buttonMeshClose_clicked();
    void on_buttonMeshRuntimeParamsRefresh_clicked();
    void on_buttonMeshRuntimeParamsSave_clicked();
};

#endif // MESHRUNTIMEPARAMSFRAME_H
