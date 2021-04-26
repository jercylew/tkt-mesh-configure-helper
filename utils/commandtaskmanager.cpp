#include "commandtaskmanager.h"
#include "commandmanager.h"
#include <QDebug>
#include "model/meshmodel.h"
#include "commandmanager.h"
#include "utils/meshsavingbox.h"
#include "utils/meshlogmanager.h"
#include <QDateTime>
#include "utils/remoteconfigsessionholder.h"
#include "utils/remoteconfigmanager.h"
#include "model/preparemeshmodel.h"

/*********** 配置和获取配置的指令，它们具有最高级别的优先级 ********/
void CommandTaskManager::runOnlineAllCommand()
{
    runHighestPriorityCommand(new ControlCommand(CommandManager::getInstance()->getOnlineAllCommand(), false, "OnlineAll"));
}

void CommandTaskManager::runGetAllG8Command()
{
    runHighestPriorityCommand(new ControlCommand(CommandManager::getInstance()->getAllG8Command(), false, "GetAllG8"));
}

void CommandTaskManager::runAddG1Command(QList<quint16> addrList, quint8 groupId)
{
    QList<ControlCommand*> tmpList;
    for(quint16 addr : addrList)
    {
        ControlCommand *command=new ControlCommand(CommandManager::getInstance()->getAddG1Command(addr, groupId), false, "AddG1");
        command->setLogExtraText(QString("Addr=%1, GroupId=%2").arg(addr&0xFF).arg(groupId&0xFF));
        tmpList.push_back(command);
    }
    runHighestPriorityCommands(tmpList);
}

void CommandTaskManager::runDeleteG1Command(QList<quint16> addrList, quint8 groupId)
{
    QList<ControlCommand*> tmpList;
    for(quint16 addr : addrList)
    {
        ControlCommand *command=new ControlCommand(CommandManager::getInstance()->getDeleteG1Command(addr, groupId), false, "DeleteG1");
        command->setLogExtraText(QString("Addr=%1, GroupId=%2").arg(addr&0xFF).arg(groupId&0xFF));
        tmpList.push_back(command);
    }
    runHighestPriorityCommands(tmpList);
}

/******************* 控制指令，分为高优先级指令和普通指令 *******************/
void CommandTaskManager::runAllOnCommand()
{
    runCommand(new ControlCommand(CommandManager::getInstance()->getAllOnCommand(), false, "AllOn"));
}

void CommandTaskManager::runAllOffCommand()
{
    runCommand(new ControlCommand(CommandManager::getInstance()->getAllOffCommand(), false, "AllOff"));
}

void CommandTaskManager::runGroupAllOnCommand(quint8 groupId, bool highPriority, int calculateNumber)
{
    if(highPriority)
    {
        optimizateCommandBuffer(groupId, calculateNumber);
    }
    ControlCommand *command=new ControlCommand(CommandManager::getInstance()->getGroupAllOnCommand(groupId), true, "GroupOn", calculateNumber);
    command->setHighPriority(highPriority);
    command->setGroupId(groupId);
    command->setLogExtraText(QString("GroupId=%1").arg(groupId&0xFF));
    runCommand(command, highPriority);
}

void CommandTaskManager::runGroupAllOffCommand(quint8 groupId, bool highPriority, int calculateNumber)
{
    if(highPriority)
    {
        optimizateCommandBuffer(groupId, calculateNumber);
    }
    ControlCommand *command=new ControlCommand(CommandManager::getInstance()->getGroupAllOffCommand(groupId), true, "GroupOff", calculateNumber);
    command->setHighPriority(highPriority);
    command->setGroupId(groupId);
    command->setLogExtraText(QString("GroupId=%1").arg(groupId&0xFF));
    runCommand(command, highPriority);
}

void CommandTaskManager::runOnCommand(QList<quint16> addrList)
{
    QList<ControlCommand*> tmpList;
    for(quint16 addr : addrList)
    {
        ControlCommand *command=new ControlCommand(CommandManager::getInstance()->getOnCommand(addr), false, "OneOn");
        command->setLogExtraText(QString("Addr=%1").arg(addr&0xFF));
        tmpList.push_back(command);
    }
    runCommands(tmpList);
}

void CommandTaskManager::runOffCommand(QList<quint16> addrList)
{
    QList<ControlCommand*> tmpList;
    for(quint16 addr : addrList)
    {
        ControlCommand *command=new ControlCommand(CommandManager::getInstance()->getOffCommand(addr), false, "OneOff");
        command->setLogExtraText(QString("Addr=%1").arg(addr&0xFF));
        tmpList.push_back(command);
    }
    runCommands(tmpList);
}

