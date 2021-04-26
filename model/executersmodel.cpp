#include "executersmodel.h"
#include "utils/commandtaskmanager.h"
#include <QDebug>
#include "meshmodel.h"
#include "commandmanager.h"
#include "utils/meshsavingbox.h"
#include "domain/mesh.h"
#include "model/sensorsmodel.h"
#include "domain/onechannelluminaire.h"
#include "objectfactory.h"
#include "parsermanager.h"
#include "parser/executerparser.h"
#include "utils/meshlogmanager.h"
#include "model/nodegroupsmodel.h"
#include "db/meshdbcaller.h"
#include "db/meshdbmanager.h"
#include "parser/executerparser.h"

ExecutersModel::ExecutersModel(MeshModel *meshModel, QObject *parent)
    : QObject(parent),
      m_meshModel(meshModel)
{
    nodeIndex=0;

    m_checkOnlineStatusChangeTimer.setInterval(2000);
    connect(&m_checkOnlineStatusChangeTimer, SIGNAL(timeout()),
            this, SLOT(doCheckOnlineStatusChange()));
    m_checkOnlineStatusChangeTimer.start();
}

void ExecutersModel::discover()
{
    m_meshModel->commandTaskManager()->runOnlineAllCommand();
    m_meshModel->commandTaskManager()->runGetAllG8Command();
}

void ExecutersModel::clear()
{
    nodeIndex=0;
    for(Executer *executer : m_executerList)
    {
        delete executer;
    }
    m_executerList.clear();
    m_executerMap.clear();
    emit modelChanged();
}

bool ExecutersModel::contains(const QString &executerId)
{
    return m_executerMap.contains(executerId);
}

void ExecutersModel::updateExecuterByDBReason(Executer *dbExecuter)
{
    ExecuterParser *executerParser=ParserManager::instance()->getExecuterParserByTypeText(dbExecuter->typeText());
    if(executerParser==NULL)
    {
        return;
    }
    Executer *oldExecuter=m_executerMap.value(dbExecuter->executerId());
    if(oldExecuter==NULL)
    {
        oldExecuter=ObjectFactory::instance()->createExecuterByTypeText(dbExecuter->typeText());
        if(oldExecuter==NULL)
        {
            return;
        }
        oldExecuter->setExecuterId(dbExecuter->executerId());
        executerParser->updateExecuterFromDB(m_meshModel, dbExecuter, oldExecuter);
        m_executerMap.insert(oldExecuter->executerId(), oldExecuter);
        m_executerList.push_back(oldExecuter);
    }
    else
    {
        executerParser->updateExecuterFromDB(m_meshModel, dbExecuter, oldExecuter);
    }
    executerParser->updateReferredExecuterDataAndNotify(m_meshModel, oldExecuter, true);
}

void ExecutersModel::updateExecuterByOnlineStatusReason(Executer *executer)
{
    ExecuterParser *executerParser=ParserManager::instance()->getExecuterParserByTypeText(executer->typeText());
    if(executerParser==NULL)
    {
        return;
    }
    if(m_executerMap.contains(executer->executerId()))
    {
        Executer *oldExecuter=m_executerMap.value(executer->executerId());
        executerParser->updateExecuterFromOnlineStatus(m_meshModel, executer, oldExecuter);
        emit dataChanged();
    }
    else
    {
        Executer *copyExecuter=ObjectFactory::instance()->createExecuterByTypeText(executer->typeText());
        if(copyExecuter==NULL)
        {
            return;
        }
        copyExecuter->setExecuterId(executer->executerId());
        copyExecuter->setName(copyExecuter->executerId());
        executerParser->updateExecuterFromOnlineStatus(m_meshModel, executer, copyExecuter);
        m_executerList.push_back(copyExecuter);
        m_executerMap.insert(copyExecuter->executerId(), copyExecuter);

//        emit modelChanged();

        QMap<QString, QPair<QString, QString> > *nameMap=new QMap<QString, QPair<QString, QString> >;
        nameMap->insert(copyExecuter->executerId(),
                        QPair<QString, QString>(copyExecuter->typeText(), copyExecuter->getName()));
        m_meshModel->dbManager()->dbCaller()->setExecuterNodesName(nameMap);
    }

    Executer *reportExecuter=m_executerMap.value(executer->executerId());
    if(reportExecuter==NULL)
    {
        return;
    }
    emit bluetoothNodeChanged(reportExecuter);
}

