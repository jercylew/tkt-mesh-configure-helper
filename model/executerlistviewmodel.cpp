#include "executerlistviewmodel.h"

#include <QSize>
#include <QPixmap>
#include <QDebug>
#include "executerlistmodel.h"
#include "meshmodel.h"
#include "nodegroupsmodel.h"
#include "parser/executerparser.h"
#include "parsermanager.h"

ExecuterListViewModel::ExecuterListViewModel(MeshModel *meshModel, ExecutersModel *executersModel, QObject *parent)
    : QAbstractListModel(parent)
{
    this->executersModel=executersModel;
    this->m_meshModel=meshModel;
    m_executerListModel=new ExecuterListModel(executersModel, meshModel->sensorsModel(), meshModel->savingBox());
    connect(executersModel, SIGNAL(dataChanged()), m_executerListModel, SLOT(doExecutersModelDataChanged()));
    connect(executersModel, SIGNAL(modelChanged()), m_executerListModel, SLOT(doExecutersModelModelChanged()));
    connect(m_executerListModel, SIGNAL(modelChanged()), this, SLOT(doListModelChanged()));
    connect(m_executerListModel, SIGNAL(dataChanged()), this, SLOT(doListDataChanged()));
}

ExecuterListViewModel::~ExecuterListViewModel()
{
    if(m_executerListModel!=NULL)
    {
        delete m_executerListModel;
        m_executerListModel=NULL;
    }
}

int ExecuterListViewModel::rowCount(const QModelIndex &parent) const
{
    return m_executerListModel->count();
}

int ExecuterListViewModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ExecuterListViewModel::data(const QModelIndex &index, int role) const
{
    Executer *executer=m_executerListModel->at(index.row());
    if(executer==NULL)
    {
        return QVariant();
    }
    ExecuterParser *parser=ParserManager::instance()->getExecuterParserByTypeText(executer->typeText());
    if(parser==NULL)
    {
        return QVariant();
    }

    switch (role) {
        case Qt::SizeHintRole:
        {
            return QSize((int)(110*m_executerListModel->scale()), (int)(140*m_executerListModel->scale()));
        }
        case Qt::UserRole:
            return executer->executerId();
        case Qt::DecorationRole:
        {
            QPixmap pix=parser->getSurfaceWithTouchInfo(m_meshModel, executer);
            QPixmap scaledPix=pix.scaled((int)(110*m_executerListModel->scale()), (int)(140*m_executerListModel->scale()));
            return scaledPix;
        }
        case Qt::ToolTipRole:
        {
            return parser->getExecuterTooltipInExecuterListView(executer);
        }
        default:
            return QVariant();
    }

    return QVariant();
}

ExecuterListModel *ExecuterListViewModel::executerListModel()
{
    return this->m_executerListModel;
}

void ExecuterListViewModel::doListModelChanged()
{
    emit layoutChanged();
}

void ExecuterListViewModel::doListDataChanged()
{
    int row=m_executerListModel->count();
    emit dataChanged(this->index(0), this->index(row-1));
}
