#ifndef EXECUTER_H
#define EXECUTER_H

#include "bluetoothnode.h"


class Executer : public BluetoothNode
{
public:
    Executer();

    QString executerId();
    void setExecuterId(const QString &executerId);

    QString executerData();
    void setExecuterData(const QString &executerData);

    virtual void loadInUpdateExecuter(Executer *executer);
    virtual void loadExecuterFromDB(Executer *executer);
    virtual void loadUserData(Executer *executer);
    virtual bool isLuminaire();
    virtual QString nodeKindText();
    static QString staticNodeKindText();

    QString nodeId();

private:
    QString m_executerId;
    QString m_executerData;
};

#endif // EXECUTER_H
