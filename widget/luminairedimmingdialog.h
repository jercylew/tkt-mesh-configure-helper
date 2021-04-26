#ifndef LUMINAIREDIMMINGDIALOG_H
#define LUMINAIREDIMMINGDIALOG_H

#include <QDialog>

namespace Ui {
class LuminaireDimmingDialog;
}

class MeshModel;

class LuminaireDimmingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LuminaireDimmingDialog(MeshModel *meshModel, QList<quint16> addrList, int currentValue, QWidget *parent = 0);
    explicit LuminaireDimmingDialog(MeshModel *meshModel, quint8 groupId, int currentValue, QWidget *parent = 0);
    ~LuminaireDimmingDialog();

    quint8 getCurrentBrightness();

protected:
    void changeEvent(QEvent *ev);

private slots:

    void on_sliderDimming_valueChanged(int value);

    void on_buttonDialogClose_clicked();

    void on_buttonDialogBottomClose_clicked();

    void on_spinDimmingDialogBrightnessValue_valueChanged(int arg1);

    void on_buttonDialogApply_clicked();

private:
    Ui::LuminaireDimmingDialog *ui;
    QList<quint16> addrList;
    bool isGroupMode;
    quint8 groupId;
    MeshModel *m_meshModel;
};

#endif // LUMINAIREDIMMINGDIALOG_H
