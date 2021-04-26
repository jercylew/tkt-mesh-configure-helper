#include "questiondialogframe.h"
#include "ui_questiondialogframe.h"

QuestionDialogFrame::QuestionDialogFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::QuestionDialogFrame)
{
    ui->setupUi(this);

    ui->buttonDialogOk->setCursor(Qt::PointingHandCursor);
    ui->buttonDialogCancel->setCursor(Qt::PointingHandCursor);
}

QuestionDialogFrame::~QuestionDialogFrame()
{
    delete ui;
}

void QuestionDialogFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void QuestionDialogFrame::setQuestion(const QString &question)
{
    ui->labelDialogQuestion->setText(question);
}

void QuestionDialogFrame::on_buttonDialogOk_clicked()
{
    emit accept();
}

void QuestionDialogFrame::on_buttonDialogCancel_clicked()
{
    emit reject();
}
