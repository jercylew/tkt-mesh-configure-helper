#include "setmeshinfodialogframe.h"
#include "ui_setmeshinfodialogframe.h"
#include <QMessageBox>
#include "dialogbuilder.h"

SetMeshInfoDialogFrame::SetMeshInfoDialogFrame(SwitchMeshInfo *pSwitchMeshInfo, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SetMeshInfoDialogFrame),
    m_ptrSwitchMeshInfo(pSwitchMeshInfo)
{
    ui->setupUi(this);
}

SetMeshInfoDialogFrame::~SetMeshInfoDialogFrame()
{
    delete ui;
}

void SetMeshInfoDialogFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void SetMeshInfoDialogFrame::on_buttonDialogOk_clicked()
{
    if(ui->edtMeshName->text().size() < 1 ||
            ui->edtMeshName->text().size() > 16 ||
            ui->edtMeshPassword->text().size() < 1 ||
            ui->edtMeshPassword->text().size() > 16)
    {
        QMessageBox::critical(this, "设置网关MESH名", "MESH名及其密码长度不能为空且不能超过17");
        return;
    }

    m_ptrSwitchMeshInfo->m_strMeshName = ui->edtMeshName->text();
    m_ptrSwitchMeshInfo->m_strMeshPassword = ui->edtMeshPassword->text();
    emit accept();
}

void SetMeshInfoDialogFrame::on_buttonDialogCancel_clicked()
{
    emit reject();
}