void ExecutersModel::updateExecuterByUserDataNotifyReason(Executer *executer)
{
    ExecuterParser *executerParser=ParserManager::instance()->getExecuterParserByTypeText(executer->typeText());
    if(executerParser==NULL)
    {
        return;
    }
    if(m_executerMap.contains(executer->executerId()))
    {
        Executer *oldExecuter=m_executerMap.value(executer->executerId());
        executerParser->updateExecuterFromUserDataNotify(m_meshModel, executer, oldExecuter);
        emit dataChanged();
    }
    else
    {
        Executer *copyExecuter=ObjectFactory::instance()->createExecuterByTypeText(executer->typeText());
        if(copyExecuter==NULL)
        {
            return;
        }
        copyExecuter->setExecuterId(executer->executerId());
        copyExecuter->setName(copyExecuter->executerId());
        copyExecuter->setBluetoothAddress(executer->getBluetoothAddress());
        executerParser->updateExecuterFromUserDataNotify(m_meshModel, executer, copyExecuter);
        m_executerList.push_back(copyExecuter);
        m_executerMap.insert(copyExecuter->executerId(), copyExecuter);

//        emit modelChanged();

        QMap<QString, QPair<QString, QString> > *nameMap=new QMap<QString, QPair<QString, QString> >;
        nameMap->insert(copyExecuter->executerId(),
                        QPair<QString, QString>(copyExecuter->typeText(), copyExecuter->getName()));
        m_meshModel->dbManager()->dbCaller()->setExecuterNodesName(nameMap);
    }

    Executer *reportExecuter=m_executerMap.value(executer->executerId());
    if(reportExecuter==NULL)
    {
        return;
    }
    emit bluetoothNodeChanged(reportExecuter);
}

void ExecutersModel::updateReferredExecuterDataAndNotify(Executer *executer)
{
    ExecuterParser *executerParser=ParserManager::instance()->getExecuterParserByTypeText(executer->typeText());
    if(executerParser)
    {
        executerParser->updateReferredExecuterDataAndNotify(m_meshModel, executer);
    }
}

int ExecutersModel::count()
{
    return m_executerList.size();
}

Executer *ExecutersModel::at(int index)
{
    if(index<m_executerList.size())
    {
        return m_executerList.at(index);
    }
    else
    {
        return NULL;
    }
}

Executer *ExecutersModel::getExecuter(const QString &id)
{
    return m_executerMap.value(id);
}

Executer *ExecutersModel::getExecuterByExecuterId(const QString &executerId)
{
    return m_executerMap.value(executerId);
}

void ExecutersModel::setGroupList(quint8 addr, QList<quint8> groupList)
{
    for(int i=0; i<m_executerList.size(); i++)
    {
        Executer *executer=m_executerList.at(i);
        if(executer->isConfigurable())
        {
            continue;
        }
        if(executer->getBluetoothAddress()==addr)
        {
            executer->setGroupList(groupList);
            break;
        }
    }
    emit modelChanged();
}

int ExecutersModel::countOfGroup(quint8 groupId)
{
    if(groupId==0) //All Group
    {
        return m_executerList.size();
    }
    else if(groupId==255)  //Ungrouped
    {
        int size=0;
        for(Executer *executer : m_executerList)
        {
            if(executer->getGroupList().size()==0)
            {
                size++;
            }
        }
        return size;
    }
    else
    {
        int size=0;
        for(Executer *executer : m_executerList)
        {
            if(executer->belongsToGroup(groupId))
            {
                size++;
            }
        }
        return size;
    }
}

QString ExecutersModel::getNodeName(const QString &id)
{
    Executer *executer=m_executerMap.value(id);
    if(executer==NULL)
    {
        return "";
    }
    else
    {
        return executer->getName();
    }
}

int ExecutersModel::getBrightnessValue(const QString &id)
{
    Executer *executer=m_executerMap.value(id);
    if(executer==NULL || (executer->typeText()!=OneChannelLuminaire::staticTypeText()))
    {
        return -1;
    }
    else
    {
        OneChannelLuminaire *one=static_cast<OneChannelLuminaire*>(executer);
        if(one)
        {
            return one->getBrightness();
        }
        else
        {
            return -1;
        }
    }
}

QList<Executer *> ExecutersModel::getAll()
{
    return m_executerList;
}

