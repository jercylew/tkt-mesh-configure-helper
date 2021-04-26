#ifndef VIRTUALMOTIONSENSOREDITDIALOGFRAME_H
#define VIRTUALMOTIONSENSOREDITDIALOGFRAME_H

#include <QFrame>

class MeshModel;
class VirtualMotionSensor;

namespace Ui {
class VirtualMotionSensorEditDialogFrame;
}

class VirtualMotionSensorEditDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit VirtualMotionSensorEditDialogFrame(MeshModel *meshModel, VirtualMotionSensor *virtualMotionSensor, QWidget *parent = 0);
    ~VirtualMotionSensorEditDialogFrame();

private slots:
    void on_buttonDialogBottomDone_clicked();

    void on_buttonDialogBottomClose_clicked();

    void on_buttonAddBindingSensor_clicked();

    void on_buttonRemoveBindingSensor_clicked();

private:
    Ui::VirtualMotionSensorEditDialogFrame *ui;
    MeshModel *m_meshModel;
    VirtualMotionSensor *m_virtualMotionSensor;

    QList<QString> canSelectedSensorIdList;
    QList<QString> selectedIdList;

    void updateListWidget();

signals:
    void accept();
    void reject();
};

#endif // VIRTUALMOTIONSENSOREDITDIALOGFRAME_H
