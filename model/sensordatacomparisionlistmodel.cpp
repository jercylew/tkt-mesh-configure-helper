#include "sensordatacomparisionlistmodel.h"

SensorDataComparisionListModel::SensorDataComparisionListModel(QObject *parent)
    : QObject(parent)
{
}

int SensorDataComparisionListModel::count()
{
    return m_lstDataCmp.size();
}

SensorDataComparision *SensorDataComparisionListModel::at(int index)
{
    if (index >= m_lstDataCmp.size() || index < 0)
    {
        return nullptr;
    }

    return m_lstDataCmp.at(index);
}

void SensorDataComparisionListModel::append(SensorDataComparision *data)
{
    m_lstDataCmp.append(data);
    emit modelChanged();
}

void SensorDataComparisionListModel::update(int index, SensorDataComparision *data)
{
    if (index >= m_lstDataCmp.size() || index < 0)
    {
        return;
    }

    m_lstDataCmp[index]->m_strSensorId = data->m_strSensorId;
    m_lstDataCmp[index]->m_strOperator = data->m_strOperator;
    m_lstDataCmp[index]->m_strValue = data->m_strValue;
    m_lstDataCmp[index]->m_dDataThresold = data->m_dDataThresold;

    //TODO: more precisely, emit dataChanged(topLeft, bottomRight)
    emit dataChanged();
}

void SensorDataComparisionListModel::removeAt(int index)
{
    m_lstDataCmp.removeAt(index);
    emit modelChanged();
}

void SensorDataComparisionListModel::clear()
{
    m_lstDataCmp.clear();
    emit modelChanged();
}