bool ExecutersModel::parseADataFrame(const QByteArray &dataFrame)
{
    qDebug()<<"dataFrame="<<dataFrame.toHex();
    bool parsed=true;
    do{
        if(dataFrame.size()<20)
        {
            parsed=false;
            break;
        }
        if(dataFrame.at(7)==CMD_MARK_CODE_LIGHT_STATUS)
        {
            if(dataFrame.at(10)!=0)  //第一路灯
            {
                quint8 nodeTypeCode=dataFrame.at(13);
                qDebug() << "ExecutersModel::parseADataFrame, dataFrame=" <<
                                        QString::fromLatin1(dataFrame.toHex()) <<
                            "Light Status, CH1: nodeTypeCode=" << hex << nodeTypeCode;
                if(ObjectFactory::instance()->isExecuterNodeTypeCode(nodeTypeCode))
                {
                   Executer *executer=ObjectFactory::instance()->createExecuterByNodeTypeCode(nodeTypeCode);
                   if(executer!=NULL)
                   {
                       ExecuterParser *executerParser=ParserManager::instance()->getExecuterParserByTypeText(executer->typeText());
                       if(executerParser!=NULL)
                       {
                           executerParser->parseOnlineStatus4BytesDataFrameToExecuter(dataFrame.mid(10, 4), executer);
                           updateExecuterByOnlineStatusReason(executer);
                       }
                       delete executer;
                   }
                }
                else
                {
                    parsed=false;
                }
            }
            if(dataFrame.at(14)!=0)  //第二路灯
            {
                quint8 nodeTypeCode=dataFrame.at(17);
                qDebug() << "ExecutersModel::parseADataFrame, dataFrame=" <<
                                        QString::fromLatin1(dataFrame.toHex()) <<
                            "Light Status, CH2: nodeTypeCode=" << hex << nodeTypeCode;
                if(ObjectFactory::instance()->isExecuterNodeTypeCode(nodeTypeCode))
                {
                   Executer *executer=ObjectFactory::instance()->createExecuterByNodeTypeCode(nodeTypeCode);
                   if(executer!=NULL)
                   {
                       ExecuterParser *executerParser=ParserManager::instance()->getExecuterParserByTypeText(executer->typeText());
                       if(executerParser!=NULL)
                       {
                           executerParser->parseOnlineStatus4BytesDataFrameToExecuter(dataFrame.mid(14, 4), executer);
                           updateExecuterByOnlineStatusReason(executer);
                       }
                       delete executer;
                   }
                }
                else
                {
                    parsed=false;
                }
            }
        }
        else if(dataFrame.at(7)==CMD_MARK_CODE_USER_NOTIFY_DATA)
        {
            quint8 code=dataFrame.at(10);
            qDebug() << "ExecutersModel::parseADataFrame, dataFrame=" <<
                                QString::fromLatin1(dataFrame.toHex()) <<
                        "User Notify: Code=" << hex << code;
            if(ObjectFactory::instance()->isExecuterUserDataTypeCode(code))
            {
                Executer *executer=ObjectFactory::instance()->createExecuterByNotifyDataCode(code);
                if(executer!=NULL)
                {
                    ExecuterParser *executerParser=ParserManager::instance()->getExecuterParserByTypeText(executer->typeText());
                    if(executerParser!=NULL)
                    {
                        executerParser->parseUserDataFrameToExecuter(dataFrame, executer);
                        updateExecuterByUserDataNotifyReason(executer);
                        updateReferredExecuterDataAndNotify(executer);
                    }
                    delete executer;
                }
            }
            else
            {
                parsed=false;
            }
        }
        else if(dataFrame.at(7)==CMD_MARK_CODE_GROUP_G8_NOTIFY_DATA)
        {
            quint8 addr=(quint8)dataFrame.at(3);
            QList<quint8> groupList;
            for(int i=10; i<18; i++)
            {
                quint8 group=dataFrame.at(i);
                if(group!=(quint8)0xFF)
                {
                    groupList.append(group);
                }
            }
            setGroupList(addr, groupList);
        }
    }while(false);

    return parsed;
}

void ExecutersModel::doCheckOnlineStatusChange()
{
    int len=m_executerList.size();
    bool changed=false;
    for(int i=0; i<len; i++)
    {
        Executer *lum=m_executerList.at(i);
        bool oldOnline=lum->isOnline();
        lum->checkOnlineStatusTimeoutChange(60*1000); //60 seconds
        if(oldOnline!=lum->isOnline())
        {
            changed=true;
        }
    }
    if(changed)
    {
        emit modelChanged();
    }
}

void ExecutersModel::doSyncExecuterNodes(QMap<QString, Executer *> *executerMap)
{
    QList<Executer*> executerList=executerMap->values();
    QList<QString> removeKeys=this->m_executerMap.keys();

    while(!executerList.isEmpty())
    {
        Executer *executer=executerList.takeFirst();
        removeKeys.removeAll(executer->executerId());
        updateExecuterByDBReason(executer);
        delete executer;
    }

    for(int i=0; i<removeKeys.size(); i++)
    {
        QString removeKey=removeKeys.at(i);
        Executer *executer=this->m_executerMap.value(removeKey);
        if(executer==NULL)
        {
            continue;
        }
        if(executer->isVirtual() || (!executer->isTouched()))
        {
            this->m_executerList.removeAll(executer);
            this->m_executerMap.remove(removeKey);
            delete executer;
        }
        else if((!executer->isVirtual()) && (executer->isTouched()))
        {
            ExecuterParser *executerParser=ParserManager::instance()->getExecuterParserByTypeText(executer->typeText());
            if(executerParser)
            {
                executer->setName("");
            }
        }
    }

    emit modelChanged();
    delete executerMap;
}

