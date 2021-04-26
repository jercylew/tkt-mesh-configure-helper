#include "nodegroupframe.h"
#include "ui_nodegroupframe.h"

#include <QPixmap>
#include <QBitmap>

NodeGroupFrame::NodeGroupFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::NodeGroupFrame)
{
    ui->setupUi(this);
    QPixmap pixmap(":/images/group_mask.png");
    ui->labelGroupHead->setMask(pixmap.mask());
    setScheduleOn(true);
}

NodeGroupFrame::~NodeGroupFrame()
{
    delete ui;
}

void NodeGroupFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void NodeGroupFrame::setGroupColor(const QColor &color)
{
    ui->labelGroupHead->setStyleSheet(QString("background-color:%1;").arg(color.name()));
}

void NodeGroupFrame::setGroupName(const QString &name)
{
    ui->labelGroupName->setText(name);
}

void NodeGroupFrame::setNodeNumber(int nodeNumber)
{
    ui->labelNodeNumber->setText(QString::number(nodeNumber));
}

void NodeGroupFrame::setScheduleOn(bool on)
{
    if(on)
    {
        ui->line->setProperty("schedule", "on");
    }
    else
    {
        ui->line->setProperty("schedule", "off");
    }
    ui->line->setStyle(ui->line->style());
}

void NodeGroupFrame::setGroupType(const QString &groupType)
{
    ui->labelGroupType->setText(tr(groupType.toLatin1().data()));
}
