#ifndef NODESUPDATEDIALOG_H
#define NODESUPDATEDIALOG_H

#include <QDialog>
#include "model/tktmeshesupdatemodel.h"
#include "model/tktmeshupdatetableviewmodel.h"
#include <QSet>

namespace Ui {
class NodesUpdateDialog;
}

class NodesUpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NodesUpdateDialog(QWidget *parent = 0);
    ~NodesUpdateDialog();

private slots:
    void on_buttonSelectUpdateFile_clicked();

    void on_buttonUpdate_clicked();

    void on_buttonBottomClose_clicked();

    void doScanProgress(int total, int finished);

    void on_buttonScanTKTMesh_clicked();

    void on_buttonAddTKTMesh_clicked();

    void doUpdateNodeFinished();

    void doUpdateWorkStarted(const QString &hostId);

    void doUpdateWorkFinished(const QString &hostId, bool ok);

    void doUpdateWorkProgressReport(const QString &hostId, int progress);

private:
    TKTMeshesUpdateModel *m_tktMeshesUpdateModel;
    TKTMeshUpdateTableViewModel *m_tktMeshUpdateTableViewModel;
    QList<QString> m_updateHostIDList;
    void startUpdateTasks();

private:
    Ui::NodesUpdateDialog *ui;
    int m_idleTaskNumber;
};

#endif // NODESUPDATEDIALOG_H
