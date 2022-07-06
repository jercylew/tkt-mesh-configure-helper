#include "tktmeshdetailsframe.h"
#include "ui_tktmeshdetailsframe.h"
#include "model/tktmeshmodel.h"
#include "domain/tktmesh.h"
#include "globalsavingbox.h"

#include "domain/tktmesh.h"
#include "callback/gethostinfocallback.h"
#include "utils/asyncapirequesttask.h"
#include <QJsonObject>
#include "utils/tktmeshconnectionholder.h"
#include "utils/tktmeshapiworkercontroller.h"
#include "utils/asyncapirequesttask.h"
#include "callback/sethostinfocallback.h"
#include "domain/mesh.h"
#include "callback/getmeshbasicinfocallback.h"
#include "callback/setmeshbasicinfocallback.h"
#include "callback/refreshmeshnodescallback.h"
#include "callback/getmeshallnodescallback.h"
#include "callback/defaultcallback.h"
#include <QMessageBox>
#include "domain/bluetoothnode.h"
#include "callback/getallcomcallback.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include "model/nodegroupsmodel.h"
#include "callback/getmeshallgroupcallback.h"
#include "domain/nodegroup.h"
#include "dialogbuilder.h"
#include "model/meshesmodel.h"
#include "model/meshtableviewmodel.h"
#include "callback/getallmeshcallback.h"
#include "model/meshmodel.h"
#include "model/preparemeshmodel.h"
#include "widget/meshdetailsframe.h"
#include "model/meshmodel.h"
#include "utils/remoteconfigmanager.h"
#include "utils/remoteconfigsessionholder.h"
#include "callback/apitestrequestcallback.h"
#include "utils/tools.h"
#include "widget/meshdevicelistframe.h"
#include "widget/meshruntimeparamsframe.h"
#include "widget/meshlogframe.h"

TKTMeshDetailsFrame::TKTMeshDetailsFrame(TKTMeshModel *tktMeshModel, AbstractCallbackAlerter *alerter, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TKTMeshDetailsFrame)
{
    ui->setupUi(this);
    this->m_alerter=alerter;
    this->m_tktMeshModel=tktMeshModel;
    m_prepareMeshModel=NULL;
    m_meshDetailsFrame=NULL;
    m_meshModel=NULL;
    m_meshInfoWidget=NULL;

    qDebug() << "TKTMeshDetailsFrame::TKTMeshDetailsFrame"
             << "Scene:" << m_tktMeshModel->tktMesh()->sceneName();

    if(this->m_tktMeshModel->tktMesh()->inCloudConfigMode())
    {
        ui->labelTKTMeshTitle->setText(QString("【云配置模式】 主机ID: %1 场地：%2")
                                       .arg(m_tktMeshModel->tktMesh()->hostID())
                                       .arg(m_tktMeshModel->tktMesh()->sceneName()));
    }
    else
    {
        ui->labelTKTMeshTitle->setText(QString("【本地配置模式】 主机ID: %1, 主机通信IP: %2 场地：%3")
                                       .arg(m_tktMeshModel->tktMesh()->hostID())
                                       .arg(m_tktMeshModel->tktMesh()->hostPeerIP())
                                       .arg(m_tktMeshModel->tktMesh()->sceneName()));
    }

    ui->labelConnectionStatus->setText("已连接");
    ui->labelConnectionStatus->setStyleSheet("color:#0000FF;");

    m_meshesModel=new MeshesModel(this);
    MeshTableViewModel *meshTableViewModel=new MeshTableViewModel(m_meshesModel, this);
    ui->tableMesh->setModel(meshTableViewModel);
    ui->tableMesh->setEditTriggers(QTableView::NoEditTriggers);
    ui->tableMesh->verticalHeader()->setVisible( true );
    ui->tableMesh->horizontalHeader()->setStretchLastSection( true );
    ui->tableMesh->setSelectionMode(QTableView::ExtendedSelection);
    ui->tableMesh->setSelectionBehavior(QTableView::SelectRows);

    ui->tableMesh->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableMesh->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableMesh->setColumnWidth(0, 200);

    ui->tabTKTMeshDetails->setCurrentWidget(ui->tabHostInfo);

    connect(ui->combComName, SIGNAL(aboutToShowPopup()), this, SLOT(doComComboxAboutShowPopup()));
    ui->buttonTKTMeshClose->setCursor(Qt::PointingHandCursor);
    ui->buttonTKTMeshClose->setToolTip("返回TKTMesh列表");

    connect(m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController(), SIGNAL(socketDisconnected()),
            this, SLOT(doSocketDisconnected()));

    ui->stackedWidget->setCurrentWidget(ui->pageTKTMesh);
    QTimer::singleShot(100, ui->buttonRefreshHostInfo, SLOT(click()));

    connect(m_meshDetailsFrame, SIGNAL(backToTKTMeshPage()),
            this, SLOT(doBackToTKTMeshPage()));

    //Load mesh info by default
    on_buttonRefreshMeshList_clicked();
}

