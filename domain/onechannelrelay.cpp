#include "onechannelrelay.h"

OneChannelRelay::OneChannelRelay()
{
    m_on=false;
}

QString OneChannelRelay::typeText()
{
    return ONE_CHANNEL_RELAY_TYPE_TEXT;
}

QString OneChannelRelay::displayEnglishTypeText()
{
    return "One-Channel Relay";
}

QString OneChannelRelay::shortTypeText()
{
    return ONE_CHANNEL_RELAY_SHORT_TYPE_TEXT;
}

QString OneChannelRelay::staticTypeText()
{
    return ONE_CHANNEL_RELAY_TYPE_TEXT;
}

void OneChannelRelay::loadInUpdateExecuter(Executer *executer)
{
    Executer::loadInUpdateExecuter(executer);
    if(executer->typeText()==OneChannelRelay::staticTypeText())
    {
        OneChannelRelay *relay=static_cast<OneChannelRelay*>(executer);
        if(relay)
        {
            this->setOn(relay->on());
        }
    }
}

bool OneChannelRelay::on() const
{
    return m_on;
}

void OneChannelRelay::setOn(bool on)
{
    m_on = on;
}

QString OneChannelRelay::dataText()
{
    QString onText = QObject::tr("ON");
    QString offText = QObject::tr("OFF");
    return QObject::tr("STATUS: %1").arg(on()?onText:offText);
}
