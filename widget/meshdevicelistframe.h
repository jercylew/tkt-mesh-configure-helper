#ifndef MESHDEVICELISTFRAME_H
#define MESHDEVICELISTFRAME_H

#include <QFrame>
#include <QMenu>
#include "callback/abstractcallbackinvoker.h"
#include "callback/abstractcallbackalerter.h"

class TKTMeshModel;
class MeshNodesModel;

namespace Ui {
class MeshDeviceListFrame;
}

class MeshDeviceListFrame : public QFrame, public AbstractCallbackInvoker
{
    Q_OBJECT

public:
    explicit MeshDeviceListFrame(TKTMeshModel *tktMeshModel, const QString &meshUUID, AbstractCallbackAlerter *alerter, QWidget *parent = 0);
    ~MeshDeviceListFrame();

    void updateMeshNodes(QList<MeshDeviceNode*> *meshNodeList);

private slots:
    void on_buttonRefresh_clicked();

    void on_buttonGetAllNode_clicked();

    void on_buttonMeshClose_clicked();

    void doActionCopy();

    void on_tableMeshNode_customContextMenuRequested(const QPoint &pos);

private:
    void initMenuAndAction();

    Ui::MeshDeviceListFrame *ui;
    QString m_meshUUID;
    TKTMeshModel *m_tktMeshModel;
    MeshNodesModel *m_meshNodesModel;
    AbstractCallbackAlerter *m_alerter;
    QMenu *m_menu;
    QAction *m_actionCopy;

signals:
    void backToTKTMeshPage();
};

#endif // MESHDEVICELISTFRAME_H
