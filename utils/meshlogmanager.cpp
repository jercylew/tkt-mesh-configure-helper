#include "meshlogmanager.h"
#include <QTime>
#include <QDir>
#include <QApplication>
#include <QDate>
#include <QTextStream>
#include "model/meshmodel.h"
#include "utils/meshsavingbox.h"
#include "domain/mesh.h"

#define LOG_FOLDER_HEX  "LOG_FOLDER_HEX"

MeshLogManager::MeshLogManager(MeshModel *meshModel, QObject *parent) : QObject(parent)
{
    this->m_meshModel=meshModel;

    m_logFolder=QString::fromUtf8(QByteArray::fromHex(m_meshModel->savingBox()->getValue(LOG_FOLDER_HEX).toLatin1()));
    if(m_logFolder.isEmpty())
    {
        m_logFolder=QDir::fromNativeSeparators(QApplication::applicationDirPath());
    }

    m_logSaveTimer.setInterval(60*1000); //60s
    connect(&m_logSaveTimer, SIGNAL(timeout()), this, SLOT(doLogSaveTimerout()));
    m_logSaveTimer.start();

    doLogSaveTimerout();
}

void MeshLogManager::pushLog(const QString &log, bool debugInfo)
{
    if(debugInfo && (!m_meshModel->savingBox()->debugOutputIsOpen()))
    {
        return;
    }
    while(m_logList.size()>=200)
    {
        m_logList.takeFirst();
    }
    QString logText=QString("[%1] %2").arg(QTime::currentTime().toString("HH:mm:ss-zzz")).arg(log);
    m_logList.push_back(logText);
    if(m_meshModel->savingBox()->enableLogToDisk())
    {
        m_bufferedLog.push_back(logText);
    }
}

QList<QString> MeshLogManager::getAllLog()
{
    return m_logList;
}

void MeshLogManager::clearLog()
{
    m_logList.clear();
}

QString MeshLogManager::logFolder()
{
    return this->m_logFolder;
}

void MeshLogManager::setLogFolder(const QString &logFolder)
{
    this->m_logFolder=logFolder;
    QString pathHex=QString::fromLatin1(m_logFolder.toUtf8().toHex());
    m_meshModel->savingBox()->setValue(LOG_FOLDER_HEX, pathHex);
}

void MeshLogManager::doLogSaveTimerout()
{
    if(!m_meshModel->savingBox()->enableLogToDisk())
    {
        m_bufferedLog.clear();
        return;
    }
    QDir dir;
    dir.mkpath(QString("%1/MeshLightLog").arg(m_logFolder));
    QFile logFile(QString("%1/MeshLightLog/meshlight-%2-%3.log").arg(m_logFolder).arg(m_meshModel->mesh()->meshUUID()).arg(QDate::currentDate().toString("yyyy-MM-dd")));
    if(logFile.open(QFile::WriteOnly | QFile::Append))
    {
        for(int i=0; i<m_bufferedLog.size(); i++)
        {
            logFile.write(m_bufferedLog.at(i).toUtf8());
            logFile.write("\r\n");
        }
        m_bufferedLog.clear();
        logFile.flush();
        logFile.close();
    }
}
