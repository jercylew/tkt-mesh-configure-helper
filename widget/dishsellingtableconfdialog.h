#ifndef DISHSELLINGTABLECONFDIALOG_H
#define DISHSELLINGTABLECONFDIALOG_H

#include <QDialog>

namespace Ui {
class DishSellingTableConfDialog;
}

class MeshModel;

class DishSellingTableConfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DishSellingTableConfDialog(MeshModel *meshModel, QList<quint16> addrList,
                                    QWidget *parent = nullptr);
    explicit DishSellingTableConfDialog(MeshModel *meshModel, quint8 groupId,
                                    QWidget *parent = nullptr);
    ~DishSellingTableConfDialog();

protected:
    void changeEvent(QEvent *ev);

private slots:
    void on_buttonDialogClose_clicked();
    void on_buttonDialogBottomClose_clicked();
    void on_buttonDialogApply_clicked();

private:
    Ui::DishSellingTableConfDialog *ui;
    QList<quint16> addrList;
    bool isGroupMode;
    quint8 groupId;
    MeshModel *m_meshModel;
};

#endif // LUMINAIREDIMMINGDIALOG_H
