#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QObject>
#include "mesh_define.h"


class CommandManager : public QObject
{
    Q_OBJECT
public:
    static CommandManager *getInstance();

    QByteArray getTurnOffAutoNotificationOfLuminairesCommand();
    QByteArray getOnlineAllCommand();
    QByteArray getAllOnCommand();
    QByteArray getAllOffCommand();
    QByteArray getGroupAllOnCommand(quint8 groupId);
    QByteArray getGroupAllOffCommand(quint8 groupId);
    QByteArray getOnCommand(quint8 addr);
    QByteArray getOffCommand(quint8 addr);
    QByteArray getWarningLightAlarmOnCommand(quint8 addr);
    QByteArray getWarningLightAlarmOffCommand(quint8 addr);
    QByteArray getUVSterilizeCommandAutoCycle(quint8 addr, qint8 nSterizeTime, qint16 nCycleTime);
    QByteArray getDishSellingTableConfigCommand(quint8 addr, qint8 nMaxTemp, qint16 nMaxWaterLevel);
    QByteArray getUVSterilizeCommandImmediately(quint8 addr, qint8 nSterizeTime);
    QByteArray getDimmingCommand(quint8 addr, int brightness);
    QByteArray getAllDimmingCommand(quint8 brightness);
    QByteArray getAddG1Command(quint8 addr, quint8 groupId);
    QByteArray getDeleteG1Command(quint8 addr, quint8 groupId);
    QByteArray getGroupG8Command(quint8 addr);
    QByteArray getAllG8Command();
    QByteArray getGroupDimmingCommand(quint8 addr, quint8 brightness);
    QByteArray getEnableCameraCommand(quint8 addr);
    QByteArray getDisableCameraCommand(quint8 addr);
    QByteArray getSetADBoardDisplayModeCommand(quint8 addr, int mode);
    QByteArray getSetGroupADBoardDisplayModeCommand(quint8 addr, int mode);
    QByteArray getSetSwitchKeyForGroupCommand(quint8 key, quint8 groupAdr);

private:
    explicit CommandManager(QObject *parent = 0);
    static CommandManager *commandManager;

    quint64 sequenceNumber;

signals:

public slots:
};

#endif // COMMANDMANAGER_H
