#ifndef SETGATEWAYIDDIALOGFRAME_H
#define SETGATEWAYIDDIALOGFRAME_H

#include <QFrame>
#include "model/meshmodel.h"

namespace Ui {
class SetGatewayIdDialogFrame;
}

class SetGatewayIdDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SetGatewayIdDialogFrame(int *pGatewayId, QWidget *parent = 0);
    ~SetGatewayIdDialogFrame();

protected:
    void changeEvent(QEvent *ev);

private slots:

    void on_buttonDialogOk_clicked();

    void on_buttonDialogCancel_clicked();

private:
    Ui::SetGatewayIdDialogFrame *ui;
    int *m_ptrGatewayId;

signals:
    void accept();
    void reject();
};

#endif // SETGATEWAYIDDIALOGFRAME_H
