#include "executerframe.h"
#include "ui_executerframe.h"
#include <QPainter>

ExecuterFrame::ExecuterFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ExecuterFrame)
{
    ui->setupUi(this);
    ui->labelIcon->setAlignment(Qt::AlignCenter);
    ui->labelIcon->setStyleSheet("border:2px solid #DDDDDD; border-radius:10px; color:#0000FF; font-size:30px;");
}

ExecuterFrame::~ExecuterFrame()
{
    delete ui;
}

QLabel *ExecuterFrame::getIconLabel()
{
    return ui->labelIcon;
}

QLabel *ExecuterFrame::getTextLabel()
{
    return ui->labelText;
}

