#include "timelinecontrolpolicy.h"

TimeLineControlPolicy::TimeLineControlPolicy()
{
    id=0;
    groupId=0;
    subId=0;
}

bool TimeLineControlPolicy::isDefaultPolicy()
{
    return (subId==0);
}
