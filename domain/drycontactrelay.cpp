#include "drycontactrelay.h"

DryContactRelay::DryContactRelay()
{
    m_on = false;
}

QString DryContactRelay::typeText()
{
    return DRY_CONTACT_RELAY_TYPE_TEXT;
}

QString DryContactRelay::shortTypeText()
{
    return DRY_CONTACT_RELAY_SHORT_TYPE_TEXT;
}

QString DryContactRelay::staticTypeText()
{
    return DRY_CONTACT_RELAY_TYPE_TEXT;
}

QString DryContactRelay::displayEnglishTypeText()
{
    return "Dry Contact Relay";
}

void DryContactRelay::loadInUpdateExecuter(Executer *executer)
{
    Executer::loadInUpdateExecuter(executer);
    if(executer->typeText()==DryContactRelay::staticTypeText())
    {
        DryContactRelay *relay=static_cast<DryContactRelay*>(executer);
        if(relay)
        {
            this->setOn(relay->on());
        }
    }
}

bool DryContactRelay::on() const
{
    return m_on;
}

void DryContactRelay::setOn(bool on)
{
    m_on = on;
}

QString DryContactRelay::dataText()
{
    QString onText = QObject::tr("ON");
    QString offText = QObject::tr("OFF");
    return QObject::tr("STATUS: %1").arg(on()?onText:offText);
}
