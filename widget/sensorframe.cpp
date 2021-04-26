#include "sensorframe.h"
#include "ui_sensorframe.h"

SensorFrame::SensorFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SensorFrame)
{
    ui->setupUi(this);
}

SensorFrame::~SensorFrame()
{
    delete ui;
}

void SensorFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

QLabel *SensorFrame::getIconLabel()
{
    return ui->labelIcon;
}

void SensorFrame::setName(const QString &name)
{
    ui->labelName->setText(name);
}

void SensorFrame::setId(const QString &sensorId)
{
    ui->labelAddr->setText(QString("[%1]").arg(sensorId));
}

void SensorFrame::setStateText(const QString &stateText)
{
    ui->labelState->setText(stateText);
}

void SensorFrame::resetStyle()
{
    ui->labelIcon->setStyle(ui->labelIcon->style());
}
