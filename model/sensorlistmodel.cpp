#include "sensorlistmodel.h"

#include "sensorsmodel.h"
#include <QDebug>
#include "utils/meshsavingbox.h"
#include "utils/meshruntimeparameterbox.h"

bool sensorIDLessThan(Sensor *sensor1, Sensor *sensor2)
{
    return sensor1->sensorId() < sensor2->sensorId();
}

bool sensorNameLessThan(Sensor *sensor1, Sensor *sensor2)
{
    return sensor1->getName() < sensor2->getName();
}

SensorListModel::SensorListModel(SensorsModel *sensorsModel, MeshSavingBox *savingBox, QObject *parent)
    : QObject(parent),
      m_sensorsModel(sensorsModel)
{
    this->m_savingBox=savingBox;
}

void SensorListModel::init()
{
    refreshList();
}

MeshRuntimeParameterBox::SensorOrderMethod SensorListModel::orderMethod()
{
    return m_savingBox->runtimeParameterBox()->sensorOrderMethod();
}

void SensorListModel::setOrderMethod(MeshRuntimeParameterBox::SensorOrderMethod orderMethod)
{
    if(m_savingBox->runtimeParameterBox()->sensorOrderMethod()!=orderMethod)
    {
        m_savingBox->runtimeParameterBox()->setSensorOrderMethod(orderMethod);
        refreshList();
    }
}

bool SensorListModel::onlyShowAvailableNodes()
{
    return m_savingBox->runtimeParameterBox()->sensorListOnlyShowAvailableNodes();
}

void SensorListModel::setOnlyShowAvailableNodes(bool onlyShowAvailable)
{
    if(m_savingBox->runtimeParameterBox()->sensorListOnlyShowAvailableNodes()!=onlyShowAvailable)
    {
        m_savingBox->runtimeParameterBox()->setSensorListOnlyShowAvailableNodes(onlyShowAvailable);
        refreshList();
    }
}

int SensorListModel::count()
{
    return m_sensorList.size();
}

Sensor *SensorListModel::at(int index)
{
    if(m_sensorList.size()==0)
    {
        return NULL;
    }
    else if(index<m_sensorList.size())
    {
        return m_sensorList.at(index);
    }
    else
    {
        return NULL;
    }
}


double SensorListModel::scale()
{
    return m_savingBox->runtimeParameterBox()->sensorListViewScaleValue();
}

void SensorListModel::setScale(double scale)
{
    if(m_savingBox->runtimeParameterBox()->sensorListViewScaleValue()!=scale)
    {
        m_savingBox->runtimeParameterBox()->setSensorListViewScaleValue(scale);
        emit modelChanged();
    }
}

void SensorListModel::scaleUp()
{
    double tmpScale=scale()+0.1;
    if(tmpScale>1.0)
    {
        tmpScale=1.0;
    }
    setScale(tmpScale);
}

void SensorListModel::scaleDown()
{
    double tmpScale=scale()-0.1;
    if(tmpScale<0.1)
    {
        tmpScale=0.1;
    }
    setScale(tmpScale);
}

void SensorListModel::doSensorsModelDataChanged()
{
    emit dataChanged();
}

void SensorListModel::doSensorsModelModelChanged()
{
    refreshList();
}

void SensorListModel::refreshList()
{
    m_sensorList.clear();

    int sensorsNumber=m_sensorsModel->count();
    for(int i=0; i<sensorsNumber; i++)
    {
        Sensor *sensor=m_sensorsModel->at(i);
        if(m_savingBox->runtimeParameterBox()->sensorListOnlyShowAvailableNodes() && (!sensor->isTouched()))
        {
            continue;
        }
        m_sensorList.push_back(sensor);
    }

    if(m_savingBox->runtimeParameterBox()->sensorOrderMethod()==MeshRuntimeParameterBox::SensorOrderByNodeID)
    {
        qSort(this->m_sensorList.begin(), this->m_sensorList.end(), sensorIDLessThan);
        for(int i=0; i<m_sensorList.size(); i++)
        {
            qDebug()<<m_sensorList.at(i)->getName();
        }
    }
    else if(m_savingBox->runtimeParameterBox()->sensorOrderMethod()==MeshRuntimeParameterBox::SensorOrderByNodeName)
    {
        qSort(this->m_sensorList.begin(), this->m_sensorList.end(), sensorNameLessThan);
        for(int i=0; i<m_sensorList.size(); i++)
        {
            qDebug()<<m_sensorList.at(i)->getName();
        }
    }

    emit modelChanged();
}