void TKTMeshDetailsFrame::reloadHostInfo()
{
    ui->lineHostID->setText(m_tktMeshModel->tktMesh()->hostID());
    ui->lineHostCode->setText(m_tktMeshModel->tktMesh()->hostCode());
    ui->lineHostDescription->setText(m_tktMeshModel->tktMesh()->hostDescription());
    ui->lineHostName->setText(m_tktMeshModel->tktMesh()->hostName());
    ui->lineHostKey->setText(m_tktMeshModel->tktMesh()->hostKey());
    ui->lineHostHardVer->setText(m_tktMeshModel->tktMesh()->hostHardVersion());
    ui->lineHostSoftVer->setText(m_tktMeshModel->tktMesh()->hostSoftVersion());
    if(m_tktMeshModel->tktMesh()->hostRegistered())
    {
        ui->labelHostRegisterState->setText("已注册");
        ui->labelHostRegisterState->setStyleSheet("color:#0000FF;");
        ui->checkClearRegisterState->setCheckable(true);
    }
    else
    {
        ui->labelHostRegisterState->setText("未注册");
        ui->labelHostRegisterState->setStyleSheet("color:#FF0000;");
        ui->checkClearRegisterState->setChecked(false);
    }
    ui->linePlatformAddress->setText(m_tktMeshModel->tktMesh()->platformAddress());
    ui->linePlatformPort->setText(QString::number(m_tktMeshModel->tktMesh()->platformPort()));
    ui->lineCloudConfigAddress->setText(m_tktMeshModel->tktMesh()->cloudConfigAddress());
    ui->lineCloudConfigPort->setText(QString::number(m_tktMeshModel->tktMesh()->cloudConfigPort()));

    if(this->m_tktMeshModel->tktMesh()->inCloudConfigMode())
    {
        ui->labelTKTMeshTitle->setText(QString("【云配置模式】 主机ID: %1 场地：%2")
                                       .arg(m_tktMeshModel->tktMesh()->hostID())
                                       .arg(m_tktMeshModel->tktMesh()->sceneName()));
    }
    else
    {
        ui->labelTKTMeshTitle->setText(QString("【本地配置模式】 主机ID: %1, 主机通信IP: %2 场地：%3")
                                       .arg(m_tktMeshModel->tktMesh()->hostID())
                                       .arg(m_tktMeshModel->tktMesh()->hostPeerIP())
                                       .arg(m_tktMeshModel->tktMesh()->sceneName()));
    }
}

