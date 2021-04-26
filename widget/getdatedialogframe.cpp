#include "getdatedialogframe.h"
#include "ui_getdatedialogframe.h"

GetDateDialogFrame::GetDateDialogFrame(QDate *date, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::GetDateDialogFrame)
{
    ui->setupUi(this);
    this->date=date;
    ui->calendarWidget->setSelectedDate(*date);

    ui->buttonDialogDone->setCursor(Qt::PointingHandCursor);
    ui->buttonDialogCancel->setCursor(Qt::PointingHandCursor);
}

GetDateDialogFrame::~GetDateDialogFrame()
{
    delete ui;
}

void GetDateDialogFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void GetDateDialogFrame::on_buttonDialogDone_clicked()
{
    QDate selectedDate=ui->calendarWidget->selectedDate();
    date->setDate(selectedDate.year(), selectedDate.month(), selectedDate.day());
    emit accept();
}

void GetDateDialogFrame::on_buttonDialogCancel_clicked()
{
    emit reject();
}
