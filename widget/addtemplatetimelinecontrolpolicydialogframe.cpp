#include "addtemplatetimelinecontrolpolicydialogframe.h"
#include "ui_addtemplatetimelinecontrolpolicydialogframe.h"

AddTemplateTimeLineControlPolicyDialogFrame::AddTemplateTimeLineControlPolicyDialogFrame(QString *name, QString *description, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AddTemplateTimeLineControlPolicyDialogFrame)
{
    ui->setupUi(this);
    this->name=name;
    this->description=description;
    ui->lineName->setText(*name);

    ui->buttonOk->setCursor(Qt::PointingHandCursor);
    ui->buttonCancel->setCursor(Qt::PointingHandCursor);
}

AddTemplateTimeLineControlPolicyDialogFrame::~AddTemplateTimeLineControlPolicyDialogFrame()
{
    delete ui;
}

void AddTemplateTimeLineControlPolicyDialogFrame::on_buttonOk_clicked()
{
    *name=ui->lineName->text();
    emit accept();
}

void AddTemplateTimeLineControlPolicyDialogFrame::on_buttonCancel_clicked()
{
    emit reject();
}
