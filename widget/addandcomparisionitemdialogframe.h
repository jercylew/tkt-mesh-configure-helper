#ifndef ADDANDCOMPARISIONITEMDIALOGFRAME_H
#define ADDANDCOMPARISIONITEMDIALOGFRAME_H

#include <QFrame>
#include "model/meshmodel.h"

namespace Ui {
class AddANDComparisionItemDialogFrame;
}

class AddANDComparisionItemDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit AddANDComparisionItemDialogFrame(SensorDataComparision *ptrSensorDataCmpItem,
             MeshModel *ptrMeshModel, QWidget *parent = 0);
    ~AddANDComparisionItemDialogFrame();

protected:
    void changeEvent(QEvent *ev);

private slots:

    void on_buttonDialogOk_clicked();

    void on_buttonDialogCancel_clicked();

    void on_cmbBindSensorId_currentIndexChanged(const QString &strText);

    void on_cmbSensorValue_currentIndexChanged(const QString &strValue);

private:
    void populateCmbData();
    bool isSensorBooleanData(QString strSensorId, QString strValue);

    Ui::AddANDComparisionItemDialogFrame *ui;
    SensorDataComparision *m_ptrSensorDataCmpItem;
    MeshModel *m_ptrMeshModel;

signals:
    void accept();
    void reject();
};

#endif // ADDANDCOMPARISIONITEMDIALOGFRAME_H
