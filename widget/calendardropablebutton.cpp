#include "calendardropablebutton.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMessageBox>
#include <QMimeData>
#include <QDebug>
#include <QHBoxLayout>

CalendarDropableButton::CalendarDropableButton(QWidget *parent) :
    QFrame(parent)
{
    this->setAcceptDrops(true);
    QHBoxLayout *layout=new QHBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2);
    this->setLayout(layout);
    m_button=new QPushButton(this);
    m_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(m_button);
    setSelected(false);
    setColor("#888888");
    setInMonth(true);
    setMonthInDetailsMode(false);
    connect(m_button, SIGNAL(clicked()), this, SIGNAL(clicked()));
    connect(m_button, SIGNAL(pressed()), this, SIGNAL(pressed()));
    m_isDutyMode=false;
}

CalendarDropableButton::CalendarDropableButton(const QDate &date, QWidget *parent)
    : CalendarDropableButton(parent)
{
    setDate(date);
}

CalendarDropableButton::CalendarDropableButton(const QString &dutyTypeText, const QString &dutyDisplayText, QWidget *parent)
    : CalendarDropableButton(parent)
{
    this->m_dutyTypeText=dutyTypeText;
    m_button->setText(dutyDisplayText);
    m_isDutyMode=true;
}

bool CalendarDropableButton::selected()
{
    return m_selected;
}

void CalendarDropableButton::setSelected(bool selected)
{
    if(this->m_selected!=selected)
    {
        this->m_selected=selected;
        resetStyle();
    }
}

QString CalendarDropableButton::color()
{
    return this->m_color;
}

void CalendarDropableButton::setColor(const QString &color)
{
    if(this->m_color!=color)
    {
        this->m_color=color;
        resetStyle();
    }
}

bool CalendarDropableButton::inMonth()
{
    return this->m_inMonth;
}

void CalendarDropableButton::setInMonth(bool inMonth)
{
    if(m_inMonth!=inMonth)
    {
        m_inMonth=inMonth;
        resetStyle();
    }
}

void CalendarDropableButton::setDate(const QDate &date)
{
    this->m_date=date;
    this->m_button->setText(QString::number(date.day()));
}

QDate CalendarDropableButton::date()
{
    return this->m_date;
}

void CalendarDropableButton::setMonthInDetailsMode(bool monthInDetailsMode)
{
    this->m_monthInDetailsMode=monthInDetailsMode;
}

bool CalendarDropableButton::monthInDetailsMode()
{
    return this->m_monthInDetailsMode;
}

bool CalendarDropableButton::isDutyMode()
{
    return this->m_isDutyMode;
}

QString CalendarDropableButton::dutyTypeText()
{
    return this->m_dutyTypeText;
}

void CalendarDropableButton::setText(const QString &text)
{
    m_button->setText(text);
}

void CalendarDropableButton::dragEnterEvent(QDragEnterEvent *ev)
{
    if(this->selected() || this->isDutyMode())
    {
        ev->acceptProposedAction();
    }
    else
    {
        ev->ignore();
    }
}

void CalendarDropableButton::dropEvent(QDropEvent *ev)
{
    if(((!this->selected()) && (!isDutyMode())))
    {
        return;
    }
    QByteArray encodedata = ev->mimeData()->data("application/taikwan-meshlight-item-policy-id");
    if(!encodedata.isEmpty())
    {
        int controlPolicyId=encodedata.toInt();
        if(this->selected() || isDutyMode())
        {
            emit controlPolicyDroped(controlPolicyId);
        }
    }
}

void CalendarDropableButton::resetStyle()
{
    if(inMonth() || isDutyMode())
    {
        if(m_selected)
        {
            this->setStyleSheet("background-color:#0000AA");
        }
        else
        {
            this->setStyleSheet("background-color:transparent;");
        }
        if(m_monthInDetailsMode)
        {
            m_button->setStyleSheet(QString("border:none; border-radius:4px; background-color:%1; font-size:18px; font-weight:bold; color:#000000;").arg(this->m_color));
        }
        else
        {
            m_button->setStyleSheet(QString("border:none; border-radius:4px; background-color:%1; font-size:12px; font-weight:bold; color:#000000;").arg(this->m_color));
        }
    }
    else
    {
        this->setStyleSheet("background-color:transparent;");
        if(m_monthInDetailsMode)
        {
            m_button->setStyleSheet(QString("border:none; border-radius:4px; background-color:#DDDDDD; font-size:18px; font-weight: bold; color: #CCCCCC;"));
        }
        else
        {
            m_button->setStyleSheet(QString("border:none; border-radius:4px; background-color:#DDDDDD; font-size:12px; font-weight: bold; color: #CCCCCC;"));
        }
    }
}



