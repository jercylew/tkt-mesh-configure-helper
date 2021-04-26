#include "languagetranslator.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

LanguageTranslator::LanguageTranslator(QObject *parent) :
    QTranslator(parent)
{
    translateMap=new QMap<QString, QString>;
}

LanguageTranslator::~LanguageTranslator()
{
    delete translateMap;
}

QString LanguageTranslator::translate(const char *context, const char *sourceText, const char *disambiguation, int n) const
{
    QString sourceString=QString::fromUtf8(sourceText);
    QString value=translateMap->value(sourceString).trimmed();
    if(value.isEmpty())
    {
        return sourceString;
    }
    else
    {
        return value;
    }
}

/*!
 * 文件后缀名：*.trans
 * 文件格式：
   #这是注释
   Global.LanguageName=Language Name
   Global.Tip=This is a tip
 */
void LanguageTranslator::setTranslateFilePath(const QString &filePath)
{
    translateMap->clear();

    QFile file(filePath);
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream ts(&file);
        QString line;
        bool replaceMode=false;
        QList<QPair<QString, QString> > replaceList;
        int replaceListLen=replaceList.size();
        while(!(line=ts.readLine()).isNull())
        {
            line=line.trimmed();
            if(line.isEmpty())
            {
                continue;
            }
            if(line.startsWith("#"))
            {
                continue;
            }
            if(line.startsWith("@Replace"))
            {
                replaceMode=true;
                continue;
            }
            QString key=line.left(line.indexOf("="));
            QString value=line.remove(0, line.indexOf("=")+1);
            value=value.replace("\\n", "\n");
            if(replaceMode)
            {
                for(int i=0; i<replaceListLen; i++)
                {
                    value=value.replace(replaceList.at(i).first, replaceList.at(i).second);
                }
                translateMap->insert(key, value);
                replaceMode=false;
            }
            else
            {
                translateMap->insert(key, value);
            }
        }
        file.close();
    }
    else
    {
        qDebug()<<"Translation file "<<filePath<<" is not existed!";
    }
}
