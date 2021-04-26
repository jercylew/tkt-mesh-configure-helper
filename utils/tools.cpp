#include "tools.h"
#include <QJsonDocument>
#include "utils/qaesencryption.h"
#include <QCryptographicHash>
#include <QUuid>
#include <QFile>

Tools::Tools()
{

}

QByteArray Tools::getDataArrayOfJsonObject(QJsonObject *jsonObject)
{
    return QJsonDocument(*jsonObject).toJson(QJsonDocument::Compact);
}

QString Tools::getRandomUUID()
{
    return QUuid::createUuid().toString().replace("-","").replace("{", "").replace("}","");
}

QJsonObject Tools::getJsonObjectFromDataArray(const QByteArray &dataArray)
{
    QJsonDocument jsonDocument=QJsonDocument::fromJson(dataArray);
    if(jsonDocument.isNull())
    {
        return QJsonObject();
    }
    else
    {
        return jsonDocument.object();
    }
}

QByteArray Tools::encodeAES128Base64Data(const QByteArray &data, const QString &key)
{
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::ZERO);
    QByteArray hashKey=QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Md5);
    QByteArray encodedDataBase64=encryption.encode(data, hashKey).toBase64();
    return encodedDataBase64;
}

QByteArray Tools::decodeAES128Base64Data(const QByteArray &base64Data, const QString &key)
{
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::ZERO);
    QByteArray hashKey=QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Md5);
    QByteArray decodedData=encryption.decode(QByteArray::fromBase64(base64Data), hashKey);
    return encryption.removePadding(decodedData);
}

QString Tools::getFileMD5(const QString &sourceFilePath)
{
    QFile sourceFile(sourceFilePath);
    quint64 fileSize=sourceFile.size();
    const quint64 bufferSize=10240;
    if(sourceFile.open(QFile::ReadOnly))
    {
        char buffer[bufferSize];
        int bytesRead;
        int readSize=qMin(fileSize, bufferSize);
        QCryptographicHash hash(QCryptographicHash::Md5);
        while(readSize>0 && (bytesRead=sourceFile.read(buffer, readSize))>0)
        {
            fileSize -= bytesRead;
            hash.addData(buffer, bytesRead);
            readSize=qMin(fileSize, bufferSize);
        }
        sourceFile.close();
        return QString(hash.result().toHex());
    }
    return QString();
}

QString Tools::getMD5Text(const QString &text)
{
    return QString::fromLatin1(QCryptographicHash::hash(text.toLatin1(), QCryptographicHash::Md5).toHex());
}

