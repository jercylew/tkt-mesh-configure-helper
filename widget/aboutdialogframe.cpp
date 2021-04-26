#include "aboutdialogframe.h"
#include "ui_aboutdialogframe.h"

AboutDialogFrame::AboutDialogFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AboutDialogFrame)
{
    ui->setupUi(this);

    ui->buttonDialogOk->setCursor(Qt::PointingHandCursor);
}

AboutDialogFrame::~AboutDialogFrame()
{
    delete ui;
}

void AboutDialogFrame::setText(const QString &text)
{
    ui->labelDialogAbout->setText(text);
}

void AboutDialogFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void AboutDialogFrame::on_buttonDialogOk_clicked()
{
    emit accept();
}
