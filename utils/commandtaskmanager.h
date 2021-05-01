#ifndef COMMANDTASKMANAGER_H
#define COMMANDTASKMANAGER_H

#include <QObject>
#include <QTimer>
#include <QMap>

class MeshModel;

class ControlCommand{
public:
    ControlCommand(QByteArray command, bool isGroup, const QString &commandTypeText, int calculateNumber=0);

    QByteArray command();
    void setCommand(const QByteArray &command);
    bool isGroup();
    void setIsGroup(bool isGroup);
    quint8 groupId();
    void setGroupId(quint8 groupId);
    bool highPriority();
    void setHighPriority(bool highPriority);
    bool isValid();
    void setIsValid(bool isValid);
    int calculateNumber();
    void setCommandTypeText(const QString &commandTypeText);
    QString commandTypeText();
    void setLogExtraText(const QString &logExtraText);
    QString logExtraText();

    QString toString();

private:
    QByteArray m_command; //命令内容
    bool m_isGroup; //group or others
    quint8 m_groupId;  //groupId
    bool m_highPriority;  //是否具有高优先级
    bool m_isValid;
    int m_calculateNumber; //第几次计算
    QString m_commandTypeText;  //命令类型
    QString m_logExtraText; //日志额外信息
};

class CommandTaskManager : public QObject
{
    Q_OBJECT
public:
    explicit CommandTaskManager(MeshModel *meshModel, QObject *parent = 0);

    void runTurnOffAutoNotificationOfLuminairesCommand();

    void runOnlineAllCommand();
    void runGetAllG8Command();
    void runAddG1Command(QList<quint16> addrList, quint8 groupId);
    void runDeleteG1Command(QList<quint16> addrList, quint8 groupId);

    void runAllOnCommand();
    void runAllOffCommand();
    void runGroupAllOnCommand(quint8 groupId, bool highPriority=false, int calculateNumber=0);
    void runGroupAllOffCommand(quint8 groupId, bool highPriority=false, int calculateNumber=0);
    void runOnCommand(QList<quint16> addrList);
    void runOffCommand(QList<quint16> addrList);
    void runDimmingCommand(QList<quint16> addrList, int brightness);
    void runAllDimmingCommand(quint8 brightness);
    void runGroupDimmingCommand(quint8 groupId, quint8 brightness, bool hightPriority=false, int calculateNumber=0);
    void runFindLumCommand(quint8 addr);
    void runEnableCameraAP(quint8 addr);
    void runDisableCameraAP(quint8 addr);
    void runSetADBoardDisplayMode(quint8 addr, int mode);
    void runSetGroupADBoardDisplayMode(quint8 groupId, int mode, bool hightPriority=false, int calculateNumber=0);
    void runSwitchKeyGroupSetup(quint8 key, quint8 group, bool hightPriority = false, int calculateNumber = 0);
    void runUVSterilizingCommand(QList<quint16> addrList, qint8 nSterilizeTime, qint16 nCycleTime, bool bAutoCycle);

    int bufferedCommandNumber();
    void clearAllBufferedCommand();

    bool hasTooManyBufferedCommand();

private:
    bool taskBusy;
    QList<ControlCommand*> commandList;  //普通的控制命令
    QList<ControlCommand*> highestPriorityCommandList;  //所有配置命令和获取配置的命令

    void runHighestPriorityCommand(ControlCommand *command);
    void runHighestPriorityCommands(const QList<ControlCommand*> &commandList);
    void runCommand(ControlCommand *command, bool hightPriority=false);
    void runCommands(const QList<ControlCommand*> &commandList, bool hightPriority=false);
    QTimer tickTimer;
    QTimer findLumTimer;
    MeshModel *m_meshModel;

    quint8 m_findLumAddr;
    int m_findLumCurrentNumber;

    void optimizateCommandBuffer(quint8 groupId, int calculateNumber);

    int getRange0To100Brightness(int brightness);
    void keepTickTimerStart();

    quint64 m_tickTimerStopMSec;

signals:
    void taskManagerIsBusy();

private slots:
    void doTickTimerout();
    void doFindLumTimerout();
};

#endif // COMMANDTASKMANAGER_H
