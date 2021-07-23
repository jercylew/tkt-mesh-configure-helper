#include "alarmloudspeakerframe.h"
#include "ui_alarmloudspeakerframe.h"

AlarmLoudspeakerFrame::AlarmLoudspeakerFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AlarmLoudspeakerFrame)
{
    ui->setupUi(this);
    groupLabelList.append(ui->labelGroup1);
    groupLabelList.append(ui->labelGroup2);
    groupLabelList.append(ui->labelGroup3);
    groupLabelList.append(ui->labelGroup4);
    groupLabelList.append(ui->labelGroup5);
    groupLabelList.append(ui->labelGroup6);
    groupLabelList.append(ui->labelGroup7);
    groupLabelList.append(ui->labelGroup8);

    ui->labelGroup5->setVisible(false);
    ui->labelGroup6->setVisible(false);
    ui->labelGroup7->setVisible(false);
    ui->labelGroup8->setVisible(false);

    this->setMaximumSize(110, 140);
    this->setMinimumSize(110, 140);
}

AlarmLoudspeakerFrame::~AlarmLoudspeakerFrame()
{
    delete ui;
}

void AlarmLoudspeakerFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

QLabel *AlarmLoudspeakerFrame::getIconLabel()
{
    return ui->labelIcon;
}

void AlarmLoudspeakerFrame::setName(const QString &name)
{
    ui->labelName->setText(name);
}

void AlarmLoudspeakerFrame::setGroupColors(const QList<QColor> &groupColorList)
{
    for(int i=0; i<groupLabelList.size(); i++)
    {
        QLabel *label=groupLabelList.at(i);
        if(i<groupColorList.size())
        {
            label->setStyleSheet(QString("border:none;border-radius:8px;background-color:%1;").arg(groupColorList.at(i).name()));
        }
        else
        {
            label->setStyleSheet("border:1px solid #888888;border-radius:8px;background-color:transparent;");
        }
    }
}

void AlarmLoudspeakerFrame::resetStyle()
{
    for(int i=0; i<groupLabelList.size(); i++)
    {
        groupLabelList.at(i)->setStyle(groupLabelList.at(i)->style());
    }
    ui->labelIcon->setStyle(ui->labelIcon->style());
}

