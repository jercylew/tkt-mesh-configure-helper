#include "addandcomparisionitemdialogframe.h"
#include "ui_addandcomparisionitemdialogframe.h"
#include "model/sensorsmodel.h"
#include "domain/temperaturehumiditysensor.h"
#include "domain/refrgtemperaturehumiditysensor.h"
#include "domain/realmotionsensor.h"
#include "domain/realmotionandluxcamerasensor.h"
#include "domain/handwashingsensor.h"
#include "domain/uvlightsensor.h"
#include "domain/uvsterilizer.h"
#include <QMessageBox>
#include "dialogbuilder.h"

AddANDComparisionItemDialogFrame::AddANDComparisionItemDialogFrame(
        SensorDataComparision *ptrSensorDataCmpItem, MeshModel *ptrMeshModel,
                 QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AddANDComparisionItemDialogFrame),
    m_ptrSensorDataCmpItem(ptrSensorDataCmpItem),
    m_ptrMeshModel(ptrMeshModel)
{
    ui->setupUi(this);

    //Populate the items into combobox
    populateCmbData();
}

void AddANDComparisionItemDialogFrame::populateCmbData()
{
    for (int i = 0;i < m_ptrMeshModel->sensorsModel()->count();i++)
    {
        ui->cmbBindSensorId->addItem(m_ptrMeshModel->sensorsModel()->at(i)->sensorId(),
                                     m_ptrMeshModel->sensorsModel()->at(i)->typeText());
    }
    ui->cmbCmpOperator->addItem(">", ">");
    ui->cmbCmpOperator->addItem("<", "<");
}

bool AddANDComparisionItemDialogFrame::isSensorBooleanData(QString strSensorId, QString strValue)
{
    bool bRet = false;
    if (strSensorId.startsWith(HAND_WASHING_SENSOR_SHORT_TYPE_TEXT) ||
            strSensorId.startsWith(REAL_MOTION_SENSOR_SHORT_TYPE_TEXT) ||
            strSensorId.startsWith(UVLIGHT_SENSOR_SHORT_TYPE_TEXT))
    {
        bRet = true;
    }
    else if (strSensorId.startsWith(UV_STERILIZER_SHORT_TYPE_TEXT))
    {
        if (strValue == "CONF" || strValue == "STERIZE")
        {
            bRet = true;
        }
    }
    else
    {
    }

    return bRet;
}

AddANDComparisionItemDialogFrame::~AddANDComparisionItemDialogFrame()
{
    delete ui;
}

void AddANDComparisionItemDialogFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void AddANDComparisionItemDialogFrame::on_buttonDialogOk_clicked()
{
    m_ptrSensorDataCmpItem->m_strSensorId = ui->cmbBindSensorId->currentText();
    m_ptrSensorDataCmpItem->m_strOperator = ui->cmbCmpOperator->currentText();
    QString strSensorValue = ui->cmbSensorValue->currentText();
    m_ptrSensorDataCmpItem->m_strValue = " ";
    if (strSensorValue != "DEFAULT")
    {
        m_ptrSensorDataCmpItem->m_strValue = QString("[%1]").arg(strSensorValue);
    }

    m_ptrSensorDataCmpItem->m_dDataThresold = ui->edtDataThresold->text().toDouble();
    emit accept();
}

void AddANDComparisionItemDialogFrame::on_buttonDialogCancel_clicked()
{
    emit reject();
}

void AddANDComparisionItemDialogFrame::on_cmbBindSensorId_currentIndexChanged(const QString &strText)
{
    ui->cmbSensorValue->clear();

    if (strText.startsWith(TEMPERATURE_HUMIDITY_SENSOR_SHORT_TYPE_TEXT))
    {
        ui->cmbSensorValue->addItem("TEMP", "TEMP");
        ui->cmbSensorValue->addItem("HUM", "HUM");
    }
    else if (strText.startsWith(REFRG_TEMPERATURE_HUMIDITY_SENSOR_SHORT_TYPE_TEXT))
    {
        ui->cmbSensorValue->addItem("TEMP1", "TEMP1");
        ui->cmbSensorValue->addItem("HUM1", "HUM1");
        ui->cmbSensorValue->addItem("TEMP2", "TEMP2");
        ui->cmbSensorValue->addItem("HUM2", "HUM2");
        ui->cmbSensorValue->addItem("VOL", "VOL");
    }
    else if (strText.startsWith(UV_STERILIZER_SHORT_TYPE_TEXT))
    {
        ui->cmbSensorValue->addItem("CONF", "CONF");
        ui->cmbSensorValue->addItem("STERIZE", "STERIZE");
        ui->cmbSensorValue->addItem("WIFI", "WIFI");
        ui->cmbSensorValue->addItem("FUNC", "FUNC");
    }
    else if (strText.startsWith(REAL_MOTION_LUX_CAMERA_SHORT_TYPE_TEXT))
    {
        ui->cmbSensorValue->addItem("TRIG1", "TRIG1");
        ui->cmbSensorValue->addItem("TRIG1", "TRIG1");
        ui->cmbSensorValue->addItem("TRIG1", "TRIG1");
        ui->cmbSensorValue->addItem("TRIG1", "TRIG1");
        ui->cmbSensorValue->addItem("LUX1", "LUX1");
        ui->cmbSensorValue->addItem("LUX2", "LUX2");
        ui->cmbSensorValue->addItem("LUX3", "LUX3");
        ui->cmbSensorValue->addItem("LUX4", "LUX4");
    }
    else
    {
        ui->cmbSensorValue->addItem("DEFAULT", "DEFAULT");
    }
}

void AddANDComparisionItemDialogFrame::on_cmbSensorValue_currentIndexChanged(const QString &strValue)
{
    ui->cmbCmpOperator->clear();
    if (isSensorBooleanData(ui->cmbBindSensorId->currentText(), strValue))
    {
        ui->cmbCmpOperator->addItem("==", "==");
    }
    else
    {
        ui->cmbCmpOperator->addItem(">", ">");
        ui->cmbCmpOperator->addItem("<", "<");
    }
}
