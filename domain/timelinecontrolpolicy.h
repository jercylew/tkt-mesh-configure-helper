#ifndef TIMELINECONTROLITEMLIST_H
#define TIMELINECONTROLITEMLIST_H

#include <QString>

class TimeLineControlPolicy
{
public:
    TimeLineControlPolicy();

    int id;
    quint8 groupId;
    QString name;
    QString color;
    QString description;
    int subId;

    bool isDefaultPolicy();
};

#endif // TIMELINECONTROLITEMLIST_H