void CommandTaskManager::runDimmingCommand(QList<quint16> addrList, int pBrightness)
{
    int brightness=getRange0To100Brightness(pBrightness);
    QList<ControlCommand*> tmpList;
    for(quint16 addr : addrList)
    {
        ControlCommand *command=new ControlCommand(CommandManager::getInstance()->getDimmingCommand(addr, brightness), false, "OneDimming");
        command->setLogExtraText(QString("Addr=%1, Brightness=%2").arg(addr&0xFF).arg(brightness));
        tmpList.push_back(command);
    }
    runCommands(tmpList);
}

void CommandTaskManager::runAllDimmingCommand(quint8 pBrightness)
{
    int brightness=getRange0To100Brightness(pBrightness);
    ControlCommand *command=new ControlCommand(CommandManager::getInstance()->getAllDimmingCommand(brightness), false, "AllDimming");
    command->setLogExtraText(QString("Brightness=%1").arg(brightness));
    runCommand(command);
}

void CommandTaskManager::runGroupDimmingCommand(quint8 groupId, quint8 pBrightness, bool highPriority, int calculateNumber)
{
    if(highPriority)
    {
        optimizateCommandBuffer(groupId, calculateNumber);
    }
    int brightness=getRange0To100Brightness(pBrightness);

    ControlCommand *command=new ControlCommand(CommandManager::getInstance()->getGroupDimmingCommand(groupId, brightness), true, "GroupDimming", calculateNumber);
    command->setGroupId(groupId);
    command->setHighPriority(highPriority);
    command->setLogExtraText(QString("GroupID=%1, Brightness=%2").arg(groupId&0xFF).arg(brightness));

    runCommand(command, highPriority);
}

void CommandTaskManager::runFindLumCommand(quint8 addr)
{
    if(findLumTimer.isActive())
    {
        return;
    }
    m_findLumAddr=addr;
    m_findLumCurrentNumber=0;
    findLumTimer.start();
}

void CommandTaskManager::runEnableCameraAP(quint8 addr)
{
    ControlCommand *command=new ControlCommand(CommandManager::getInstance()->getEnableCameraCommand(addr), false, "EnableCameraAP");
    command->setLogExtraText(QString("Addr=%1").arg(addr&0xFF));
    runCommand(command);
}

void CommandTaskManager::runDisableCameraAP(quint8 addr)
{
    ControlCommand *command=new ControlCommand(CommandManager::getInstance()->getDisableCameraCommand(addr), false, "DisableCameraAP");
    command->setLogExtraText(QString("Addr=%1").arg(addr&0xFF));
    runCommand(command);
}
void CommandTaskManager::runSetADBoardDisplayMode(quint8 addr, int mode)
{
    ControlCommand *command=new ControlCommand(CommandManager::getInstance()->getSetADBoardDisplayModeCommand(addr, mode), false, "SetADBoardMode");
    command->setLogExtraText(QString("Addr=%1, Mode=%2").arg(addr&0xFF).arg(mode));
    runCommand(command);
}

void CommandTaskManager::runSetGroupADBoardDisplayMode(quint8 groupId, int mode, bool hightPriority, int calculateNumber)
{
    if(hightPriority)
    {
        optimizateCommandBuffer(groupId, calculateNumber);
    }
    ControlCommand *command = new ControlCommand(CommandManager::getInstance()->getSetGroupADBoardDisplayModeCommand(groupId, mode), true, "SetGroupADBoardMode", calculateNumber);
    command->setGroupId(groupId);
    command->setHighPriority(hightPriority);
    command->setLogExtraText(QString("GroupID=%1, Mode=%2").arg(groupId&0xFF).arg(mode));
    runCommand(command);
}

void CommandTaskManager::runSwitchKeyGroupSetup(quint8 key, quint8 group, bool hightPriority, int calculateNumber)
{
    ControlCommand *command = new ControlCommand(CommandManager::getInstance()->getSetSwitchKeyForGroupCommand(key, group),
                                                 true, "SetGroupForSwitchKey", calculateNumber);
    command->setHighPriority(hightPriority);
    command->setLogExtraText(QString("Key[%1] to be assigned into group[%2]").arg(key).arg(group));
    runCommand(command);
}

