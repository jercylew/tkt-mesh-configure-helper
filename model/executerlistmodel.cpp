#include "executerlistmodel.h"

#include "executersmodel.h"
#include "utils/meshsavingbox.h"
#include <QDebug>
#include "sensorsmodel.h"
#include "utils/meshruntimeparameterbox.h"

bool executerIDLessThan(Executer *lum1, Executer *lum2)
{
    return lum1->getBluetoothAddress() < lum2->getBluetoothAddress();
}

bool executerNameLessThan(Executer *lum1, Executer *lum2)
{
    return lum1->getName() < lum2->getName();
}

bool executerOnlineStatusLessThan(Executer *lum1, Executer *lum2)
{
    if(lum1->isOnline() && (!lum2->isOnline()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

ExecuterListModel::ExecuterListModel(ExecutersModel *executersModel, SensorsModel *sensorsModel, MeshSavingBox *savingBox, QObject *parent)
    : QObject(parent),
      m_executersModel(executersModel),
      m_sensorsModel(sensorsModel),
      m_savingBox(savingBox)
{
}

void ExecuterListModel::init()
{
    refreshList();
}

MeshRuntimeParameterBox::ExecuterOrderMethod ExecuterListModel::orderMethod()
{
    return m_savingBox->runtimeParameterBox()->executerOrderMethod();
}

void ExecuterListModel::setOrderMethod(MeshRuntimeParameterBox::ExecuterOrderMethod orderMethod)
{
    if(m_savingBox->runtimeParameterBox()->executerOrderMethod()!=orderMethod)
    {
        m_savingBox->runtimeParameterBox()->setExecuterOrderMethod(orderMethod);
        refreshList();
    }
}

bool ExecuterListModel::onlyShowOnlineNodes()
{
    return m_savingBox->runtimeParameterBox()->executerListOnlyShowOnlineNodes();
}

void ExecuterListModel::setOnlyShowOnlineNodes(bool onlyShowOnline)
{
    if(m_savingBox->runtimeParameterBox()->executerListOnlyShowOnlineNodes()!=onlyShowOnline)
    {
        m_savingBox->runtimeParameterBox()->setExecuterListOnlyShowOnlineNodes(onlyShowOnline);
        refreshList();
    }
}

bool ExecuterListModel::onlyShowAvailableNodes()
{
    return m_savingBox->runtimeParameterBox()->executerListOnlyShowAvailableNodes();
}

void ExecuterListModel::setOnlyShowAvailableNodes(bool onlyShowAvailable)
{
    if(m_savingBox->runtimeParameterBox()->executerListOnlyShowAvailableNodes()!=onlyShowAvailable)
    {
        m_savingBox->runtimeParameterBox()->setExecuterListOnlyShowAvailableNodes(onlyShowAvailable);
        refreshList();
    }
}

quint8 ExecuterListModel::selectedGroupId()
{
    return m_savingBox->runtimeParameterBox()->executerListViewCurrentGroupId();
}

void ExecuterListModel::setSelectedGroupId(quint8 selectedGroupId)
{
    if(m_savingBox->runtimeParameterBox()->executerListViewCurrentGroupId()!=selectedGroupId)
    {
        m_savingBox->runtimeParameterBox()->setExecuterListViewCurrentGroupId(selectedGroupId);
        refreshList();
    }
}

int ExecuterListModel::count()
{
    return m_executerList.size();
}

Executer *ExecuterListModel::at(int index)
{
    if(m_executerList.size()==0)
    {
        return NULL;
    }
    else if(index<m_executerList.size())
    {
        return m_executerList.at(index);
    }
    else
    {
        return NULL;
    }
}

double ExecuterListModel::scale()
{
    return m_savingBox->runtimeParameterBox()->executerListViewScaleValue();
}

void ExecuterListModel::setScale(double scale)
{
    if(m_savingBox->runtimeParameterBox()->executerListViewScaleValue()!=scale)
    {
        m_savingBox->runtimeParameterBox()->setExecuterListViewScaleValue(scale);
        emit modelChanged();
    }
}

void ExecuterListModel::scaleUp()
{
    double tmpScale=scale()+0.1;
    if(tmpScale>1.0)
    {
        tmpScale=1.0;
    }
    setScale(tmpScale);
}

void ExecuterListModel::scaleDown()
{
    double tmpScale=scale()-0.1;
    if(tmpScale<0.1)
    {
        tmpScale=0.1;
    }
    setScale(tmpScale);
}

void ExecuterListModel::doExecutersModelDataChanged()
{
    emit dataChanged();
}

void ExecuterListModel::doExecutersModelModelChanged()
{
    refreshList();
}

void ExecuterListModel::refreshList()
{
    m_executerList.clear();
    QList<Executer*> tmpAllList=m_executersModel->getAll();
    QList<Executer*> tmpShowCheckedList;

    int tmpAllListSize=tmpAllList.size();
    for(int i=0; i<tmpAllListSize; i++)
    {
        Executer *lum=tmpAllList.at(i);
        if(m_savingBox->runtimeParameterBox()->executerListOnlyShowOnlineNodes() && (!lum->isOnline()))
        {
            continue;
        }
        if(m_savingBox->runtimeParameterBox()->executerListOnlyShowAvailableNodes() && (!lum->isTouched()))
        {
            continue;
        }

        tmpShowCheckedList.push_back(lum);
    }

    int tmpShowCheckedListSize=tmpShowCheckedList.size();
    if(m_savingBox->runtimeParameterBox()->executerListViewCurrentGroupId()==0)
    {
        m_executerList=tmpShowCheckedList;
    }
    else if((m_savingBox->runtimeParameterBox()->executerListViewCurrentGroupId()&0xFF)==255)
    {
        for(int i=0; i<tmpShowCheckedListSize; i++)
        {
            Executer *lum=tmpShowCheckedList.at(i);
            if(lum->groupListSize()==0)
            {
                m_executerList.push_back(lum);
            }
        }
    }
    else
    {
        for(int i=0; i<tmpShowCheckedListSize; i++)
        {
            Executer *lum=tmpShowCheckedList.at(i);
            if(lum->belongsToGroup(m_savingBox->runtimeParameterBox()->executerListViewCurrentGroupId()))
            {
                m_executerList.push_back(lum);
            }
        }
    }

    if(m_savingBox->runtimeParameterBox()->executerOrderMethod()==MeshRuntimeParameterBox::ExecuterOrderByNodeID)
    {
        qSort(this->m_executerList.begin(), this->m_executerList.end(), executerIDLessThan);
    }
    else if(m_savingBox->runtimeParameterBox()->executerOrderMethod()==MeshRuntimeParameterBox::ExecuterOrderByNodeName)
    {
        qSort(this->m_executerList.begin(), this->m_executerList.end(), executerNameLessThan);
    }
    else if(m_savingBox->runtimeParameterBox()->executerOrderMethod()==MeshRuntimeParameterBox::ExecuterOrderByOnlineStatus)
    {
        qSort(this->m_executerList.begin(), this->m_executerList.end(), executerOnlineStatusLessThan);
    }

    emit modelChanged();
}