void TKTMeshDetailsFrame::loadMesh(Mesh *mesh)
{
    ui->labelCurrentMeshName->setText(mesh->meshName());
    ui->labelMeshUUID->setText(mesh->meshUUID());
    ui->combComName->setCurrentText(mesh->comName());
    ui->radioAutoScheduleOn->setChecked(mesh->autoScheduleIsOn());
    ui->radioAutoScheduleOff->setChecked(!mesh->autoScheduleIsOn());
    ui->labelComState->setText(mesh->comIsOpened()?tr("已打开"):tr("未打开"));
    ui->labelReadDataFrames->setText(QString::number(mesh->readFrames()));
    ui->radioComEnable->setChecked(mesh->comIsEnabled());
    ui->radioComDisable->setChecked(!mesh->comIsEnabled());
    ui->radioBindComNameToMeshNameYes->setChecked(mesh->bindComNameToMeshName());
    ui->radioBindComNameToMeshNameNo->setChecked(!mesh->bindComNameToMeshName());
}

void TKTMeshDetailsFrame::loadComList(const QList<QString> &comList)
{
    ui->combComName->clear();
    for(int i=0; i<comList.size(); i++)
    {
        ui->combComName->addItem(comList.at(i));
    }
}

void TKTMeshDetailsFrame::loadMeshList(const QList<Mesh *> &meshList)
{
    m_meshesModel->setMeshList(meshList);
}

void TKTMeshDetailsFrame::on_buttonTKTMeshClose_clicked()
{
    emit backToTKTMeshList();
}

void TKTMeshDetailsFrame::on_buttonRefreshHostInfo_clicked()
{
    if(m_tktMeshModel==NULL)
    {
        return;
    }
    GetHostInfoCallback *callback=new GetHostInfoCallback(this, this->m_alerter, m_tktMeshModel->tktMesh());
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "get_host_info");
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void TKTMeshDetailsFrame::on_buttonSaveHostInfo_clicked()
{
    if(m_tktMeshModel==NULL)
    {
        return;
    }
    if(QMessageBox::question(this, "保存TKTMesh主机信息", "确定保存主机信息吗?")!=QMessageBox::Yes)
    {
        return;
    }
    SetHostInfoCallback *callback=new SetHostInfoCallback(this, this->m_alerter, m_tktMeshModel->tktMesh());
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "set_host_info");
    QJsonObject dataJson;
    QJsonObject hostDataJson;
    hostDataJson.insert("host_code", ui->lineHostCode->text().trimmed());
    hostDataJson.insert("host_name", ui->lineHostName->text());
    hostDataJson.insert("host_description", ui->lineHostDescription->text());
    hostDataJson.insert("host_registered", !ui->checkClearRegisterState->isChecked());
    hostDataJson.insert("platform_address", ui->linePlatformAddress->text().trimmed());
    hostDataJson.insert("platform_port", ui->linePlatformPort->text().trimmed().toInt());
    hostDataJson.insert("cloud_config_address", ui->lineCloudConfigAddress->text().trimmed());
    hostDataJson.insert("cloud_config_port", ui->lineCloudConfigPort->text().trimmed().toInt());
    dataJson.insert("host_data", hostDataJson);
    asyncTask->requestJson()->insert("data", dataJson);
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void TKTMeshDetailsFrame::on_buttonRefreshTKTMeshInfo_clicked()
{

}

