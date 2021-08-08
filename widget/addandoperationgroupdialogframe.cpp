#include "addandoperationgroupdialogframe.h"
#include "ui_addandoperationgroupdialogframe.h"
#include "model/sensordatacomparisionlistmodel.h"
#include "model/sensordatacomparisiontableviewmodel.h"
#include <QMessageBox>
#include "dialogbuilder.h"

AddANDOperationGroupDialogFrame::AddANDOperationGroupDialogFrame(MeshModel *meshModel,
                                 QVector<SensorDataComparision *> *pvecSensorsANDOperation, QWidget *parent):
    QFrame(parent),
    ui(new Ui::AddANDOperationGroupDialogFrame),
    m_pvecSensorsANDOperations(pvecSensorsANDOperation),
    m_ptrMeshModel(meshModel)
{
    ui->setupUi(this);

    m_ptrDataCmpListModel = new SensorDataComparisionListModel;
    m_ptrDataCmpTableViewModel = new SensorDataComparisionTableViewModel(m_ptrDataCmpListModel);
    ui->tableANDOperationGroupView->setModel(m_ptrDataCmpTableViewModel);

    ui->tableANDOperationGroupView->setEditTriggers(QTableView::NoEditTriggers);
    ui->tableANDOperationGroupView->verticalHeader()->setVisible( true );
    ui->tableANDOperationGroupView->horizontalHeader()->setStretchLastSection( true );
    ui->tableANDOperationGroupView->setSelectionMode(QTableView::ExtendedSelection);
    ui->tableANDOperationGroupView->setSelectionBehavior(QTableView::SelectRows);
}

AddANDOperationGroupDialogFrame::~AddANDOperationGroupDialogFrame()
{
    m_ptrDataCmpListModel->clear();
    delete ui;
}

void AddANDOperationGroupDialogFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void AddANDOperationGroupDialogFrame::on_buttonDialogOk_clicked()
{
    for (int i = 0;i < m_ptrDataCmpListModel->count();i++)
    {
        m_pvecSensorsANDOperations->append(m_ptrDataCmpListModel->at(i));
    }
    emit accept();
}

void AddANDOperationGroupDialogFrame::on_buttonDialogCancel_clicked()
{
    emit reject();
}


void AddANDOperationGroupDialogFrame::on_btnAddSensorDataCompararision_clicked()
{
    SensorDataComparision *ptrDataCmp = new SensorDataComparision;
    QDialog *dialog = DialogBuilder::getInstance()->buildAddANDOperationItemDialog(m_ptrMeshModel,
                      tr("CompositeSensor.AddANDOperationsGroup"), ptrDataCmp);

    if (dialog->exec() != QDialog::Accepted)
    {
        return;
    }

    if (ptrDataCmp->m_strSensorId == "" ||
            ptrDataCmp->m_strOperator == "")
    {
        return;
    }

    m_ptrDataCmpListModel->append(ptrDataCmp);
}

void AddANDOperationGroupDialogFrame::on_btnClear_clicked()
{
    m_ptrDataCmpListModel->clear();
}
