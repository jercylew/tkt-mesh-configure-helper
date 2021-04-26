#include "controlitemeditdialogframeofrelaygroup.h"
#include "ui_controlitemeditdialogframeofrelaygroup.h"
#include "model/meshmodel.h"
#include "db/meshdbmanager.h"

#define ITEM_POWER_DATA             "power"
#define ITEM_LUX_SENSOR_DATA        "lux_sensor"
#define ITEM_MOTION_SENSOR_DATA     "motion_sensor"
#define ITEM_GAS_TRANSDUCER_DATA    "gas_transducer"
#define ITEM_NO_OPERATION_DATA      "no_operation"

ControlItemEditDialogFrameOfRelayGroup::ControlItemEditDialogFrameOfRelayGroup(MeshModel *meshModel, TimeLineControlItem *controlItem, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ControlItemEditDialogFrameOfRelayGroup),
    m_meshModel(meshModel)
{
    ui->setupUi(this);

    ui->comboBox->addItem(tr("Power"), ITEM_POWER_DATA);
    ui->comboBox->addItem(tr("Lux Sensor"), ITEM_LUX_SENSOR_DATA);
    ui->comboBox->addItem(tr("Motion Sensor"), ITEM_MOTION_SENSOR_DATA);
    ui->comboBox->addItem(tr("Gas Transducer"), ITEM_GAS_TRANSDUCER_DATA);
    ui->comboBox->addItem(tr("No Operation"), ITEM_NO_OPERATION_DATA);

    ui->buttonDone->setCursor(Qt::PointingHandCursor);
    ui->buttonCancel->setCursor(Qt::PointingHandCursor);

    ui->lineMotionTypeBindSensorId->setMeshModel(meshModel);
    ui->lineMotionTypeBindSensorId->addAccessSensorType(Sensor::VirtualMotionSensor);
    ui->lineMotionTypeBindSensorId->addAccessSensorType(Sensor::RealMotionSensor);

    ui->lineLuxTypeBindSensorId->setMeshModel(meshModel);
    ui->lineLuxTypeBindSensorId->addAccessSensorType(Sensor::LuxSensor);

    ui->cmbGTBindSensorId->setMeshModel(meshModel);
    ui->cmbGTBindSensorId->addAccessSensorType(Sensor::CO1Sensor);
    ui->cmbGTBindSensorId->addAccessSensorType(Sensor::CO2Sensor);
    ui->cmbGTBindSensorId->addAccessSensorType(Sensor::FormaldehydeSensor);
    ui->cmbGTBindSensorId->addAccessSensorType(Sensor::PMSensor);

    setControlItem(controlItem);
}

ControlItemEditDialogFrameOfRelayGroup::~ControlItemEditDialogFrameOfRelayGroup()
{
    delete ui;
}

