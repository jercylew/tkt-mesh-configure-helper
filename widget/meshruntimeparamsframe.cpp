#include "meshruntimeparamsframe.h"
#include "ui_meshruntimeparamsframe.h"
#include "model/meshmodel.h"
#include "utils/meshsavingbox.h"
#include <QTimer>

#include "model/tktmeshmodel.h"
#include "utils/tktmeshconnectionholder.h"
#include "utils/tktmeshapiworkercontroller.h"

#include "callback/getmeshscheduleparamscallback.h"
#include "callback/setmeshruntimeparamscallback.h"

MeshRuntimeParamsFrame::MeshRuntimeParamsFrame(TKTMeshModel *tktMeshModel, const QString &meshUUID, AbstractCallbackAlerter *alerter, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MeshRuntimeParamsFrame)
{
    ui->setupUi(this);
    this->m_tktMeshModel=tktMeshModel;
    this->m_meshUUID=meshUUID;
    this->m_alerter=alerter;

    ui->buttonMeshRuntimeParamsRefresh->setCursor(Qt::PointingHandCursor);
    ui->buttonMeshRuntimeParamsSave->setCursor(Qt::PointingHandCursor);

    ui->scrollAreaWidgetContents->setMinimumWidth(ui->frameContent->x()+ui->frameContent->width());
    ui->scrollAreaWidgetContents->setMinimumHeight(ui->frameContent->y()+ui->frameContent->height());
    ui->frameContent->setAutoFillBackground(false);
    ui->scrollAreaWidgetContents->setAutoFillBackground(false);
    this->setStyleSheet("#scrollAreaWidgetContents{background-color:white;}");

    QTimer::singleShot(100, ui->buttonMeshRuntimeParamsRefresh, SLOT(click()));
}

MeshRuntimeParamsFrame::~MeshRuntimeParamsFrame()
{
    delete ui;
}

void MeshRuntimeParamsFrame::loadMeshRuntimeParams(MeshRuntimeParams *params)
{
    ui->combCommandInterval->setCurrentText(QString::number(params->cmdIntervalMsec()));
    ui->spinTwoGroupsSchedulingTimeInterval->setValue(params->twoGroupsSchedulingTimeIntervalSec());
    ui->checkTurnOffAutoNotifyWhenSchedulingStart->setChecked(params->turnOffAutoNotificationWhenSchedulingIsStarted());
    ui->checkAutoRefreshMesh->setChecked(params->autoRefreshNodesDuringMeshInitialization());
    ui->spinBufferCommandThresholdInBusyScheduling->setValue(params->thresholdOfBufferedCommandNumberInBusyScheduling());
    ui->spinLuminaireMinBrightness->setValue(params->luminaireMinBrightness());
}

void MeshRuntimeParamsFrame::on_buttonMeshClose_clicked()
{
    emit backToTKTMeshPage();
}

void MeshRuntimeParamsFrame::on_buttonMeshRuntimeParamsRefresh_clicked()
{
    if(m_tktMeshModel==NULL)
    {
        return;
    }
    GetMeshScheduleParamsCallback *callback=new GetMeshScheduleParamsCallback(this, this->m_alerter, m_tktMeshModel->tktMesh());
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "get_mesh_schedule_params");
    asyncTask->requestJson()->insert("data", this->m_meshUUID);
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void MeshRuntimeParamsFrame::on_buttonMeshRuntimeParamsSave_clicked()
{
    SetMeshRuntimeParamsCallback *callback=new SetMeshRuntimeParamsCallback(this, this->m_alerter, m_tktMeshModel->tktMesh());
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "set_mesh_schedule_params");
    QJsonObject data;
    data.insert("mesh_uuid", m_meshUUID);
    data.insert("cmd_interval_msec", ui->combCommandInterval->currentText().toInt());
    data.insert("two_groups_schedule_interval_sec", ui->spinTwoGroupsSchedulingTimeInterval->value());
    data.insert("turn_off_auto_notification_when_schedule_started", ui->checkTurnOffAutoNotifyWhenSchedulingStart->isChecked());
    data.insert("auto_refresh_when_mesh_init", ui->checkAutoRefreshMesh->isChecked());
    data.insert("threshold_of_buffered_command_number_in_busy_schedule", ui->spinBufferCommandThresholdInBusyScheduling->value());
    data.insert("luminaire_min_brightness", ui->spinLuminaireMinBrightness->value());
    asyncTask->requestJson()->insert("data", data);
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}