void CommandTaskManager::runUVSterilizingCommand(QList<quint16> addrList, int nSterilizeTime, int nCycleTime, bool bAutoCycle)
{
//    ControlCommand *command;

//    if (bAutoCycle)
//    {
//        command = new ControlCommand(CommandManager::getInstance()->getSetSwitchKeyForGroupCommand(key, group),
//                                                     true, "UVSterilizing");
//    }
//    else
//    {
//        command = new ControlCommand(CommandManager::getInstance()->getSetSwitchKeyForGroupCommand(key, group),
//                                                     true, "UVSterilizing");
//    }

//    command->setHighPriority(hightPriority);
//    command->setLogExtraText(QString("Key[%1] to be assigned into group[%2]").arg(key).arg(group));
//    runCommand(command);
}

int CommandTaskManager::bufferedCommandNumber()
{
    return commandList.size()+highestPriorityCommandList.size();
}

void CommandTaskManager::clearAllBufferedCommand()
{
    while(!highestPriorityCommandList.isEmpty())
    {
        delete highestPriorityCommandList.takeFirst();
    }
    while(!commandList.isEmpty())
    {
        delete commandList.takeFirst();
    }
}

bool CommandTaskManager::hasTooManyBufferedCommand()
{
    return (bufferedCommandNumber()>=m_meshModel->savingBox()->thresholdOfBufferedCommandNumberInBusyScheduling());
}

void CommandTaskManager::optimizateCommandBuffer(quint8 groupId, int calculateNumber)
{
    for(int i=0; i<commandList.size(); i++)
    {
        ControlCommand *controCommand=commandList.at(i);
        if(!controCommand->isValid())
        {
            continue;
        }
        if(controCommand->isGroup() && (controCommand->groupId()==groupId) && (controCommand->calculateNumber()!=calculateNumber))
        {
            controCommand->setIsValid(false);
        }
    }
}

int CommandTaskManager::getRange0To100Brightness(int brightness)
{
    int result=brightness;
    if(result>=100)
    {
        result=100;
    }
    else if(result<=m_meshModel->savingBox()->luminaireMinBrightness())
    {
        result=5;
    }
    return result;
}

void CommandTaskManager::keepTickTimerStart()
{
    if(!tickTimer.isActive())
    {
        quint64 currentMSec=QDateTime::currentMSecsSinceEpoch();
        if((currentMSec-m_tickTimerStopMSec)>=m_meshModel->savingBox()->cmdIntervalMsec())
        {
            tickTimer.setInterval(10);  // 设置10毫秒，是为了让计时器立刻触发一次，在doTickTimerout中会重新将时间间隔设置回正常值
        }
        tickTimer.start();
    }
}

CommandTaskManager::CommandTaskManager(MeshModel *meshModel, QObject *parent)
    : QObject(parent),
      m_meshModel(meshModel)
{
    tickTimer.setInterval(m_meshModel->savingBox()->cmdIntervalMsec());
    connect(&tickTimer, SIGNAL(timeout()), this, SLOT(doTickTimerout()));

    m_findLumCurrentNumber=0;
    m_tickTimerStopMSec=0;
    findLumTimer.setInterval(1000);
    connect(&findLumTimer, SIGNAL(timeout()), this, SLOT(doFindLumTimerout()));
}

void CommandTaskManager::runTurnOffAutoNotificationOfLuminairesCommand()
{
    ControlCommand *command=new ControlCommand(CommandManager::getInstance()->getTurnOffAutoNotificationOfLuminairesCommand(), false, "TurnOffAutoNotification");
    runCommand(command);
}

void CommandTaskManager::runHighestPriorityCommand(ControlCommand *command)
{
    highestPriorityCommandList.push_back(command);
    keepTickTimerStart();
}

void CommandTaskManager::runHighestPriorityCommands(const QList<ControlCommand *> &commandList)
{
    highestPriorityCommandList.append(commandList);
    keepTickTimerStart();
}

void CommandTaskManager::runCommand(ControlCommand *command, bool hightPriority)
{
    if(hightPriority)
    {
        commandList.push_front(command);
    }
    else
    {
        commandList.push_back(command);
    }
    keepTickTimerStart();
}

void CommandTaskManager::runCommands(const QList<ControlCommand*> &pcommandList, bool hightPriority)
{
    if(hightPriority)
    {
        int len=pcommandList.size();
        if(len>0)
        {
            for(int i=(len-1); i>=0; i--)
            {
                commandList.push_front(pcommandList.at(i));
            }
        }
    }
    else
    {
        commandList.append(pcommandList);
    }

    keepTickTimerStart();
}

