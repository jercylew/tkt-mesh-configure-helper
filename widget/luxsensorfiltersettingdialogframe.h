#ifndef LUXSENSORFILTERSETTINGDIALOGFRAME_H
#define LUXSENSORFILTERSETTINGDIALOGFRAME_H

#include <QFrame>

class MeshModel;

namespace Ui {
class LuxSensorFilterSettingDialogFrame;
}

class LuxSensorFilterSettingDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit LuxSensorFilterSettingDialogFrame(MeshModel *meshModel, QWidget *parent = 0);
    ~LuxSensorFilterSettingDialogFrame();

private slots:
    void on_buttonDialogDone_clicked();

    void on_buttonDialogCancel_clicked();

signals:
    void accept();
    void reject();

private:
    Ui::LuxSensorFilterSettingDialogFrame *ui;
    MeshModel *m_meshModel;
};

#endif // LUXSENSORFILTERSETTINGDIALOGFRAME_H
