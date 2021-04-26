#ifndef TRANSLATEHELPER_H
#define TRANSLATEHELPER_H

#include <QObject>
#include "domain/timelinecontrolpolicy.h"
#include <QMap>

class TranslateHelper : public QObject
{
    Q_OBJECT
public:
    explicit TranslateHelper(QObject *parent = 0);

signals:

public slots:
};

#endif // TRANSLATEHELPER_H
