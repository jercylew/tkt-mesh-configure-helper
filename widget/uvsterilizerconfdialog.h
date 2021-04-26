#ifndef UVSTERILIZERCONFDIALOG_H
#define UVSTERILIZERCONFDIALOG_H

#include <QDialog>

namespace Ui {
class UVSterilizerConfDialog;
}

class MeshModel;

class UVSterilizerConfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UVSterilizerConfDialog(MeshModel *meshModel, QList<quint16> addrList,
                                    QWidget *parent = nullptr);
    explicit UVSterilizerConfDialog(MeshModel *meshModel, quint8 groupId,
                                    QWidget *parent = nullptr);
    ~UVSterilizerConfDialog();

protected:
    void changeEvent(QEvent *ev);

private slots:
    void on_buttonDialogClose_clicked();
    void on_buttonDialogBottomClose_clicked();
    void on_buttonDialogApply_clicked();

private:
    Ui::UVSterilizerConfDialog *ui;
    QList<quint16> addrList;
    bool isGroupMode;
    quint8 groupId;
    MeshModel *m_meshModel;
};

#endif // LUMINAIREDIMMINGDIALOG_H
