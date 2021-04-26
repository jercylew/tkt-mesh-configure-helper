#include "executerparser.h"
#include "domain/executer.h"
#include "widget/executerframe.h"
#include <QLabel>
#include <QPainter>
#include <QSqlQuery>

ExecuterParser::ExecuterParser()
{
    m_executerFrame=NULL;
}

void ExecuterParser::parseDBDataToExecuter(QSqlQuery *query, Executer *executer)
{
    executer->setName(query->value("executer_name").toString());
    executer->setExecuterId(query->value("executer_id").toString());
    QString data=query->value("executer_data").toString();
    parseExecuterDataToExecuter(data, executer);
}

bool ExecuterParser::parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame, Executer *executer)
{
    if(dataFrame.size()!=4)
    {
        return false;
    }
    quint8 bluetoothAddress=dataFrame.at(0);
    executer->setExecuterId(QString("%1.%2").arg(executer->shortTypeText()).arg(bluetoothAddress&0xFF));
    executer->setName(executer->executerId());
    executer->setBluetoothAddress(bluetoothAddress);
    executer->setOnline(dataFrame.at(1)!=0);
    return true;
}

void ExecuterParser::parseUserDataFrameToExecuter(const QByteArray &dataFrame, Executer *executer)
{
    quint8 bluetoothAddress=dataFrame.at(3);
    executer->setExecuterId(QString("%1.%2").arg(executer->shortTypeText()).arg(bluetoothAddress&0xFF));
    executer->setName(executer->executerId());
    executer->setBluetoothAddress(bluetoothAddress);
}

void ExecuterParser::updateReferredExecuterDataAndNotify(MeshModel *meshModel, Executer *executer, bool dbReason)
{

}

void ExecuterParser::updateExecuterFromDB(MeshModel *meshModel, Executer *dbExecuter, Executer *oldExecuter)
{
    oldExecuter->setName(dbExecuter->getName());
}

void ExecuterParser::updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter)
{
    oldExecuter->setOnline(newExecuter->isOnline(), 60*1000); //60 seconds
    if(oldExecuter->getBluetoothAddress()==0)
    {
        oldExecuter->setBluetoothAddress(newExecuter->getBluetoothAddress());
    }
    if(!oldExecuter->isTouched())
    {
        oldExecuter->setTouched(true);
    }
}

void ExecuterParser::updateExecuterFromUserDataNotify(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter)
{
    if(!oldExecuter->isTouched())
    {
        oldExecuter->setTouched(true);
    }
}

void ExecuterParser::parseExecuterDataToExecuter(const QString &executerData, Executer *executer)
{

}

QString ExecuterParser::getExecuterTooltipInExecuterListView(Executer *executer)
{
    return QObject::tr("Address: %1").arg(executer->getBluetoothAddress()&0xFF);
}

QPixmap ExecuterParser::getSurfaceWithTouchInfo(MeshModel *meshModel, Executer *executer)
{
    QPixmap surface=buildSurfaceWithNoTouchInfo(meshModel, executer);
    if(executer->isVirtual())
    {
        return surface;
    }
    else
    {
        QPainter painter(&surface);
        if(executer->isTouched())
        {
            painter.drawImage(surface.width()-30, 0, QImage(":/images/touch_yes.png"));
        }
        else
        {
            painter.drawImage(surface.width()-30, 0, QImage(":/images/touch_no.png"));
        }
        return surface;
    }
}

ExecuterFrame *ExecuterParser::getExecuterFrame()
{
    if(m_executerFrame==NULL)
    {
        m_executerFrame=new ExecuterFrame;
    }
    return m_executerFrame;
}

QPixmap ExecuterParser::buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer)
{
    ExecuterFrame *executerFrame=getExecuterFrame();
    executerFrame->getIconLabel()->setText(QString::number(executer->getBluetoothAddress()&0xFF));
    executerFrame->getTextLabel()->setText(QString::number(executer->getBluetoothAddress()&0xFF));
    return executerFrame->grab();
}

