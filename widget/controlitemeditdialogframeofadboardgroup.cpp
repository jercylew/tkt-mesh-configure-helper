#include "controlitemeditdialogframeofadboardgroup.h"
#include "ui_controlitemeditdialogframeofadboardgroup.h"
#include "model/meshmodel.h"
#include "db/meshdbmanager.h"
#include "domain/timelinecontrolitemofadboardgroup.h"

#define ITEM_POWER_DATA             "power"
#define ITEM_LUX_SENSOR_DATA        "lux_sensor"
#define ITEM_MOTION_SENSOR_DATA     "motion_sensor"
#define ITEM_GAS_TRANSDUCER_DATA    "gas_transducer"
#define ITEM_NO_OPERATION_DATA      "no_operation"

ControlItemEditDialogFrameOfADBoardGroup::ControlItemEditDialogFrameOfADBoardGroup(MeshModel *meshModel, TimeLineControlItem *controlItem, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ControlItemEditDialogFrameOfADBoardGroup),
    m_meshModel(meshModel)
{
    ui->setupUi(this);

    ui->comboBox->addItem(tr("Power"), ITEM_POWER_DATA);
    ui->comboBox->addItem(tr("Lux Sensor"), ITEM_LUX_SENSOR_DATA);
    ui->comboBox->addItem(tr("Motion Sensor"), ITEM_MOTION_SENSOR_DATA);
    ui->comboBox->addItem(tr("No Operation"), ITEM_NO_OPERATION_DATA);

    ui->buttonDone->setCursor(Qt::PointingHandCursor);
    ui->buttonCancel->setCursor(Qt::PointingHandCursor);

    ui->lineMotionTypeBindSensorId->setMeshModel(meshModel);
    ui->lineMotionTypeBindSensorId->addAccessSensorType(Sensor::VirtualMotionSensor);
    ui->lineMotionTypeBindSensorId->addAccessSensorType(Sensor::RealMotionSensor);

    ui->lineLuxTypeBindSensorId->setMeshModel(meshModel);
    ui->lineLuxTypeBindSensorId->addAccessSensorType(Sensor::LuxSensor);

    setControlItem(controlItem);
}

ControlItemEditDialogFrameOfADBoardGroup::~ControlItemEditDialogFrameOfADBoardGroup()
{
    delete ui;
}

void ControlItemEditDialogFrameOfADBoardGroup::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void ControlItemEditDialogFrameOfADBoardGroup::setControlItem(TimeLineControlItem *controlItem)
{
    this->controlItem=controlItem;
    setType(controlItem->controlType);
    ui->spinPowerTypePower->setValue((quint16)controlItem->powerTypePower);
    ui->spinMotionTypeBasePower->setValue((quint16)controlItem->motionTypeBasePower);
    if(controlItem->motionTrigPowerByLuxSensor)
    {
        ui->checkBindLuxSensorResultAsTrigPower->setChecked(true);
        ui->spinMotionTypeTrigPower->setEnabled(false);
    }
    else
    {
        ui->checkBindLuxSensorResultAsTrigPower->setChecked(false);
        ui->spinMotionTypeTrigPower->setEnabled(true);
    }
    ui->spinMotionTypeTrigPower->setValue((quint16)controlItem->motionTypeTriggerPower);
    ui->spinMotionTypeTrigTime->setValue((quint16)controlItem->motionTypeTriggerTime);
    ui->spinMotionTypeHoldPower->setValue((quint16)controlItem->motionTypeHoldPower);
    ui->spinMotionTypeHoldTime->setValue((quint16)controlItem->motionTypeHoldTime);
    ui->lineMotionTypeBindSensorId->setText(controlItem->motionTypeBindSensorId);
    if(controlItem->luxTypeIsFeedBack)
    {
        ui->radioLuxTypeFeedBack->setChecked(true);
        ui->radioLuxTypeNotFeedBack->setChecked(false);
    }
    else
    {
        ui->radioLuxTypeFeedBack->setChecked(false);
        ui->radioLuxTypeNotFeedBack->setChecked(true);
    }
    ui->lineLuxTypeBindSensorId->setText(controlItem->luxTypeBindSensorId);
    ui->spinLuxTypeFeedBackLux->setValue((quint16)controlItem->luxTypeFeedBackLux);
    ui->spinLuxTypeFeedBackFloatLux->setValue((quint16)controlItem->luxTypeFeedBackFloatLux);
    ui->spinLuxTypeFeedBackStepPower->setValue(controlItem->luxTypeFeedBackStepPower);
    ui->spinLuxTypeNotFeedBackLuxOf0->setValue((quint16)controlItem->luxTypeNotFeedBackLuxOf0);
    ui->spinLuxTypeNotFeedBackLuxOf100->setValue((quint16)controlItem->luxTypeNotFeedBackLuxOf100);
    ui->spinDimmingIntervalSec->setValue(controlItem->dimmingIntervalSec);
    ui->spinMotionTriggerShieldMSec->setValue(controlItem->motionTypeTriggerShieldMSec);

    setUiLuxSensorFeedBack(controlItem->luxTypeIsFeedBack);

    if(controlItem->groupType()==NodeGroup::ADBoardGroup)
    {
        TimeLineControlItemOfADBoardGroup *adboardControlItem=static_cast<TimeLineControlItemOfADBoardGroup*>(controlItem);
        ui->radioTriggerPowerAsPower->setChecked(!adboardControlItem->motionTypeTriggerPowerAsDisplayMode);
        ui->radioTriggerPowerAsDisplayMode->setChecked(adboardControlItem->motionTypeTriggerPowerAsDisplayMode);
        ui->radioHoldPowerAsPower->setChecked(!adboardControlItem->motionTypeHoldPowerAsDisplayMode);
        ui->radioHoldPowerAsDisplayMode->setChecked(adboardControlItem->motionTypeHoldPowerAsDisplayMode);
        ui->radioBasePowerAsPower->setChecked(!adboardControlItem->motionTypeBasePowerAsDisplayMode);
        ui->radioBasePowerAsDisplayMode->setChecked(adboardControlItem->motionTypeBasePowerAsDisplayMode);
    }
}

