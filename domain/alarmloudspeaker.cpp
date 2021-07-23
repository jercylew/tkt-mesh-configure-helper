#include "alarmloudspeaker.h"

AlarmLoudspeaker::AlarmLoudspeaker()
{
    m_on = false;
}

QString AlarmLoudspeaker::typeText()
{
    return ALARM_LOUDSPEAKER_TYPE_TEXT;
}

QString AlarmLoudspeaker::shortTypeText()
{
    return ALARM_LOUDSPEAKER_SHORT_TYPE_TEXT;
}

QString AlarmLoudspeaker::staticTypeText()
{
    return ALARM_LOUDSPEAKER_TYPE_TEXT;
}

void AlarmLoudspeaker::loadInUpdateExecuter(Executer *executer)
{
    Executer::loadInUpdateExecuter(executer);
    if(executer->typeText() == AlarmLoudspeaker::staticTypeText())
    {
        AlarmLoudspeaker *alarmSpeaker = static_cast<AlarmLoudspeaker*>(executer);
        if(alarmSpeaker)
        {
            this->setOn(alarmSpeaker->on());
        }
    }
}

bool AlarmLoudspeaker::on() const
{
    return m_on;
}

void AlarmLoudspeaker::setOn(bool on)
{
    m_on = on;
}

QString AlarmLoudspeaker::displayEnglishTypeText()
{
    return "Alarm Loudspeaker";
}

QString AlarmLoudspeaker::dataText()
{
    QString onText = QObject::tr("ON");
    QString offText = QObject::tr("OFF");
    return QObject::tr("STATUS: %1").arg(on()?onText:offText);
}
