#include "setgatewayiddialogframe.h"
#include "ui_setgatewayiddialogframe.h"
#include <QMessageBox>
#include <QIntValidator>
#include "dialogbuilder.h"

SetGatewayIdDialogFrame::SetGatewayIdDialogFrame(int *pGatewayId, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SetGatewayIdDialogFrame),
    m_ptrGatewayId(pGatewayId)
{
    ui->setupUi(this);

    QValidator *validator = new QIntValidator(0, 255, this);
    ui->edtGatewayId->setValidator(validator);
}

SetGatewayIdDialogFrame::~SetGatewayIdDialogFrame()
{
    delete ui;
}

void SetGatewayIdDialogFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void SetGatewayIdDialogFrame::on_buttonDialogOk_clicked()
{
    if(ui->edtGatewayId->text().isEmpty())
    {
        QMessageBox::critical(this, "设置网关ID", "网关ID不能为空");
        return;
    }

    *m_ptrGatewayId = ui->edtGatewayId->text().toInt();

    if (*m_ptrGatewayId > 255 || *m_ptrGatewayId < 0)
    {
        QMessageBox::critical(this, "设置网关ID", "网关ID必须位于0 ~ 255之间");
        return;
    }

    emit accept();
}

void SetGatewayIdDialogFrame::on_buttonDialogCancel_clicked()
{
    emit reject();
}