void ControlItemEditDialogFrameOfADBoardGroup::setType(const QString &type)
{
    ui->comboBox->setCurrentIndex(ui->comboBox->findData(type));
    if(type == ITEM_POWER_DATA)
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pagePower));
    }
    else if(type == ITEM_MOTION_SENSOR_DATA)
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pageMotionSensor));
    }
    else if(type == ITEM_LUX_SENSOR_DATA)
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pageLuxSensor));
    }
}

void ControlItemEditDialogFrameOfADBoardGroup::setUiLuxSensorFeedBack(bool feedBack)
{
    if(feedBack)
    {
        ui->spinLuxTypeFeedBackFloatLux->setEnabled(true);
        ui->spinLuxTypeFeedBackLux->setEnabled(true);
        ui->spinLuxTypeFeedBackStepPower->setEnabled(true);
        ui->spinLuxTypeNotFeedBackLuxOf0->setEnabled(false);
        ui->spinLuxTypeNotFeedBackLuxOf100->setEnabled(false);
    }
    else
    {
        ui->spinLuxTypeFeedBackFloatLux->setEnabled(false);
        ui->spinLuxTypeFeedBackLux->setEnabled(false);
        ui->spinLuxTypeFeedBackStepPower->setEnabled(false);
        ui->spinLuxTypeNotFeedBackLuxOf0->setEnabled(true);
        ui->spinLuxTypeNotFeedBackLuxOf100->setEnabled(true);
    }
}

void ControlItemEditDialogFrameOfADBoardGroup::on_comboBox_activated(int)
{
    QString type=ui->comboBox->currentData().toString();
    if(type == ITEM_POWER_DATA)
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pagePower));
    }
    else if(type == ITEM_LUX_SENSOR_DATA)
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pageLuxSensor));
    }
    else if(type == ITEM_MOTION_SENSOR_DATA)
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pageMotionSensor));
    }
    else
    {

    }
}

void ControlItemEditDialogFrameOfADBoardGroup::on_radioLuxTypeFeedBack_clicked(bool)
{
    setUiLuxSensorFeedBack(true);
}

void ControlItemEditDialogFrameOfADBoardGroup::on_radioLuxTypeNotFeedBack_clicked(bool)
{
    setUiLuxSensorFeedBack(false);
}

void ControlItemEditDialogFrameOfADBoardGroup::on_buttonDone_clicked()
{
    controlItem->controlType=ui->comboBox->currentData().toString();

    controlItem->powerTypePower=(quint8)(ui->spinPowerTypePower->value());
    controlItem->motionTypeBasePower=(quint8)(ui->spinMotionTypeBasePower->value());
    controlItem->motionTrigPowerByLuxSensor=ui->checkBindLuxSensorResultAsTrigPower->isChecked();
    controlItem->motionTypeTriggerPower=(quint8)(ui->spinMotionTypeTrigPower->value());
    controlItem->motionTypeTriggerTime=ui->spinMotionTypeTrigTime->value();
    controlItem->motionTypeHoldPower=(quint8)(ui->spinMotionTypeHoldPower->value());
    controlItem->motionTypeHoldTime=ui->spinMotionTypeHoldTime->value();
    controlItem->luxTypeIsFeedBack=ui->radioLuxTypeFeedBack->isChecked();
    controlItem->luxTypeFeedBackLux=(quint16)ui->spinLuxTypeFeedBackLux->value();
    controlItem->luxTypeFeedBackFloatLux=(quint16)(ui->spinLuxTypeFeedBackFloatLux->value());
    controlItem->luxTypeFeedBackStepPower=ui->spinLuxTypeFeedBackStepPower->value();
    controlItem->luxTypeNotFeedBackLuxOf0=(quint16)(ui->spinLuxTypeNotFeedBackLuxOf0->value());
    controlItem->luxTypeNotFeedBackLuxOf100=(quint16)(ui->spinLuxTypeNotFeedBackLuxOf100->value());
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

    if(controlItem->groupType()==NodeGroup::ADBoardGroup)
    {
        TimeLineControlItemOfADBoardGroup *adboardControlItem=static_cast<TimeLineControlItemOfADBoardGroup*>(controlItem);
        adboardControlItem->motionTypeTriggerPowerAsDisplayMode=ui->radioTriggerPowerAsDisplayMode->isChecked();
        adboardControlItem->motionTypeHoldPowerAsDisplayMode=ui->radioHoldPowerAsDisplayMode->isChecked();
        adboardControlItem->motionTypeBasePowerAsDisplayMode=ui->radioBasePowerAsDisplayMode->isChecked();
    }

    emit accept();
}

void ControlItemEditDialogFrameOfADBoardGroup::on_buttonCancel_clicked()
{
    emit reject();
}

void ControlItemEditDialogFrameOfADBoardGroup::on_checkBindLuxSensorResultAsTrigPower_clicked(bool checked)
{
    if(checked)
    {
        ui->spinMotionTypeTrigPower->setEnabled(false);
    }
    else
    {
        ui->spinMotionTypeTrigPower->setEnabled(true);
    }
}

void ControlItemEditDialogFrameOfADBoardGroup::on_tabWidget_currentChanged(int)
{
    QWidget* currentPage = ui->tabWidget->currentWidget();
    if (currentPage == ui->pagePower)
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
    else
    {

    }
}
