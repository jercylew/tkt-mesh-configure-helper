#ifndef PARSERMANAGER_H
#define PARSERMANAGER_H

#include <QObject>
#include <QMutex>
#include <QMap>
#include "domain/sensor.h"

class ExecuterParser;
class SensorParser;
class Parser;

class ParserManager : public QObject
{
    Q_OBJECT
public:
    static ParserManager *instance();

    ExecuterParser *getExecuterParserByTypeText(const QString &typeText);
    ExecuterParser *getExecuterParserByTypeCode(quint8 executerTypeCode);

    SensorParser *getSensorParserByTypeText(const QString &typeText);
    SensorParser *getSensorParserByType(Sensor::SensorType sensorType);
    SensorParser *getSensorParserBySensorCode(quint8 sensorCode);

private:
    explicit ParserManager(QObject *parent = 0);
    static ParserManager *m_parserManager;
    static QMutex m_parserManagerMutex;

    QMap<QString, SensorParser*> m_sensorTypeTextToParserMap;
    QMap<Sensor::SensorType, SensorParser*> m_sensorTypeToParserMap;
    QMap<quint8, SensorParser*> m_sensorCodeToParserMap;
    QMap<QString, ExecuterParser*> m_executerTypeTextToParserMap;
    QMap<quint8, ExecuterParser*> m_executerTypeCodeToParserMap;

signals:

public slots:
};

#endif // PARSERMANAGER_H
