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

    ui->edtMeshName->setText(m_ptrSwitchMeshInfo->m_strMeshName);
    ui->edtMeshPassword->setText(m_ptrSwitchMeshInfo->m_strMeshPassword);
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
    if(!validateMeshNamePass(ui->edtMeshName->text()) ||
            !validateMeshNamePass(ui->edtMeshPassword->text()))
    {
        QMessageBox::critical(this, "设置网关MESH名",
                              "MESH名及其密码长度不能为空，不能包含空格，特殊字符，且不能超过17个字符");
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

bool SetMeshInfoDialogFrame::validateMeshNamePass(QString strValue)
{
    if (strValue.length() < 4 ||
            strValue.length() >= 16 ||
            strValue.contains(" ") ||
            strValue.contains('\t') ||
            strValue.contains('\n'))
    {
        return false;
    }

    for (int i = 0;i < strValue.length();i++)
    {
        QChar ch = strValue.at(i);
        if (ch.isNonCharacter())
        {
            return false;
        }
    }

    return true;
}