void TKTMeshDetailsFrame::on_buttonSaveTKTMeshInfo_clicked()
{
    if(ui->labelMeshUUID->text().size()<10)
    {
        QMessageBox::about(this, "消息", "请在左侧列表中选中一个Mesh!");
        return;
    }
    Mesh *mesh=m_meshesModel->getByUUID(ui->labelMeshUUID->text());
    if(mesh==NULL)
    {
        QMessageBox::about(this, "消息", QString("未找到UUID为'%1'的Mesh!").arg(ui->labelMeshUUID->text()));
        return;
    }
    if(QMessageBox::question(this, "保存TKTMesh信息", "确定保存TKTMesh信息吗?")!=QMessageBox::Yes)
    {
        return;
    }
    SetMeshBasicInfoCallback *callback=new SetMeshBasicInfoCallback(this, this->m_alerter, m_tktMeshModel->tktMesh());
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "set_mesh_info");
    QJsonObject data;
    data.insert("mesh_uuid", mesh->meshUUID());
    data.insert("com_name", ui->combComName->currentText());
    data.insert("auto_schedule_is_on", ui->radioAutoScheduleOn->isChecked());
    data.insert("com_is_enabled", ui->radioComEnable->isChecked());
    data.insert("bind_com_name_to_mesh_name", ui->radioBindComNameToMeshNameYes->isChecked());
    asyncTask->requestJson()->insert("data", data);
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void TKTMeshDetailsFrame::on_buttonRefreshNodes_clicked()
{
    if(ui->labelMeshUUID->text().size()<10)
    {
        QMessageBox::about(this, "消息", "请在左侧列表中选中一个Mesh!");
        return;
    }
    Mesh *mesh=m_meshesModel->getByUUID(ui->labelMeshUUID->text());
    if(mesh==NULL)
    {
        QMessageBox::about(this, "消息", QString("未找到UUID为'%1'的Mesh!").arg(ui->labelMeshUUID->text()));
        return;
    }
    RefreshMeshNodesCallback *callback=new RefreshMeshNodesCallback(this, this->m_alerter, m_tktMeshModel);
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "refresh_mesh_nodes");
    asyncTask->requestJson()->insert("data", mesh->meshUUID());
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void TKTMeshDetailsFrame::on_buttonGetAllNodes_clicked()
{
    if(m_tktMeshModel==NULL)
    {
        return;
    }
    GetMeshAllNodesCallback *callback=new GetMeshAllNodesCallback(this, this->m_alerter, m_tktMeshModel);
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "get_mesh_all_nodes");
    asyncTask->requestJson()->insert("data", "00000000000000000000000000000000");
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void TKTMeshDetailsFrame::on_buttonRestartTKTMesh_clicked()
{
    if(m_tktMeshModel==NULL)
    {
        return;
    }
    if(QMessageBox::question(this, "重启TKTMesh", "确定重启TKTMesh吗?")!=QMessageBox::Yes)
    {
        return;
    }
    DefaultCallback *callback=new DefaultCallback(this, this->m_alerter, m_tktMeshModel->tktMesh(), tr("重启TKTMesh成功!"), tr("重启TKTMesh失败!"));
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "restart_app");
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void TKTMeshDetailsFrame::on_buttonRebootHost_clicked()
{
    if(m_tktMeshModel==NULL)
    {
        return;
    }
    if(QMessageBox::question(this, "重启TKTMesh主机", "确定重启TKTMesh主机吗?")!=QMessageBox::Yes)
    {
        return;
    }
    DefaultCallback *callback=new DefaultCallback(this, this->m_alerter, m_tktMeshModel->tktMesh(), tr("重启TKTMesh主机成功!"), tr("重启TKTMesh主机失败!"));
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "reboot_host");
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void TKTMeshDetailsFrame::doSocketDisconnected()
{
    ui->labelConnectionStatus->setText("已断开");
    ui->labelConnectionStatus->setStyleSheet("color:#FF0000;");
    QMessageBox::about(NULL, tr("连接断开"), tr("与主机'%1'的连接已经断开!").arg(m_tktMeshModel->tktMesh()->hostPeerIP()));
}

void TKTMeshDetailsFrame::doComComboxAboutShowPopup()
{
    if(m_tktMeshModel==NULL)
    {
        return;
    }
    GetAllComCallback *callback=new GetAllComCallback(this, this->m_alerter, m_tktMeshModel->tktMesh());
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "get_all_com");
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

TKTMeshDetailsFrame::~TKTMeshDetailsFrame()
{
    delete ui;
}


