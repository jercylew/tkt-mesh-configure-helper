#include "inputdialogframe.h"
#include "ui_inputdialogframe.h"

InputDialogFrame::InputDialogFrame(QString *text, const QString &inputTitle, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::InputDialogFrame)
{
    ui->setupUi(this);
    this->text=text;
    ui->lineInputEdit->setText(*text);
    ui->labelInputTitle->setText(inputTitle);

    ui->buttonDialogDone->setCursor(Qt::PointingHandCursor);
    ui->buttonDialogCancel->setCursor(Qt::PointingHandCursor);
}

InputDialogFrame::~InputDialogFrame()
{
    delete ui;
}

void InputDialogFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void InputDialogFrame::on_buttonDialogDone_clicked()
{
    *text=ui->lineInputEdit->text();
    emit accept();
}

void InputDialogFrame::on_buttonDialogCancel_clicked()
{
    emit reject();
}
