#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QObject>
#include <QLocale>
#include "utils/languagetranslator.h"
#include "utils/translatehelper.h"

class LanguageManager : public QObject
{
public:
    static LanguageManager* instance();

    void changeLanguage(const QString &languageMark);

    QMap<QString, QString> getSupportedLanguageMarkNameMap();

    QString currentLanguageMark();

    TranslateHelper *translateHelper();

private:
    LanguageManager(QObject *parent = 0);

    static LanguageManager *languageManager;

    LanguageTranslator *translator;

    TranslateHelper *m_translateHelper;

    QString languageMark;
};

#endif // LANGUAGEMANAGER_H