void TKTMeshDetailsFrame::on_buttonRefreshMeshList_clicked()
{
    if(m_tktMeshModel==NULL)
    {
        return;
    }
    GetAllMeshCallback *callback=new GetAllMeshCallback(this, this->m_alerter, m_tktMeshModel->tktMesh());
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "get_all_mesh");
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void TKTMeshDetailsFrame::on_tableMesh_clicked(const QModelIndex &index)
{
    Mesh *mesh=m_meshesModel->getByUUID(index.data(Qt::UserRole).toString());
    if(mesh==NULL)
    {
        ui->frameMeshBasicInfo->setVisible(false);
        return;
    }
    ui->frameMeshBasicInfo->setVisible(true);

    GetMeshBasicInfoCallback *callback=new GetMeshBasicInfoCallback(this, this->m_alerter, m_tktMeshModel->tktMesh(), false);
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "get_mesh_by_id");
    asyncTask->requestJson()->insert("data", mesh->meshUUID());
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void TKTMeshDetailsFrame::on_buttonRefreshMeshBasicInfo_clicked()
{
    if(m_tktMeshModel==NULL)
    {
        return;
    }
    if(ui->labelMeshUUID->text().size()<10)
    {
        QMessageBox::about(this, "消息", "请在左侧列表中选中一个Mesh!");
        return;
    }
    GetMeshBasicInfoCallback *callback=new GetMeshBasicInfoCallback(this, this->m_alerter, m_tktMeshModel->tktMesh());
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "get_mesh_by_id");
    asyncTask->requestJson()->insert("data", ui->labelMeshUUID->text());
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void TKTMeshDetailsFrame::on_buttonRemoteConfig_clicked()
{
    if(ui->labelMeshUUID->text().size()<10)
    {
        QMessageBox::about(this, "消息", "请在左侧列表中选中一个Mesh!");
        return;
    }
    Mesh *mesh=m_meshesModel->getByUUID(ui->labelMeshUUID->text());
    if(mesh==NULL)
    {
        QMessageBox::about(this, "消息", QString("未找到UUID为'%1'的Mesh!").arg(ui->labelMeshUUID->text()));
        return;
    }
    if(QMessageBox::question(this, "远程配置Mesh", "确认进入远程配置？")!=QMessageBox::Yes)
    {
        return;
    }
    m_prepareMeshModel=new PrepareMeshModel(mesh, m_tktMeshModel, this);
    QDialog *dialog=DialogBuilder::getInstance()->buildRemoteConfigPrepareDialog(m_prepareMeshModel);
    if(dialog->exec()!=QDialog::Accepted)
    {
        m_prepareMeshModel->deleteLater();
        m_prepareMeshModel=NULL;
        return;
    }

    m_meshModel=new MeshModel(m_prepareMeshModel, this);
    m_meshModel->init();
    m_prepareMeshModel->remoteConfigManager()->remoteConfigSessionHolder()->startKeepCommandReportTimer();

    m_meshDetailsFrame=new MeshDetailsFrame(m_meshModel);
    connect(m_meshDetailsFrame, SIGNAL(backToTKTMeshPage()),
            this, SLOT(doBackToTKTMeshPage()));
    if(ui->pageMesh->layout()==NULL)
    {
        ui->pageMesh->setLayout(new QVBoxLayout());
    }
    ui->pageMesh->layout()->addWidget(m_meshDetailsFrame);
    ui->stackedWidget->setCurrentWidget(ui->pageMesh);
    m_meshDetailsFrame->setVisible(true);
}

void TKTMeshDetailsFrame::doBackToTKTMeshPage()
{
    if(m_meshModel!=NULL)
    {
        m_prepareMeshModel->remoteConfigManager()->remoteConfigSessionHolder()->stopKeepCommandReportTimer();
        ui->stackedWidget->setCurrentWidget(ui->pageTKTMesh);
        m_meshDetailsFrame->setVisible(false);
        m_meshDetailsFrame->setParent(NULL);
        m_meshDetailsFrame->deleteLater();
        m_prepareMeshModel->deleteLater();
        m_prepareMeshModel=NULL;
        m_meshModel->deleteLater();
        m_prepareMeshModel=NULL;
        m_meshDetailsFrame=NULL;
        m_meshModel=NULL;
    }
    if(m_meshInfoWidget!=NULL)
    {
        ui->stackedWidget->setCurrentWidget(ui->pageTKTMesh);
        m_meshInfoWidget->setVisible(false);
        m_meshInfoWidget->setParent(NULL);
        m_meshInfoWidget->deleteLater();
        m_meshInfoWidget=NULL;
    }
}

