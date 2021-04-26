#include "mainwindow.h"
#include <QApplication>
#include <QThreadPool>
#include <QFile>
#include <QTextStream>
#include "utils/myproxystyle.h"
#include "languagemanager.h"
#include <QDir>
#include "globalsavingbox.h"
#include <QDebug>
#include <QLockFile>
#include <iostream>
#include <QTextCodec>

QString loadStyleSheet()
{
    QFile file(":/qss/style.qss");
    QString qss;
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream ts(&file);
        qss=ts.readAll();
        file.close();
    }
    return qss;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    QLockFile lockFile(QDir::tempPath()+"/tktmesh_configure_helper.lock");
    if(!lockFile.tryLock(100))
    {
        std::cout<<"Only one TKTMeshConfigureHelper process can be started!"<<std::endl;
        return 0;
    }
    QThreadPool::globalInstance()->setMaxThreadCount(20);
    qDebug()<<"Temp Dir Path = "<<QDir::tempPath();
    a.setStyle(new MyProxyStyle());
    QDir::setCurrent(QApplication::applicationDirPath());
    a.setStyleSheet(loadStyleSheet());
    LanguageManager::instance()->changeLanguage("zh_CN");

    MainWindow w;
    w.setWindowTitle("TKTMesh配置助手");
    w.setMinimumSize(1300, 920);
    w.show();

    return a.exec();
}