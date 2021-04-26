#ifndef INITDBSQL_H
#define INITDBSQL_H

#include <QString>

class InitDbSql
{
public:
    InitDbSql();
    bool execWhenLastQueryRecordNumberIsZero;
    QString sqlName;
    QString sqlText;
};

#endif // INITDBSQL_H