void TKTMeshDetailsFrame::on_buttonSendRequest_clicked()
{
    if(m_tktMeshModel==NULL)
    {
        return;
    }
    QJsonObject json=Tools::getJsonObjectFromDataArray(ui->textRequest->toPlainText().toUtf8());
    if(json.isEmpty())
    {
        QMessageBox::about(this, "报告", "请求的JSON格式错误!");
        return;
    }
    ApiTestRequestCallback *callback=new ApiTestRequestCallback(this, this->m_alerter);
    connect(callback, SIGNAL(getResponse(QByteArray)),
            this, SLOT(doReadTestRequestResponse(QByteArray)));
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);

    QStringList lstKeys = json.keys();
    for (int i = 0;i < lstKeys.size();i++)
    {
        QString strKey = lstKeys.at(i);
        asyncTask->requestJson()->insert(strKey, json.value(strKey));
    }

    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void TKTMeshDetailsFrame::on_buttonClearResponse_clicked()
{
    ui->textResponse->clear();
}

void TKTMeshDetailsFrame::on_buttonClearRequest_clicked()
{
    ui->textRequest->clear();
}

void TKTMeshDetailsFrame::doReadTestRequestResponse(const QByteArray &response)
{
    ui->textResponse->setText(QString::fromUtf8(response));
}

void TKTMeshDetailsFrame::on_buttonDeviceList_clicked()
{
    if(ui->labelMeshUUID->text().size()<10)
    {
        QMessageBox::about(this, "消息", "请在左侧列表中选中一个Mesh!");
        return;
    }
    Mesh *mesh=m_meshesModel->getByUUID(ui->labelMeshUUID->text());
    if(mesh==NULL)
    {
        QMessageBox::about(this, "消息", QString("未找到UUID为'%1'的Mesh!").arg(ui->labelMeshUUID->text()));
        return;
    }
    m_meshInfoWidget=new MeshDeviceListFrame(m_tktMeshModel, mesh->meshUUID(), this->m_alerter);
    connect(m_meshInfoWidget, SIGNAL(backToTKTMeshPage()),
            this, SLOT(doBackToTKTMeshPage()));
    if(ui->pageMesh->layout()==NULL)
    {
        ui->pageMesh->setLayout(new QVBoxLayout());
    }
    ui->pageMesh->layout()->addWidget(m_meshInfoWidget);
    ui->stackedWidget->setCurrentWidget(ui->pageMesh);
    m_meshInfoWidget->setVisible(true);
}

