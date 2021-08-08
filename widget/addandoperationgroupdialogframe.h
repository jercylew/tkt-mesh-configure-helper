#ifndef ADDANDOPERATIONGROUPDIALOGFRAME_H
#define ADDANDOPERATIONGROUPDIALOGFRAME_H

#include <QFrame>
#include "model/meshmodel.h"

namespace Ui {
class AddANDOperationGroupDialogFrame;
}

class SensorDataComparisionTableViewModel;
class SensorDataComparisionListModel;

class AddANDOperationGroupDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit AddANDOperationGroupDialogFrame(MeshModel *meshModel,
             QVector<SensorDataComparision*> *pvecSensorsANDOperation,
                                             QWidget *parent = 0);
    ~AddANDOperationGroupDialogFrame();

protected:
    void changeEvent(QEvent *ev);

private slots:

    void on_buttonDialogOk_clicked();

    void on_buttonDialogCancel_clicked();

    void on_btnAddSensorDataCompararision_clicked();

    void on_btnClear_clicked();

private:
    Ui::AddANDOperationGroupDialogFrame *ui;
    QVector<SensorDataComparision*> *m_pvecSensorsANDOperations;
    MeshModel *m_ptrMeshModel;
    SensorDataComparisionTableViewModel *m_ptrDataCmpTableViewModel;
    SensorDataComparisionListModel *m_ptrDataCmpListModel;

signals:
    void accept();
    void reject();
};

#endif // ADDANDOPERATIONGROUPDIALOGFRAME_H
