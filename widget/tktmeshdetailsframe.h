#ifndef TKTMESHDETAILSFRAME_H
#define TKTMESHDETAILSFRAME_H

#include <QFrame>
#include "callback/abstractcallbackinvoker.h"
#include "callback/abstractcallbackalerter.h"

class TKTMeshModel;
class MeshNodesModel;
class NodeGroupsModel;
class MeshesModel;
class PrepareMeshModel;
class MeshDetailsFrame;
class MeshModel;

class MeshDeviceListFrame;

namespace Ui {
class TKTMeshDetailsFrame;
}

class TKTMeshDetailsFrame : public QFrame, public AbstractCallbackInvoker
{
    Q_OBJECT

public:
    explicit TKTMeshDetailsFrame(TKTMeshModel *tktMeshModel, AbstractCallbackAlerter *alerter, QWidget *parent = 0);
    ~TKTMeshDetailsFrame();

    void reloadHostInfo();

    void loadMesh(Mesh *mesh);

    void loadComList(const QList<QString> &comList);

    void loadMeshList(const QList<Mesh *> &meshList);

private slots:
    void on_buttonTKTMeshClose_clicked();

    void on_buttonRefreshHostInfo_clicked();

    void on_buttonSaveHostInfo_clicked();

    void on_buttonRefreshTKTMeshInfo_clicked();

    void on_buttonSaveTKTMeshInfo_clicked();

    void on_buttonRefreshNodes_clicked();

    void on_buttonGetAllNodes_clicked();

    void on_buttonRestartTKTMesh_clicked();

    void on_buttonRebootHost_clicked();

    void doSocketDisconnected();

    void doComComboxAboutShowPopup();

    void on_buttonRefreshMeshList_clicked();

    void on_tableMesh_clicked(const QModelIndex &index);

    void on_buttonRefreshMeshBasicInfo_clicked();

    void on_buttonRemoteConfig_clicked();

    void doBackToTKTMeshPage();

    void on_buttonSendRequest_clicked();

    void on_buttonClearResponse_clicked();

    void on_buttonClearRequest_clicked();

    void doReadTestRequestResponse(const QByteArray &response);

    void on_buttonDeviceList_clicked();

    void on_buttonAddMesh_clicked();

    void on_buttonRemoveMesh_clicked();

    void on_buttonRenameMesh_clicked();

    void on_buttonRuntimeParams_clicked();

    void on_buttonMeshLog_clicked();

    void on_buttonGetPortNameToMeshNameMapping_clicked();

    void on_buttonSetMeshInfo_clicked();

private:
    Ui::TKTMeshDetailsFrame *ui;
    TKTMeshModel *m_tktMeshModel;
    MeshesModel *m_meshesModel;
    NodeGroupsModel *m_nodeGroupsModel;

    AbstractCallbackAlerter *m_alerter;

    PrepareMeshModel *m_prepareMeshModel;
    MeshModel *m_meshModel;
    MeshDetailsFrame *m_meshDetailsFrame;

    QWidget *m_meshInfoWidget;

signals:
    void backToTKTMeshList();
};

#endif // TKTMESHDETAILSFRAME_H
