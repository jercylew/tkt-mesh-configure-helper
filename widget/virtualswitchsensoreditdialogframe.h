#ifndef VIRTUALSWITCHSENSOREDITDIALOGFRAME_H
#define VIRTUALSWITCHSENSOREDITDIALOGFRAME_H

#include <QFrame>

class VirtualSwitchSensor;
class MeshModel;

namespace Ui {
class VirtualSwitchSensorEditDialogFrame;
}

class VirtualSwitchSensorEditDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit VirtualSwitchSensorEditDialogFrame(MeshModel *meshModel, VirtualSwitchSensor *virtualSwitchSensor, QWidget *parent = 0);
    ~VirtualSwitchSensorEditDialogFrame();

private slots:
    void on_buttonDialogBottomDone_clicked();

    void on_buttonDialogBottomClose_clicked();

private:
    Ui::VirtualSwitchSensorEditDialogFrame *ui;
    VirtualSwitchSensor *m_virtualSwitchSensor;
    MeshModel *m_meshModel;

    void loadControlTypes();

signals:
    void accept();
    void reject();
};

#endif // VIRTUALSWITCH6SENSOREDITDIALOGFRAME_H
