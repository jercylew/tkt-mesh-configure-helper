#ifndef ALARMLOUDSPEAKER_H
#define ALARMLOUDSPEAKER_H


#include "executer.h"

#define ALARM_LOUDSPEAKER_TYPE_TEXT  "alarm_loudspeaker"
#define ALARM_LOUDSPEAKER_SHORT_TYPE_TEXT   "ASPK"

class AlarmLoudspeaker : public Executer
{
public:
    AlarmLoudspeaker();

    QString typeText();
    QString shortTypeText();
    static QString staticTypeText();
    QString displayEnglishTypeText();

    bool isLuminaire() { return false; }

    void loadInUpdateExecuter(Executer *executer);

    bool on() const;
    void setOn(bool on);

    QString dataText();

private:
    bool m_on;
};

#endif // ALARMLOUDSPEAKER_H
