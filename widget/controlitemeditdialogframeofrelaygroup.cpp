#include "controlitemeditdialogframeofrelaygroup.h"
#include "ui_controlitemeditdialogframeofrelaygroup.h"
#include "db/meshdbmanager.h"
#include "dialogbuilder.h"
#include <QMessageBox>
#include <QTextEdit>
#include <QLabel>

#define ITEM_POWER_DATA             "power"
#define ITEM_LUX_SENSOR_DATA        "lux_sensor"
#define ITEM_MOTION_SENSOR_DATA     "motion_sensor"
#define ITEM_GAS_TRANSDUCER_DATA    "gas_transducer"
#define ITEM_COMPOSITE_SENSOR_DATA    "composite_sensor"
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
    ui->comboBox->addItem(tr("Composite Sensor"), ITEM_COMPOSITE_SENSOR_DATA);
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
    ui->cmbGTBindSensorId->addAccessSensorType(Sensor::OzoneSensor);
    ui->cmbGTBindSensorId->addAccessSensorType(Sensor::FlammableGasSensor);

    ui->labelSensorGroupAddState->setText(tr("Added %1 sensors in %2 groups").arg(0).arg(0));
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
    ui->radGTTrigModeUpperBottom->setChecked(controlItem->gasTrnsdcrTypeTrigModeUpperBottom);
    ui->radGTTrigModeBottomUpper->setChecked(controlItem->gasTrnsdcrTypeTrigModeBottomUpper);
    ui->spinGTThresholdMax->setValue(controlItem->gasTrnsdcrTypeMaxThreshold);
    ui->spinGTThresholdMin->setValue(controlItem->gasTrnsdcrTypeMinThreshold);
    ui->spinGTTriggerShieldMSec->setValue(controlItem->gasTrnsdcrTypeTrigShieldMSec);
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
    else if(type==ITEM_COMPOSITE_SENSOR_DATA)
    {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->pageCompositeSensors));
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
    QString type = ui->comboBox->currentData().toString();

    if (type == ITEM_GAS_TRANSDUCER_DATA &&
            ui->spinGTThresholdMax->value() < ui->spinGTThresholdMin->value())
    {
        QMessageBox::warning(this, "提示", "上限值必须大于下限值");
        return;
    }

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
    controlItem->gasTrnsdcrTypeTrigModeUpperBottom = ui->radGTTrigModeUpperBottom->isChecked();
    controlItem->gasTrnsdcrTypeTrigModeBottomUpper = ui->radGTTrigModeBottomUpper->isChecked();
    controlItem->gasTrnsdcrTypeMaxThreshold = ui->spinGTThresholdMax->value();
    controlItem->gasTrnsdcrTypeMinThreshold = ui->spinGTThresholdMin->value();
    controlItem->gasTrnsdcrTypeTrigShieldMSec = ui->spinGTTriggerShieldMSec->value();

    //Composite Sensor Policy
    controlItem->compositeSensorTypeCmpExpr = getCompositeSensorComparisionExpression();
    qDebug() << "EXPRESSION:" << controlItem->compositeSensorTypeCmpExpr;
    controlItem->compositeSensorTypeTrigShieldMSec = 1000;

    emit accept();
}

QString ControlItemEditDialogFrameOfRelayGroup::CmpOptr2String(QString strOptr)
{
    QString strOptrText = "";
    if (strOptr == ">")
    {
        strOptrText = tr("Greater than");
    }
    else if (strOptr == "<")
    {
        strOptrText = tr("Less than");
    }
    else if (strOptr == "==")
    {
        strOptrText = tr("Equal");
    }
    else
    {
        strOptrText = "?";
    }

    return strOptrText;
}

QString ControlItemEditDialogFrameOfRelayGroup::getAndCmpExpressionGroup(const QVector<SensorDataComparision *> &vecCmp)
{
    QString strExpr = "";
    if (vecCmp.empty())
    {
        return strExpr;
    }

    QString strCmpItem = QString("%1%2%3%4").arg(
                vecCmp.at(0)->m_strSensorId, vecCmp.at(0)->m_strValue,
                vecCmp.at(0)->m_strOperator,
                QString::number(vecCmp.at(0)->m_dDataThresold));

    strExpr.append(strCmpItem);
    for (int i = 1;i < vecCmp.size();i++)
    {
        strCmpItem = QString("&&%1%2%3%4").arg(
                    vecCmp.at(i)->m_strSensorId, vecCmp.at(i)->m_strValue,
                    vecCmp.at(i)->m_strOperator,
                    QString::number(vecCmp.at(i)->m_dDataThresold));
        strExpr.append(strCmpItem);
    }

    return strExpr;
}

