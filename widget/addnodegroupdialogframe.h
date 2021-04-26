#ifndef ADDNODEGROUPDIALOG_H
#define ADDNODEGROUPDIALOG_H

#include <QFrame>

namespace Ui {
class AddNodeGroupDialogFrame;
}

class MeshModel;

class AddNodeGroupDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit AddNodeGroupDialogFrame(MeshModel *meshModel, QWidget *parent = 0);
    ~AddNodeGroupDialogFrame();

    QString getGroupName();
    QColor getGroupColor();

protected:
    void changeEvent(QEvent *ev);

private slots:
    void on_buttonGroupColor_clicked();

    void on_buttonDialogOk_clicked();

    void on_buttonDialogCancel_clicked();

private:
    void setDefaultGroupId();
    Ui::AddNodeGroupDialogFrame *ui;

    QColor color;
    MeshModel *m_meshModel;

signals:
    void accept();
    void reject();
};

#endif // ADDNODEGROUPDIALOG_H
