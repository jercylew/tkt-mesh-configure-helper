#ifndef CONNECTHOSTDIALOG_H
#define CONNECTHOSTDIALOG_H

#include <QDialog>

class TKTMeshModel;

namespace Ui {
class ConnectHostDialog;
}

class ConnectHostDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectHostDialog(TKTMeshModel *tktMeshModel, QWidget *parent = 0);
    ~ConnectHostDialog();

private:
    Ui::ConnectHostDialog *ui;
    TKTMeshModel *m_tktMeshModel;

private slots:
    void doConnectHostFinished(bool ok);
    void doTimeout();
};

#endif // CONNECTHOSTDIALOG_H