QString ControlItemEditDialogFrameOfRelayGroup::getCompositeSensorComparisionExpression()
{
    QString strExpression = "";
    if (m_vecSensorDataCmpORGroups.empty())
    {
        return strExpression;
    }
    strExpression.append(getAndCmpExpressionGroup(m_vecSensorDataCmpORGroups.at(0)));

    for (int i = 1;i < m_vecSensorDataCmpORGroups.size();i++)
    {
        if (m_vecSensorDataCmpORGroups.at(i).empty())
        {
            continue;
        }
        strExpression.append("||");
        strExpression.append(getAndCmpExpressionGroup(m_vecSensorDataCmpORGroups.at(i)));
    }

    return strExpression;
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
    else if (currentPage == ui->pageCompositeSensors)
    {
        ui->comboBox->setCurrentIndex(ui->comboBox->findData(ITEM_COMPOSITE_SENSOR_DATA));
    }
    else
    {
        ui->comboBox->setCurrentIndex(ui->comboBox->findData(ITEM_NO_OPERATION_DATA));
    }
}


void ControlItemEditDialogFrameOfRelayGroup::on_btnAddANDGroup_clicked()
{
    QVector<SensorDataComparision*> vecCmp;
    QDialog *dialog = DialogBuilder::getInstance()->buildAddANDOperationGroupDialog(m_meshModel,
                      tr("CompositeSensor.AddANDOperationsGroup"), &vecCmp);

    if (dialog->exec() != QDialog::Accepted)
    {
        return;
    }

    if (vecCmp.isEmpty())
    {
        return;
    }

    QTextEdit *txtANDGroup = new QTextEdit(ui->pageCompositeSensors);
    QString strHtml = QString("<div><div style='display:inline-block'>%1%2 %3 %4</div>")
            .arg(vecCmp.at(0)->m_strSensorId,
                 vecCmp.at(0)->m_strValue,
                 CmpOptr2String(vecCmp.at(0)->m_strOperator),
                 QString::number(vecCmp.at(0)->m_dDataThresold));
    for (int i = 1;i < vecCmp.size();i++)
    {
        strHtml.append(tr("<h3 style='display:inline-block'>且</h3> "));
        strHtml.append(QString("<div style='display:inline-block'>%1%2 %3 %4</div>")
                       .arg(vecCmp.at(i)->m_strSensorId,
                            vecCmp.at(i)->m_strValue,
                            CmpOptr2String(vecCmp.at(i)->m_strOperator),
                            QString::number(vecCmp.at(i)->m_dDataThresold)));
    }
    strHtml.append("</div>");
    txtANDGroup->setHtml(strHtml);
    txtANDGroup->setReadOnly(true);

    if (m_vecSensorDataCmpORGroups.size()>0)
    {
        QLabel *labelORGroup = new QLabel(ui->pageCompositeSensors);
        labelORGroup->setText(tr("Or"));
        ui->vLayoutORGroups->addWidget(labelORGroup);
        m_vecORCmpGroupVLayoutChildren.append(labelORGroup);
    }
    ui->vLayoutORGroups->addWidget(txtANDGroup);
    m_vecORCmpGroupVLayoutChildren.append(txtANDGroup);
    m_vecSensorDataCmpORGroups.append(vecCmp);

    int nNrOfGroups = m_vecSensorDataCmpORGroups.size();
    for (int i = 0;i < vecCmp.size();i++)
    {
        if (m_mapSensorCmpCount.contains(vecCmp.at(i)->m_strSensorId))
        {
            m_mapSensorCmpCount[vecCmp.at(i)->m_strSensorId]++;
        }
        else
        {
            m_mapSensorCmpCount.insert(vecCmp.at(i)->m_strSensorId, 1);
        }
    }

    ui->labelSensorGroupAddState->setText(tr("Added %1 sensors in %2 groups")
                                          .arg(m_mapSensorCmpCount.size())
                                          .arg(nNrOfGroups));
}

void ControlItemEditDialogFrameOfRelayGroup::on_btnClearGroups_clicked()
{
    for (int i = 0;i < m_vecORCmpGroupVLayoutChildren.size();i++)
    {
        ui->vLayoutORGroups->removeWidget(m_vecORCmpGroupVLayoutChildren.at(i));
        delete m_vecORCmpGroupVLayoutChildren[i];
    }
    m_vecORCmpGroupVLayoutChildren.clear();

    for (int i = 0;i < m_vecSensorDataCmpORGroups.size();i++)
    {
        m_vecSensorDataCmpORGroups[i].clear();
    }

    m_vecSensorDataCmpORGroups.clear();
    m_mapSensorCmpCount.clear();
    ui->labelSensorGroupAddState->setText(tr("Added %1 sensors in %2 groups")
                                          .arg(0).arg(0));
}
