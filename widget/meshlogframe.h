#ifndef MESHLOGFRAME_H
#define MESHLOGFRAME_H

#include <QFrame>
#include "callback/abstractcallbackinvoker.h"

class TKTMeshModel;
class AbstractCallbackAlerter;

namespace Ui {
class MeshLogFrame;
}

class MeshLogFrame : public QFrame, public AbstractCallbackInvoker
{
    Q_OBJECT

public:
    explicit MeshLogFrame(TKTMeshModel *tktMeshModel, const QString &meshUUID, AbstractCallbackAlerter *alerter, QWidget *parent = 0);
    ~MeshLogFrame();

    void loadMeshLog(QList<QString> *meshLogList);

private slots:
    void on_buttonClear_clicked();

    void on_buttonRefresh_clicked();

    void on_buttonMeshClose_clicked();

private:
    Ui::MeshLogFrame *ui;
    QString m_meshUUID;
    TKTMeshModel *m_tktMeshModel;
    AbstractCallbackAlerter *m_alerter;

signals:
    void backToTKTMeshPage();
};

#endif // MESHLOGFRAME_H
