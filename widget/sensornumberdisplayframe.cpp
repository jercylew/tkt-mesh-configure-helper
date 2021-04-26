#include "sensornumberdisplayframe.h"
#include "ui_sensornumberdisplayframe.h"
#include <QPropertyAnimation>

SensorNumberDisplayFrame::SensorNumberDisplayFrame(const QString &typeText, const QString &m_sensorNotTranslatedText, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SensorNumberDisplayFrame)
{
    ui->setupUi(this);
    this->m_typeText=typeText;
    this->m_sensorNotTranslatedText=m_sensorNotTranslatedText;
    ui->labelSensorNumberText->setText(tr(m_sensorNotTranslatedText.toLatin1().data()));
    ui->labelSensorNumberIcon->setProperty("type", this->m_typeText);
    ui->labelSensorNumberText->setMaximumWidth(0);

    m_propertyAnimation=NULL;
}

SensorNumberDisplayFrame::~SensorNumberDisplayFrame()
{
    delete ui;
}

void SensorNumberDisplayFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->labelSensorNumberText->setText(tr(m_sensorNotTranslatedText.toLatin1().data()));
    }
}

void SensorNumberDisplayFrame::setNumber(int number)
{
    ui->labelSensorNumberNumber->setText(QString::number(number));
}

void SensorNumberDisplayFrame::enterEvent(QEvent *event)
{
    animatedShowText();
}

void SensorNumberDisplayFrame::leaveEvent(QEvent *event)
{
    animatedHideText();
}

void SensorNumberDisplayFrame::animatedShowText()
{
    if(m_propertyAnimation!=NULL)
    {
        m_propertyAnimation->stop();
        delete m_propertyAnimation;
        m_propertyAnimation=NULL;
    }
    m_propertyAnimation=new QPropertyAnimation(ui->labelSensorNumberText, "maximumWidth");
    m_propertyAnimation->setDuration(500);
    m_propertyAnimation->setStartValue(0);
    m_propertyAnimation->setEndValue(300);
    m_propertyAnimation->start();
}

void SensorNumberDisplayFrame::animatedHideText()
{
    if(m_propertyAnimation!=NULL)
    {
        m_propertyAnimation->stop();
        delete m_propertyAnimation;
        m_propertyAnimation=NULL;
    }
    ui->labelSensorNumberText->setMaximumWidth(0);
}
