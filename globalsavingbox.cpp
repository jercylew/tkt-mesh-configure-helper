#include "globalsavingbox.h"
#include <QDir>
#include <QSettings>
#include <QApplication>
#include <QDebug>

#define TEMP_DB_FILE_PATH   (QString("%1/temp_mesh_data.db").arg(QDir::tempPath()))
#define DB_FILE_PATH    (QString("%1/mesh_data.db").arg(QDir::tempPath()))

GlobalSavingBox* GlobalSavingBox::m_globalSavingBox=NULL;

GlobalSavingBox *GlobalSavingBox::getInstance()
{
    if(m_globalSavingBox==NULL)
    {
        m_globalSavingBox=new GlobalSavingBox;
    }
    return m_globalSavingBox;
}

QString GlobalSavingBox::getMeshDBFilePath()
{
    return DB_FILE_PATH;
}

QString GlobalSavingBox::getDataRootDirPath()
{
    return QDir::tempPath();
}

QString GlobalSavingBox::defaultMeshUUID()
{
    return DEFAULT_MESH_UUID;
}

GlobalSavingBox::GlobalSavingBox(QObject *parent) : QObject(parent)
{
    qDebug()<<"Temp Dir = "<<QDir::tempPath();
}

void GlobalSavingBox::setValue(const QString &key, const QString &value)
{
    QSettings settings(QApplication::applicationDirPath()+QString("/run.ini"), QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.setValue(key, value);
    settings.sync();
}

QString GlobalSavingBox::getValue(const QString &key)
{
    QSettings settings(QApplication::applicationDirPath()+QString("/run.ini"), QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    return settings.value(key).toString();
}

QMap<QString, QString> GlobalSavingBox::getAll()
{
    QSettings settings(QApplication::applicationDirPath()+QString("/run.ini"), QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QStringList allKeys=settings.allKeys();
    QMap<QString, QString> all;
    for(int i=0; i<allKeys.size(); i++)
    {
        all.insert(allKeys.at(i), settings.value(allKeys.at(i)).toString());
    }
    return all;
}

void GlobalSavingBox::setAll(const QMap<QString, QString> &all)
{
    QSettings settings(QApplication::applicationDirPath()+QString("/run.ini"), QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QStringList keys=all.keys();
    for(int i=0; i<keys.size(); i++)
    {
        settings.setValue(keys.at(i), all.value(keys.at(i)));
    }
    settings.sync();
}

void GlobalSavingBox::removeAllKey(const QList<QString> &keyList)
{
    QSettings settings(QApplication::applicationDirPath()+QString("/run.ini"), QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    for(int i=0; i<keyList.size(); i++)
    {
        settings.remove(keyList.at(i));
    }
    settings.sync();
}
