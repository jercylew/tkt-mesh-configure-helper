#include "meshlogframe.h"
#include "ui_meshlogframe.h"

#include "model/tktmeshmodel.h"
#include "callback/abstractcallbackalerter.h"
#include "callback/getmeshlogcallback.h"

#include "model/tktmeshmodel.h"
#include "utils/tktmeshconnectionholder.h"
#include "utils/tktmeshapiworkercontroller.h"

MeshLogFrame::MeshLogFrame(TKTMeshModel *tktMeshModel, const QString &meshUUID, AbstractCallbackAlerter *alerter, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MeshLogFrame)
{
    ui->setupUi(this);
    this->m_tktMeshModel=tktMeshModel;
    this->m_meshUUID=meshUUID;
    this->m_alerter=alerter;

    QTimer::singleShot(100, ui->buttonRefresh, SLOT(click()));
}

MeshLogFrame::~MeshLogFrame()
{
    delete ui;
}

void MeshLogFrame::loadMeshLog(QList<QString> *meshLogList)
{
    ui->textLog->clear();
    for(int i=0; i<meshLogList->size(); i++)
    {
        ui->textLog->append(meshLogList->at(i));
    }
}

void MeshLogFrame::on_buttonClear_clicked()
{
    ui->textLog->clear();
}

void MeshLogFrame::on_buttonRefresh_clicked()
{
    if(m_tktMeshModel==NULL)
    {
        return;
    }
    GetMeshLogCallback *callback=new GetMeshLogCallback(this, this->m_alerter, m_tktMeshModel->tktMesh());
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "get_mesh_log");
    asyncTask->requestJson()->insert("data", this->m_meshUUID);
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void MeshLogFrame::on_buttonMeshClose_clicked()
{
    emit backToTKTMeshPage();
}
