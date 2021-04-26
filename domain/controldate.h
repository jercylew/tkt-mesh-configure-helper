#ifndef CONTROLDATE_H
#define CONTROLDATE_H

#include <QString>

class ControlDate
{
public:
    ControlDate();

    QString id;
    int groupId;
    int month;
    int day;
    int controlPolicyId;
};

#endif // CONTROLDATE_H
