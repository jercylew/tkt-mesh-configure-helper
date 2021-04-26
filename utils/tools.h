#ifndef TOOLS_H
#define TOOLS_H

#include <QJsonObject>

class Tools
{
public:
    Tools();

    static QByteArray getDataArrayOfJsonObject(QJsonObject *jsonObject);
    static QString getRandomUUID();
    static QJsonObject getJsonObjectFromDataArray(const QByteArray &dataArray);
    static QByteArray encodeAES128Base64Data(const QByteArray &data, const QString &key);
    static QByteArray decodeAES128Base64Data(const QByteArray &base64Data, const QString &key);
    static QString getFileMD5(const QString &sourceFilePath);
    static QString getMD5Text(const QString &text);
};

#endif // TOOLS_H
