#include "addcontrolpolicydialogframe.h"
#include "ui_addcontrolpolicydialogframe.h"
#include "dialogbuilder.h"

AddControlPolicyDialogFrame::AddControlPolicyDialogFrame(TimeLineControlPolicy *timeLineControlPolicy, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AddControlPolicyDialogFrame)
{
    ui->setupUi(this);
    this->m_timeLineControlPolicy=timeLineControlPolicy;

    ui->buttonControlPolicyColor->setStyleSheet("border:1px solid #444444;");

    ui->buttonControlPolicyColor->setCursor(Qt::PointingHandCursor);
    ui->buttonDialogOk->setCursor(Qt::PointingHandCursor);
    ui->buttonDialogCancel->setCursor(Qt::PointingHandCursor);
}

AddControlPolicyDialogFrame::~AddControlPolicyDialogFrame()
{
    delete ui;
}

void AddControlPolicyDialogFrame::on_buttonDialogOk_clicked()
{
    m_timeLineControlPolicy->name=ui->lineControlPolicyName->text();
    m_timeLineControlPolicy->color=m_color.name();
    emit accept();
}

void AddControlPolicyDialogFrame::on_buttonDialogCancel_clicked()
{
    emit reject();
}

void AddControlPolicyDialogFrame::on_buttonControlPolicyColor_clicked()
{
    bool ok=false;
    QColor c=DialogBuilder::getInstance()->runGetColorDialog("Please Select Color", m_color, &ok);
    if(ok)
    {
        m_color=c;
        ui->buttonControlPolicyColor->setStyleSheet(QString("border:none; background-color:%1;").arg(m_color.name()));
    }
}
