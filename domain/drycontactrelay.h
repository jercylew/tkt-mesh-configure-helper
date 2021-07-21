#ifndef DRYCONTACTRELAY_H
#define DRYCONTACTRELAY_H

#include "executer.h"

#define DRY_CONTACT_RELAY_TYPE_TEXT  "dry_contact_relay"
#define DRY_CONTACT_RELAY_SHORT_TYPE_TEXT   "DCTR"

class DryContactRelay : public Executer
{
public:
    DryContactRelay();

    QString typeText();
    QString shortTypeText();
    static QString staticTypeText();

    void loadInUpdateExecuter(Executer *executer);

    bool on() const;
    void setOn(bool on);

private:
    bool m_on;
};

#endif // DRYCONTACTRELAY_H
