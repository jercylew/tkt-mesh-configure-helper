#ifndef ADDTKTMESHDIALOG_H
#define ADDTKTMESHDIALOG_H

#include <QDialog>

namespace Ui {
class AddTKTMeshDialog;
}

class AddTKTMeshDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTKTMeshDialog(QWidget *parent = 0);
    ~AddTKTMeshDialog();
    QString getIP();

protected:
    void showEvent(QShowEvent *);

private slots:
    void on_buttonDone_clicked();

    void on_buttonCancel_clicked();

private:
    Ui::AddTKTMeshDialog *ui;
};

#endif // ADDTKTMESHDIALOG_H
