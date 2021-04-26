#ifndef MYTRANSLATOR_H
#define MYTRANSLATOR_H

#include <QTranslator>
#include <QMap>
#include <QDebug>

class LanguageTranslator : public QTranslator
{
    Q_OBJECT
public:
    explicit LanguageTranslator(QObject *parent = 0);

    ~LanguageTranslator();

    QString translate(const char *context, const char *sourceText, const char *disambiguation = Q_NULLPTR, int n = -1) const;

    void setTranslateFilePath(const QString &filePath);

signals:

public slots:

private:
    QMap<QString, QString> *translateMap;

};

#endif // MYTRANSLATOR_H
