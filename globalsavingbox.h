#ifndef GLOBALSAVINGBOX_H
#define GLOBALSAVINGBOX_H

#include <QObject>

#define DEFAULT_MESH_UUID   "00000000000000000000000000000000"

class GlobalSavingBox : public QObject
{
    Q_OBJECT
public:
    static GlobalSavingBox *getInstance();

    QString getMeshDBFilePath();
    QString getDataRootDirPath();
    QString defaultMeshUUID();

    void setValue(const QString &key, const QString &value);
    QString getValue(const QString &key);
    QMap<QString, QString> getAll();
    void setAll(const QMap<QString, QString> &all);
    void removeAllKey(const QList<QString> &keyList);

private:
    explicit GlobalSavingBox(QObject *parent = 0);
    static GlobalSavingBox *m_globalSavingBox;

signals:

public slots:
};

#endif // GLOBALSAVINGBOX_H