void CommandTaskManager::doTickTimerout()
{
    qDebug()<<"CommandTaskManager::doTickTimerout commandList.size="<<commandList.size();
    ControlCommand *controlCommand=NULL;
    if(!highestPriorityCommandList.isEmpty())
    {
        controlCommand=highestPriorityCommandList.takeFirst();
    }
    else
    {
        while(!commandList.isEmpty())
        {
            controlCommand=commandList.takeFirst();
            if(controlCommand->isValid())
            {
                break;
            }
            else
            {
                delete controlCommand;
                controlCommand=NULL;
            }
        }
    }

    if(controlCommand)
    {
        QByteArray command=controlCommand->command();
        m_meshModel->prepareMeshModel()->remoteConfigManager()->remoteConfigSessionHolder()->writeCommandToTKTMesh(command);
        m_meshModel->meshLogManager()->pushLog(QString("WriteCommand: %1").arg(controlCommand->toString()));

        delete controlCommand;
    }

    bool hasValidCommand=false;
    for(int i=0; i<commandList.size(); i++)
    {
        if(commandList.at(i)->isValid())
        {
            hasValidCommand=true;
            break;
        }
    }

    if(!hasValidCommand)
    {
        while(commandList.size()!=0)
        {
            delete commandList.takeFirst();   //如果剩下的全是无效的命令，应该全部清空
        }
    }

    tickTimer.setInterval(m_meshModel->savingBox()->cmdIntervalMsec());  //必须在这儿将时间间隔设置回正常值

    if(highestPriorityCommandList.isEmpty() && commandList.isEmpty())
    {
        tickTimer.stop();
        m_tickTimerStopMSec=QDateTime::currentMSecsSinceEpoch();  //记录下tickTimer停止的时间
        return;
    }
}

void CommandTaskManager::doFindLumTimerout()
{
    if((m_findLumCurrentNumber%2)==0)
    {
        QList<quint16> m_addrList;
        m_addrList.push_back(m_findLumAddr);
        runOnCommand(m_addrList);
    }
    else
    {
        QList<quint16> m_addrList;
        m_addrList.push_back(m_findLumAddr);
        runOffCommand(m_addrList);
    }

    m_findLumCurrentNumber++;
    if(m_findLumCurrentNumber>=6)
    {
        findLumTimer.stop();
        m_findLumCurrentNumber=0;
    }
}

ControlCommand::ControlCommand(QByteArray command, bool isGroup, const QString &commandTypeText, int calculateNumber)
{
    this->m_highPriority=false;
    this->m_groupId=0;
    this->m_isValid=true;

    this->m_command=command;
    this->m_isGroup=isGroup;
    this->m_commandTypeText=commandTypeText;
    this->m_calculateNumber=calculateNumber;
}

QByteArray ControlCommand::command()
{
    return this->m_command;
}

void ControlCommand::setCommand(const QByteArray &command)
{
    this->m_command=command;
}

bool ControlCommand::isGroup()
{
    return this->m_isGroup;
}

void ControlCommand::setIsGroup(bool isGroup)
{
    this->m_isGroup=isGroup;
}

quint8 ControlCommand::groupId()
{
    return this->m_groupId;
}

void ControlCommand::setGroupId(quint8 groupId)
{
    this->m_groupId=groupId;
}

bool ControlCommand::highPriority()
{
    return this->m_highPriority;
}

void ControlCommand::setHighPriority(bool highPriority)
{
    this->m_highPriority=highPriority;
}

bool ControlCommand::isValid()
{
    return this->m_isValid;
}

void ControlCommand::setIsValid(bool isValid)
{
    this->m_isValid=isValid;
}

int ControlCommand::calculateNumber()
{
    return this->m_calculateNumber;
}

void ControlCommand::setCommandTypeText(const QString &commandTypeText)
{
    this->m_commandTypeText=commandTypeText;
}

QString ControlCommand::commandTypeText()
{
    return this->m_commandTypeText;
}

void ControlCommand::setLogExtraText(const QString &logExtraText)
{
    this->m_logExtraText=logExtraText;
}

QString ControlCommand::logExtraText()
{
    return this->m_logExtraText;
}

QString ControlCommand::toString()
{
    QString text;
    text.push_back(QString("CommandType=%1").arg(this->commandTypeText()));
    if(!this->logExtraText().isEmpty())
    {
        text.push_back(", ");
        text.push_back(this->logExtraText());
    }
    return text;
}
