#include "sensorlistviewmodel.h"

#include "domain/luxsensor.h"
#include "domain/realmotionsensor.h"
#include "domain/virtualmotionsensor.h"
#include "domain/co1sensor.h"
#include "domain/co2sensor.h"
#include "domain/pmsensor.h"
#include "domain/virtualswitchsensor.h"
#include "domain/realmotionandluxcamerasensor.h"

#include <QDebug>
#include <QPixmap>
#include "meshmodel.h"
#include "sensorlistmodel.h"

#include "parsermanager.h"
#include "parser/sensorparser.h"
#include "widget/sensorframe.h"

SensorListViewModel::SensorListViewModel(MeshModel *meshModel, SensorsModel *sensorsModel, QObject *parent)
    : QAbstractListModel(parent)
{
    this->sensorsModel=sensorsModel;
    this->m_meshModel=meshModel;
    m_sensorListModel=new SensorListModel(sensorsModel, meshModel->savingBox());
    connect(sensorsModel, SIGNAL(dataChanged()), m_sensorListModel, SLOT(doSensorsModelDataChanged()));
    connect(sensorsModel, SIGNAL(modelChanged()), m_sensorListModel, SLOT(doSensorsModelModelChanged()));
}

SensorListViewModel::~SensorListViewModel()
{
    if(m_sensorListModel!=NULL)
    {
        delete m_sensorListModel;
        m_sensorListModel=NULL;
    }
}

int SensorListViewModel::rowCount(const QModelIndex &parent) const
{
    return m_sensorListModel->count();
}

int SensorListViewModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant SensorListViewModel::data(const QModelIndex &index, int role) const
{
    Sensor *sensor=m_sensorListModel->at(index.row());
    if(sensor==NULL)
    {
        qDebug()<<"SensorListViewModel::data NULL";
        return QVariant();
    }
    else
    {
        switch (role) {
        case Qt::SizeHintRole:
            return QSize((int)(110*m_sensorListModel->scale()), (int)(150*m_sensorListModel->scale()));
            break;
        case Qt::UserRole:
            return sensor->sensorId();
            break;
        case Qt::DecorationRole:
        {
            SensorParser *sensorParser=ParserManager::instance()->getSensorParserByType(sensor->sensorType());
            if(sensorParser!=NULL)
            {
                QPixmap pix=sensorParser->getSurfaceWithTouchInfo(m_meshModel, sensor);
                QPixmap scaledPix=pix.scaled((int)(110*m_sensorListModel->scale()), (int)(150*m_sensorListModel->scale()));
                return scaledPix;
            }
        }
            break;
        case Qt::ToolTipRole:
        {
            SensorParser *sensorParser=ParserManager::instance()->getSensorParserByType(sensor->sensorType());
            if(sensorParser)
            {
                return sensorParser->getSensorTooltipInSensorListView(sensor);
            }
            else
            {
                return QVariant();
            }
        }
            break;
        default:
            return QVariant();
        }
    }
    return QVariant();
}

SensorListModel *SensorListViewModel::sensorListModel()
{
    return this->m_sensorListModel;
}

