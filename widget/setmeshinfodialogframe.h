#ifndef SETMESHINFODIALOGFRAME_H
#define SETMESHINFODIALOGFRAME_H

#include <QFrame>
#include "model/meshmodel.h"

namespace Ui {
class SetMeshInfoDialogFrame;
}

class SetMeshInfoDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SetMeshInfoDialogFrame(SwitchMeshInfo *pSwitchMeshInfo, QWidget *parent = 0);
    ~SetMeshInfoDialogFrame();

protected:
    void changeEvent(QEvent *ev);

private slots:

    void on_buttonDialogOk_clicked();

    void on_buttonDialogCancel_clicked();

private:
    bool validateMeshNamePass(QString strValue);

    Ui::SetMeshInfoDialogFrame *ui;
    SwitchMeshInfo *m_ptrSwitchMeshInfo;

signals:
    void accept();
    void reject();
};

#endif // SETMESHINFODIALOGFRAME_H
