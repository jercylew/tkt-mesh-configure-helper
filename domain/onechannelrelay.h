#ifndef ONECHANNELRELAY_H
#define ONECHANNELRELAY_H

#include "executer.h"

#define ONE_CHANNEL_RELAY_TYPE_TEXT  "1ch_relay"
#define ONE_CHANNEL_RELAY_SHORT_TYPE_TEXT   "C1R"

class OneChannelRelay : public Executer
{
public:
    OneChannelRelay();

    QString typeText();
    QString displayEnglishTypeText();
    QString shortTypeText();
    static QString staticTypeText();

    void loadInUpdateExecuter(Executer *executer);
    bool isLuminaire() { return false; }

    bool on() const;
    void setOn(bool on);

    QString dataText();

private:
    bool m_on;
};

#endif // ONECHANNELRELAY_H
