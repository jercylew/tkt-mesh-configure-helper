#include "sensoridcombox.h"

#include "model/meshmodel.h"
#include "model/sensorsmodel.h"

SensorIdCombox::SensorIdCombox(QWidget *parent)
    : QComboBox(parent)
{
    m_meshModel=NULL;
    this->setEditable(true);
    connect(this, SIGNAL(activated(int)), this, SLOT(doActivate(int)));
    connect(this, SIGNAL(highlighted(int)), this, SLOT(doHighlight(int)));
}

void SensorIdCombox::setText(const QString &text)
{
    this->setEditText(text);
}

QString SensorIdCombox::text()
{
    return this->currentText();
}

void SensorIdCombox::setMeshModel(MeshModel *meshModel)
{
    this->m_meshModel=meshModel;
}

void SensorIdCombox::focusOutEvent(QFocusEvent *e)
{
    QComboBox::focusOutEvent(e);
    emit editingFinished();
}

void SensorIdCombox::showPopup()
{
    m_sensorIdBeforePopup=this->currentText();
    updateSensorList();
    QComboBox::showPopup();
}

void SensorIdCombox::addAccessSensorType(Sensor::SensorType sensorType)
{
    m_accessSensorTypeSet.insert(sensorType);
}

void SensorIdCombox::updateSensorList()
{
    this->clear();
    if(m_meshModel==NULL)
    {
        return;
    }
    QList<QString> sensorIdList=getFilteredAndSortedSensorIdList();
    for(int i=0; i<sensorIdList.size(); i++)
    {
        Sensor *sensor=m_meshModel->sensorsModel()->getSensorBySensorId(sensorIdList.at(i));
        if(sensor)
        {
            this->addItem(QString("[%1] %2").arg(sensor->sensorId()).arg(sensor->getName()), sensor->sensorId());
        }
    }
}

QList<QString> SensorIdCombox::getFilteredAndSortedSensorIdList()
{
    int count=m_meshModel->sensorsModel()->count();
    QList<QString> sensorIdList;
    for(int i=0; i<count; i++)
    {
        Sensor *sensor=m_meshModel->sensorsModel()->at(i);
        if(m_accessSensorTypeSet.isEmpty() || m_accessSensorTypeSet.contains(sensor->sensorType()))
        {
            sensorIdList.push_back(sensor->sensorId());
        }
    }
    qSort(sensorIdList.begin(), sensorIdList.end());
    return sensorIdList;
}

void SensorIdCombox::doActivate(int index)
{
    this->setCurrentText(this->itemData(index).toString());
}

void SensorIdCombox::doHighlight(int index)
{
    this->setCurrentText(this->m_sensorIdBeforePopup);
}