void ControlItemEditDialogFrameOfRelayGroup::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void ControlItemEditDialogFrameOfRelayGroup::setControlItem(TimeLineControlItem *controlItem)
{
    this->controlItem=controlItem;
    setType(controlItem->controlType);
    ui->radioRelayOn->setChecked(controlItem->powerTypePower!=0);
    ui->radioRelayOff->setChecked(controlItem->powerTypePower==0);
    ui->radioBaseStateOn->setChecked(controlItem->motionTypeBasePower!=0);
    ui->radioBaseStateOff->setChecked(controlItem->motionTypeBasePower==0);
    if(controlItem->motionTrigPowerByLuxSensor)
    {
        ui->checkBindLuxSensorResultAsTrigPower->setChecked(true);
        ui->radioTriggeredStateOn->setEnabled(false);
        ui->radioTriggeredStateOff->setEnabled(false);
    }
    else
    {
        ui->checkBindLuxSensorResultAsTrigPower->setChecked(false);
        ui->radioTriggeredStateOn->setEnabled(true);
        ui->radioTriggeredStateOff->setEnabled(true);
    }
    ui->radioTriggeredStateOn->setChecked(controlItem->motionTypeTriggerPower!=0);
    ui->radioTriggeredStateOff->setChecked(controlItem->motionTypeTriggerPower==0);
    ui->spinMotionTypeTrigTime->setValue((quint16)controlItem->motionTypeTriggerTime);
    ui->lineMotionTypeBindSensorId->setText(controlItem->motionTypeBindSensorId);
    ui->lineLuxTypeBindSensorId->setText(controlItem->luxTypeBindSensorId);
    ui->spinLuxTypeNotFeedBackLuxOf0->setValue((quint16)controlItem->luxTypeNotFeedBackLuxOf0);
    ui->spinLuxTypeNotFeedBackLuxOf100->setValue((quint16)controlItem->luxTypeNotFeedBackLuxOf100);
    ui->spinDimmingIntervalSec->setValue(controlItem->dimmingIntervalSec);
    ui->spinMotionTriggerShieldMSec->setValue(controlItem->motionTypeTriggerShieldMSec);

    ui->cmbGTBindSensorId->setText(controlItem->gasTrnsdcrTypeBindSensorId);
    ui->radGTTrigTypeOn->setChecked(controlItem->gasTrnsdcrTypeTrigTypeOn);
    ui->radGTTrigTypeTimer->setChecked(controlItem->gasTrnsdcrTypeTrigTypeTimer);
    ui->radGTTrigModeUpperBottom->setChecked(controlItem->gasTrnsdcrTypeTrigModeUpperBottom);
    ui->radGTTrigModeBottomUpper->setChecked(controlItem->gasTrnsdcrTypeTrigModeBottomUpper);
    ui->spinGTThresholdMax->setValue(controlItem->gasTrnsdcrTypeMaxThreshold);
    ui->spinGTThresholdMin->setValue(controlItem->gasTrnsdcrTypeMinThreshold);
    ui->spinGTTrigHoldTime->setValue(controlItem->gasTrnsdcrTypeTrigHoldTime);
    ui->spinGTTriggerShieldMSec->setValue(controlItem->gasTrnsdcrTypeTrigShieldMSec);
    ui->radGTBaseStatusOn->setChecked(controlItem->gasTrnsdcrTypeTrigBaseStatusOn);
    ui->radGTBaseStatusOff->setChecked(controlItem->gasTrnsdcrTypeTrigBaseStatusOff);
}

void ControlItemEditDialogFrameOfRelayGroup::setType(const QString &type)
{
    ui->comboBox->setCurrentIndex(ui->comboBox->findData(type));
    if(type==ITEM_POWER_DATA)
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pageOnOff));
    }
    else if(type==ITEM_MOTION_SENSOR_DATA)
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pageMotionSensor));
    }
    else if(type==ITEM_LUX_SENSOR_DATA)
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pageLuxSensor));
    }
    else if(type==ITEM_GAS_TRANSDUCER_DATA)
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pageGasTransducer));
    }
    else
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pageOthers));
    }
}

void ControlItemEditDialogFrameOfRelayGroup::on_comboBox_activated(int)
{
    QString type=ui->comboBox->currentData().toString();
    if(type==ITEM_POWER_DATA)
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pageOnOff));
    }
    else if(type==ITEM_LUX_SENSOR_DATA)
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pageLuxSensor));
    }
    else if(type==ITEM_MOTION_SENSOR_DATA)
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pageMotionSensor));
    }
    else if(type==ITEM_GAS_TRANSDUCER_DATA)
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pageGasTransducer));
    }
    else
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pageOthers));
    }
}