void TKTMeshDetailsFrame::on_buttonAddMesh_clicked()
{
    QString meshName;
    QDialog *dialog=DialogBuilder::getInstance()->buildInputDialog(tr("Add Mesh"), "请输入Mesh名称：", &meshName);
    if(dialog->exec()!=QDialog::Accepted)
    {
        return;
    }
    if(meshName.isEmpty())
    {
        dialog = DialogBuilder::getInstance()->buildAboutDialog(tr("消息"), tr("Mesh名不能为空！"));
        dialog->exec();
        return;
    }
    DefaultCallback *callback=new DefaultCallback(NULL, this->m_alerter, this->m_tktMeshModel->tktMesh(), "操作成功!", "操作失败！");
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "add_mesh");
    asyncTask->requestJson()->insert("data", meshName);
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void TKTMeshDetailsFrame::on_buttonRemoveMesh_clicked()
{
    if(ui->labelMeshUUID->text().size()<10)
    {
        QMessageBox::about(this, "消息", "请在左侧列表中选中一个Mesh!");
        return;
    }
    Mesh *mesh=m_meshesModel->getByUUID(ui->labelMeshUUID->text());
    if(mesh==NULL)
    {
        QMessageBox::about(this, "消息", QString("未找到UUID为'%1'的Mesh!").arg(ui->labelMeshUUID->text()));
        return;
    }
    QDialog *dialog=DialogBuilder::getInstance()->buildQuestionDialog(tr("Remove Mesh"), tr("Are you sure to remove '%1'?").arg(mesh->meshName()));
    if(dialog->exec()!=QDialog::Accepted)
    {
        return;
    }
    DefaultCallback *callback=new DefaultCallback(NULL, this->m_alerter, this->m_tktMeshModel->tktMesh(), "操作成功!", "操作失败！");
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "remove_mesh");
    asyncTask->requestJson()->insert("data", mesh->meshUUID());
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void TKTMeshDetailsFrame::on_buttonRenameMesh_clicked()
{
    if(ui->labelMeshUUID->text().size()<10)
    {
        QMessageBox::about(this, "消息", "请在左侧列表中选中一个Mesh!");
        return;
    }
    Mesh *mesh=m_meshesModel->getByUUID(ui->labelMeshUUID->text());
    if(mesh==NULL)
    {
        QMessageBox::about(this, "消息", QString("未找到UUID为'%1'的Mesh!").arg(ui->labelMeshUUID->text()));
        return;
    }
    QString meshName=mesh->meshName();
    QDialog *dialog=DialogBuilder::getInstance()->buildInputDialog(tr("Rename Mesh"), "请输入新的Mesh名称：", &meshName);
    if(dialog->exec()!=QDialog::Accepted)
    {
        return;
    }
    if(meshName.isEmpty())
    {
        dialog = DialogBuilder::getInstance()->buildAboutDialog(tr("消息"), tr("Mesh名不能为空!"));
        dialog->exec();
        return;
    }
    DefaultCallback *callback=new DefaultCallback(NULL, this->m_alerter, this->m_tktMeshModel->tktMesh(), "操作成功!", "操作失败！");
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "rename_mesh");
    QJsonObject dataJson;
    dataJson.insert("mesh_uuid", mesh->meshUUID());
    dataJson.insert("mesh_name", meshName);
    asyncTask->requestJson()->insert("data", dataJson);
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void TKTMeshDetailsFrame::on_buttonRuntimeParams_clicked()
{
    if(ui->labelMeshUUID->text().size()<10)
    {
        QMessageBox::about(this, "消息", "请在左侧列表中选中一个Mesh!");
        return;
    }
    Mesh *mesh=m_meshesModel->getByUUID(ui->labelMeshUUID->text());
    if(mesh==NULL)
    {
        QMessageBox::about(this, "消息", QString("未找到UUID为'%1'的Mesh!").arg(ui->labelMeshUUID->text()));
        return;
    }
    m_meshInfoWidget=new MeshRuntimeParamsFrame(m_tktMeshModel, mesh->meshUUID(), this->m_alerter);
    connect(m_meshInfoWidget, SIGNAL(backToTKTMeshPage()),
            this, SLOT(doBackToTKTMeshPage()));
    if(ui->pageMesh->layout()==NULL)
    {
        ui->pageMesh->setLayout(new QVBoxLayout());
    }
    ui->pageMesh->layout()->addWidget(m_meshInfoWidget);
    ui->stackedWidget->setCurrentWidget(ui->pageMesh);
    m_meshInfoWidget->setVisible(true);
}

