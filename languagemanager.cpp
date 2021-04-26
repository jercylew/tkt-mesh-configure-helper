#include "languagemanager.h"
#include <QApplication>
#include <QDir>
#include <QDebug>

LanguageManager* LanguageManager::languageManager=NULL;

/*!
 * 语言管理器，管理本应用程序的语言切换，需要注意的是目前所有英文使用en来表示，而不是en_US
 */
LanguageManager::LanguageManager(QObject *parent)
    :QObject(parent),
     translator(NULL)
{
    languageMark="en_US";

    /*!
     * 必须成对调用qApp->removeTranslator和qApp->installTranslator两个函数才会触发QWidget的languageChange回调，
     * 但程序刚开始运行时没有可以remove的translator，这样会导致没有成对调用上述两个函数，最终使得BackgroundFrame
     * 的languageChange函数没有被调用，产生BackgroundFrame中的"Loading..."英文不被翻译的现象。
     *
     * 解决方法是在LanguageManager初始化时创建一个translator并install到qApp中，这样程序第一次运行到changeLanguage
     * 就能成对调用上述两个函数，触发languageChange函数，从而使得BackgroundFrame中的文字被正常翻译。
     */
    translator = new LanguageTranslator(qApp);
    translator->setTranslateFilePath(":/language/en_US.trans");
    qApp->installTranslator(translator);
    QLocale::setDefault(QLocale(this->languageMark));

    m_translateHelper=new TranslateHelper;
}

LanguageManager* LanguageManager::instance()
{
    if(languageManager==NULL){
        languageManager=new LanguageManager;
    }
    return languageManager;
}

/*!
 * 切换本应用程序语言，本应用程序的语言切换需要language/目录下对应的翻译文件的支持
 * 如将en_US.trans翻译文件放在language/目录下，则程序启动时
 * 会检测到该翻译文件并将该语言加入系统支持的语言列表中，用户在界面上选择切换到该语言
 * 就可以了。
 */
void LanguageManager::changeLanguage(const QString &pLanguageMark)
{
    QString languageMark=pLanguageMark.isEmpty()?"en_US":pLanguageMark;
    qDebug()<<"LanguageManager::changeLanguage languageMark="<<languageMark;
    this->languageMark=languageMark;
    if(translator!=NULL)
    {
        qApp->removeTranslator(translator);
        translator->deleteLater();
        translator=NULL;
    }

    translator = new LanguageTranslator;
    translator->setTranslateFilePath(":/language/"+languageMark+".trans");
    qApp->installTranslator(translator);

    QLocale::setDefault(QLocale(this->languageMark));

}

/*!
 * 返回所支持的语言的语言名称列表
 */
QMap<QString, QString> LanguageManager::getSupportedLanguageMarkNameMap()
{
    QMap<QString, QString> markNameMap;
    QDir dir(":/language/");
    dir.setFilter(QDir::Files | QDir::NoSymLinks);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.completeSuffix()=="trans"){
            QString languageMark=fileInfo.baseName();
            LanguageTranslator translator;
            translator.setTranslateFilePath(":/language/"+fileInfo.fileName());
            QString languageName = translator.translate("", "Global.LanguageName");
            markNameMap.insert(languageMark, languageName);
        }
    }
    return markNameMap;
}


QString LanguageManager::currentLanguageMark()
{
    return this->languageMark;
}

TranslateHelper *LanguageManager::translateHelper()
{
    return m_translateHelper;
}