void ControlItemEditDialogFrameOfRelayGroup::on_buttonDone_clicked()
{
    controlItem->controlType=ui->comboBox->currentData().toString();

    controlItem->powerTypePower=(ui->radioRelayOn->isChecked()?100:0);
    controlItem->motionTypeBasePower=(ui->radioBaseStateOn->isChecked()?100:0);
    controlItem->motionTrigPowerByLuxSensor=ui->checkBindLuxSensorResultAsTrigPower->isChecked();
    controlItem->motionTypeTriggerPower=(ui->radioTriggeredStateOn->isChecked()?100:0);
    controlItem->motionTypeTriggerTime=ui->spinMotionTypeTrigTime->value();
    controlItem->motionTypeHoldPower=(ui->radioTriggeredStateOn->isChecked()?100:0);
    controlItem->motionTypeHoldTime=0;
    controlItem->luxTypeIsFeedBack=false;
    controlItem->luxTypeNotFeedBackLuxOf0=ui->spinLuxTypeNotFeedBackLuxOf0->value();
    controlItem->luxTypeNotFeedBackLuxOf100=ui->spinLuxTypeNotFeedBackLuxOf100->value();
    controlItem->dimmingIntervalSec=ui->spinDimmingIntervalSec->value();
    controlItem->motionTypeTriggerShieldMSec=ui->spinMotionTriggerShieldMSec->value();

    controlItem->luxTypeBindSensorId=ui->lineLuxTypeBindSensorId->text().trimmed();
    if(controlItem->luxTypeBindSensorId.isEmpty())
    {
        controlItem->luxTypeBindSensorId="";
    }

    controlItem->motionTypeBindSensorId=ui->lineMotionTypeBindSensorId->text().trimmed();
    if(controlItem->motionTypeBindSensorId.isEmpty())
    {
        controlItem->motionTypeBindSensorId="";
    }

    controlItem->gasTrnsdcrTypeBindSensorId = ui->cmbGTBindSensorId->text();
    controlItem->gasTrnsdcrTypeTrigTypeOn = ui->radGTTrigTypeOn->isChecked();
    controlItem->gasTrnsdcrTypeTrigTypeTimer = ui->radGTTrigTypeTimer->isChecked();
    controlItem->gasTrnsdcrTypeTrigModeUpperBottom = ui->radGTTrigModeUpperBottom->isChecked();
    controlItem->gasTrnsdcrTypeTrigModeBottomUpper = ui->radGTTrigModeBottomUpper->isChecked();
    controlItem->gasTrnsdcrTypeMaxThreshold = ui->spinGTThresholdMax->value();
    controlItem->gasTrnsdcrTypeMinThreshold = ui->spinGTThresholdMin->value();
    controlItem->gasTrnsdcrTypeTrigHoldTime = ui->spinGTTrigHoldTime->value();
    controlItem->gasTrnsdcrTypeTrigShieldMSec = ui->spinGTTriggerShieldMSec->value();
    controlItem->gasTrnsdcrTypeTrigBaseStatusOn = ui->radGTBaseStatusOn->isChecked();
    controlItem->gasTrnsdcrTypeTrigBaseStatusOff = ui->radGTBaseStatusOff->isChecked();

    emit accept();
}

void ControlItemEditDialogFrameOfRelayGroup::on_buttonCancel_clicked()
{
    emit reject();
}

void ControlItemEditDialogFrameOfRelayGroup::on_checkBindLuxSensorResultAsTrigPower_clicked(bool checked)
{
    if(checked)
    {
        ui->radioTriggeredStateOn->setEnabled(false);
        ui->radioTriggeredStateOff->setEnabled(false);
    }
    else
    {
        ui->radioTriggeredStateOn->setEnabled(true);
        ui->radioTriggeredStateOff->setEnabled(true);
    }
}

void ControlItemEditDialogFrameOfRelayGroup::on_tabWidget_currentChanged(int)
{
    QWidget* currentPage = ui->tabWidget->currentWidget();
    if (currentPage == ui->pageOnOff)
    {
        ui->comboBox->setCurrentIndex(ui->comboBox->findData(ITEM_POWER_DATA));
    }
    else if (currentPage == ui->pageLuxSensor)
    {
        ui->comboBox->setCurrentIndex(ui->comboBox->findData(ITEM_LUX_SENSOR_DATA));
    }
    else if (currentPage == ui->pageMotionSensor)
    {
        ui->comboBox->setCurrentIndex(ui->comboBox->findData(ITEM_MOTION_SENSOR_DATA));
    }
    else if (currentPage == ui->pageGasTransducer)
    {
        ui->comboBox->setCurrentIndex(ui->comboBox->findData(ITEM_GAS_TRANSDUCER_DATA));
    }
    else
    {
        ui->comboBox->setCurrentIndex(ui->comboBox->findData(ITEM_NO_OPERATION_DATA));
    }
}