void TKTMeshDetailsFrame::on_buttonMeshLog_clicked()
{
    if(ui->labelMeshUUID->text().size()<10)
    {
        QMessageBox::about(this, "消息", "请在左侧列表中选中一个Mesh!");
        return;
    }
    Mesh *mesh=m_meshesModel->getByUUID(ui->labelMeshUUID->text());
    if(mesh==NULL)
    {
        QMessageBox::about(this, "消息", QString("未找到UUID为'%1'的Mesh!").arg(ui->labelMeshUUID->text()));
        return;
    }
    m_meshInfoWidget=new MeshLogFrame(m_tktMeshModel, mesh->meshUUID(), this->m_alerter);
    connect(m_meshInfoWidget, SIGNAL(backToTKTMeshPage()),
            this, SLOT(doBackToTKTMeshPage()));
    if(ui->pageMesh->layout()==NULL)
    {
        ui->pageMesh->setLayout(new QVBoxLayout());
    }
    ui->pageMesh->layout()->addWidget(m_meshInfoWidget);
    ui->stackedWidget->setCurrentWidget(ui->pageMesh);
    m_meshInfoWidget->setVisible(true);
}

void TKTMeshDetailsFrame::on_buttonGetPortNameToMeshNameMapping_clicked()
{
    QJsonObject request;
    request.insert("topic", "request");
    request.insert("command", "get_port_name_to_mesh_name_mapping");
    QByteArray data=Tools::getDataArrayOfJsonObject(&request);
    ui->textRequest->setText(QString::fromLatin1(data));
    ui->textResponse->clear();
    ui->buttonSendRequest->click();
}

void TKTMeshDetailsFrame::on_buttonSetMeshInfo_clicked()
{
    SwitchMeshInfo stMeshInfo;
    Mesh *mesh = m_meshesModel->getByUUID(GlobalSavingBox::getInstance()->defaultMeshUUID());
    if (mesh != nullptr)
    {
        stMeshInfo.m_strMeshLTK = mesh->gatewayMeshLtk();
        stMeshInfo.m_strMeshName = mesh->gatewayMeshName();
        stMeshInfo.m_strMeshPassword = mesh->gatewayMeshPassword();
    }

    QDialog *dialog = DialogBuilder::getInstance()->buildSetMeshInfoDialog(&stMeshInfo,
                      tr("MeshDetailsFrame.SetSwicthMeshInfo"));

    if (dialog->exec() != QDialog::Accepted)
    {
        return;
    }

    QString strSwitchMeshName = stMeshInfo.m_strMeshName;
    QString strSwitchMeshPassword = stMeshInfo.m_strMeshPassword;

    if (strSwitchMeshName.isEmpty() ||
            strSwitchMeshName.isNull() ||
            strSwitchMeshPassword.isEmpty() ||
            strSwitchMeshPassword.isNull())
    {
        return;
    }

    QJsonObject request;
    request.insert("topic", "request");
    request.insert("command", "set_switch_mesh_info");
    request.insert("mesh_name", strSwitchMeshName);
    request.insert("mesh_password", strSwitchMeshPassword);

    QByteArray data = Tools::getDataArrayOfJsonObject(&request);
    ui->textRequest->setText(QString::fromLatin1(data));
    ui->textResponse->clear();
    ui->buttonSendRequest->click();
}

void TKTMeshDetailsFrame::on_buttonSetGatewayId_clicked()
{
    quint8 nGatewayId = 0;
    Mesh *mesh = m_meshesModel->getByUUID(GlobalSavingBox::getInstance()->defaultMeshUUID());
    if (mesh != nullptr)
    {
        nGatewayId = mesh->gatewayMeshAddress();
    }
    QDialog *dialog = DialogBuilder::getInstance()->buildSetGatewayIdDialog(&nGatewayId,
                      tr("MeshDetailsFrame.SetGatewayId"));

    if (dialog->exec() != QDialog::Accepted)
    {
        return;
    }


    QJsonObject request;
    request.insert("topic", "request");
    request.insert("command", "set_gateway_id");
    request.insert("gateway_id", nGatewayId);

    QByteArray data = Tools::getDataArrayOfJsonObject(&request);
    ui->textRequest->setText(QString::fromLatin1(data));
    ui->textResponse->clear();
    ui->buttonSendRequest->click();
}
