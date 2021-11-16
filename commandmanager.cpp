#include "commandmanager.h"
#include <QDebug>

#define ONLINE_ALL_COMMAND  "00 02 08 04 00 04 00 12 12 00 01"
#define TURN_OFF_AUTO_NOTIFICATION_COMMAND  "00 02 08 04 00 04 00 12 12 00 00"

CommandManager* CommandManager::commandManager=NULL;

CommandManager *CommandManager::getInstance()
{
    if(commandManager==NULL)
    {
        commandManager=new CommandManager;
    }
    return commandManager;
}

QByteArray CommandManager::getTurnOffAutoNotificationOfLuminairesCommand()
{
    return QByteArray::fromHex(TURN_OFF_AUTO_NOTIFICATION_COMMAND);
}

QByteArray CommandManager::getOnlineAllCommand()
{
    return QByteArray::fromHex(ONLINE_ALL_COMMAND);
}

QByteArray CommandManager::getAllOnCommand()
{
    QByteArray data=QByteArray::fromHex("00 02 14 10 00 04 00 12 15 00 00 00 00 00 00 ff ff d0 11 02 01 00 00");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getAllOffCommand()
{
    QByteArray data=QByteArray::fromHex("00 02 14 10 00 04 00 12 15 00 00 00 00 00 00 ff ff d0 11 02 00 00 00");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getGroupAllOnCommand(quint8 groupId)
{
    QByteArray data=QByteArray::fromHex("00 02 14 10 00 04 00 12 15 00 00 00 00 00 00 ff 80 d0 11 02 01 00 00");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    data[15]=(char)groupId;
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getGroupAllOffCommand(quint8 groupId)
{
    QByteArray data=QByteArray::fromHex("00 02 14 10 00 04 00 12 15 00 00 00 00 00 00 ff 80 d0 11 02 00 00 00");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    data[15]=(char)groupId;
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getOnCommand(quint8 addr)
{
    QByteArray data=QByteArray::fromHex("00 02 14 10 00 04 00 12 15 00 00 00 00 00 00 00 00 d0 11 02 01 00 00");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    data[15]=(char)(addr);
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getWarningLightAlarmOnCommand(quint8 addr)
{
    QByteArray data=QByteArray::fromHex("00 02 15 11 00 04 00 12 15 00 00 00 00 00 00 00 00 fd 11 02 01 01 01 00");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);

    data[15]=(char)(addr);
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getWarningLightAlarmOffCommand(quint8 addr)
{
    QByteArray data=QByteArray::fromHex("00 02 15 11 00 04 00 12 15 00 00 00 00 00 00 00 00 fd 11 02 01 01 00 00");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);

    data[15]=(char)(addr);
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getOffCommand(quint8 addr)
{
    QByteArray data=QByteArray::fromHex("00 02 14 10 00 04 00 12 15 00 00 00 00 00 00 00 00 d0 11 02 00 00 00");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    data[15]=(char)(addr);
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getDimmingCommand(quint8 addr, int brightness)
{
    QByteArray data=QByteArray::fromHex("00 02 12 0E 00 04 00 12 15 00 00 00 00 00 00 00 00 d2 11 02 0A");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    data[15]=(char)(addr);
    if(brightness>=99)
    {
        brightness=99;
    }
    data[20]=(char)(brightness);
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getAllDimmingCommand(quint8 brightness)
{
    QByteArray data=QByteArray::fromHex("00 02 12 0E 00 04 00 12 15 00 00 00 00 00 00 FF FF d2 11 02 0A");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    if(brightness>=99)
    {
        brightness=99;
    }
    data[20]=(char)brightness;
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getAddG1Command(quint8 addr, quint8 groupId)
{
    QByteArray data=QByteArray::fromHex("00 02 14 10 00 04 00 12 15 00 00 00 00 00 00 00 00 d7 11 02 01 00 80");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    data[15]=(char)(addr);
    data[21]=(char)(groupId);
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getDeleteG1Command(quint8 addr, quint8 groupId)
{
    QByteArray data=QByteArray::fromHex("00 02 14 10 00 04 00 12 15 00 00 00 00 00 00 00 00 d7 11 02 00 00 80");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    data[15]=(char)(addr);
    data[21]=(char)(groupId);
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getGroupG8Command(quint8 addr)
{
    QByteArray data=QByteArray::fromHex("00 02 13 0F 00 04 00 12 15 00 00 00 00 00 00 00 00 dd 11 02 10 01");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    data[15]=(char)(addr);
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getAllG8Command()
{
    QByteArray data=QByteArray::fromHex("00 02 13 0F 00 04 00 12 15 00 00 00 00 00 00 FF FF dd 11 02 10 01");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getGroupDimmingCommand(quint8 addr, quint8 brightness)
{
    QByteArray data=QByteArray::fromHex("00 02 12 0E 00 04 00 12 15 00 00 00 00 00 00 00 80 d2 11 02 0A");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    data[15]=(char)(addr);
    if(brightness>=99)
    {
        brightness=99;
    }
    data[20]=(char)brightness;
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getEnableCameraCommand(quint8 addr)
{
    //QByteArray data=QByteArray::fromHex("00 02 14 10 00 04 00 12 15 00 00 00 00 00 00 00 00 31 11 02 01 01 00");
    QByteArray data=QByteArray::fromHex("00 02 15 11 00 04 00 12 15 00 63 3a 5b 00 00 00  00 f1 11 02 01 01 01 10");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    data[15]=(char)(addr);
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getDisableCameraCommand(quint8 addr)
{
    QByteArray data=QByteArray::fromHex("00 02 15 11 00 04 00 12 15 00 63 3a 5b 00 00 00  00 f1 11 02 01 02 01 10");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    data[15]=(char)(addr);
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getSetADBoardDisplayModeCommand(quint8 addr, int mode)
{
    QByteArray data=QByteArray::fromHex("00 02 15 11 00 04 00 12 15 00 00 00 00 00 00 00 00 fb 11 02 02 00 00 00");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    data[15]=(char)(addr);
    data[21]=(char)mode;
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getSetGroupADBoardDisplayModeCommand(quint8 groupId, int mode)
{
    QByteArray data=QByteArray::fromHex("00 02 14 10 00 04 00 12 15 00 00 00 00 00 00 00 80 fb 11 02 02 00 00 00");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    data[15]=(char)groupId;
    data[21]=(char)mode;
    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getUVSterilizeCommandAutoCycle(quint8 addr, qint8 nSterizeTime, qint16 nCycleTime)
{
    QByteArray data=QByteArray::fromHex("00 02 15 11 00 04 00 12 15 00 00 00 00 00 00 00 00 f3 11 02 02 01 02 00 00 00 00");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    data[15]=(char)(addr);

    data[20] = static_cast<char>(0x01); //Type: UV Sterilize
    data[21] = static_cast<char>(0x00); //Auto Cycle
    data[22] = static_cast<char>(nCycleTime>>8); //Cycle Time: High Byte
    data[23] = static_cast<char>(nCycleTime&0xff); //Cycle Time: Low Byte
    data[24] = static_cast<char>(nSterizeTime&0xff); //Sterilize Time

    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getDishSellingTableConfigCommand(quint8 addr, qint8 nMaxTemp, qint16 nMaxWaterLevel)
{
    QByteArray data=QByteArray::fromHex("00 02 15 11 00 04 00 12 15 00 00 00 00 00 00 00 00 f3 11 02 02 01 02 00 00 00 00");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    data[15]=(char)(addr);

    data[22] = static_cast<char>(nMaxTemp&0xff); //Max Temperature
    data[23] = static_cast<char>(nMaxWaterLevel>>8); //Max Water Level: High Byte
    data[24] = static_cast<char>(nMaxWaterLevel&0xff); //Max Water Level: Low Byte

    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getUVSterilizeCommandImmediately(quint8 addr, qint8 nSterizeTime)
{
    QByteArray data=QByteArray::fromHex("00 02 15 11 00 04 00 12 15 00 00 00 00 00 00 00 00 f3 11 02 02 01 02 00 00 00 00");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);
    data[15]=(char)(addr);

    data[20] = static_cast<char>(0x01); //Type: UV Sterilize
    data[21] = static_cast<char>(0x01); //Immediate
    data[22] = static_cast<char>(nSterizeTime&0xff); //Sterilize Time

    sequenceNumber++;
    return data;
}

QByteArray CommandManager::getSetSwitchKeyForGroupCommand(quint8 key, quint8 groupAdr)
{
    QByteArray data=QByteArray::fromHex("00 02 12 0e 00 04 00 12 15 00 5b 73 1b 00 00 00 00 f2 11 02 01 00 02 80");
    data[10]=(char)(sequenceNumber);
    data[11]=(char)(sequenceNumber>>8);
    data[12]=(char)(sequenceNumber>>16);

    data[21]=(char)(key&0xff);
    data[22]=(char)(groupAdr&0xff);
    sequenceNumber++;
    return data;
}


CommandManager::CommandManager(QObject *parent) : QObject(parent)
{
    qDebug()<<"CommandManager::CommandManager(QObject *parent)";
    sequenceNumber=1;
}
