#include "executer.h"

#define EXECUTER_KIND_TEXT  "executer"

Executer::Executer()
{

}

QString Executer::executerId()
{
    return m_executerId;
}

void Executer::setExecuterId(const QString &executerId)
{
    m_executerId = executerId;
}

QString Executer::executerData()
{
    return m_executerData;
}

void Executer::setExecuterData(const QString &executerData)
{
    m_executerData = executerData;
}

void Executer::loadExecuterFromDB(Executer *executer)
{

}

void Executer::loadUserData(Executer *executer)
{

}

bool Executer::isLuminaire()
{
    return true;
}

QString Executer::nodeKindText()
{
    return EXECUTER_KIND_TEXT;
}

QString Executer::staticNodeKindText()
{
    return EXECUTER_KIND_TEXT;
}

QString Executer::nodeId()
{
    return QString("%1.%2").arg(shortTypeText()).arg(getBluetoothAddress()&0xFF);
}

void Executer::loadInUpdateExecuter(Executer *executer)
{
    this->setName(executer->getName());
}
