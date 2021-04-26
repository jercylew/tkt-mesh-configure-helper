#include "addtimepointdialogframe.h"
#include "ui_addtimepointdialogframe.h"
#include "utils/timelinecontrolmanager.h"
#include "model/meshmodel.h"

AddTimePointDialogFrame::AddTimePointDialogFrame(int *minutes, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AddTimePointDialogFrame)
{
    ui->setupUi(this);
    this->m_minutes=minutes;

    ui->buttonOk->setCursor(Qt::PointingHandCursor);
    ui->buttonCancel->setCursor(Qt::PointingHandCursor);
}

AddTimePointDialogFrame::~AddTimePointDialogFrame()
{
    delete ui;
}

void AddTimePointDialogFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

int AddTimePointDialogFrame::getMinutes()
{
    return ui->spinHour->value()*60+ui->spinMinute->value();
}

void AddTimePointDialogFrame::on_buttonOk_clicked()
{
    *m_minutes=getMinutes();
    emit accept();
}

void AddTimePointDialogFrame::on_buttonCancel_clicked()
{
    emit reject();
}
