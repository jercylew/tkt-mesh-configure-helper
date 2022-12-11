#include "meshdevicelistframe.h"
#include "ui_meshdevicelistframe.h"

#include <QClipboard>
#include <QStyleFactory>

#include "model/tktmeshmodel.h"
#include "model/meshnodesmodel.h"
#include "model/meshnodetableviewmodel.h"

#include "callback/refreshmeshnodescallback.h"
#include "utils/tktmeshconnectionholder.h"
#include "utils/tktmeshapiworkercontroller.h"
#include "callback/getmeshallnodescallback.h"
#include "arthurstyle.h"

MeshDeviceListFrame::MeshDeviceListFrame(TKTMeshModel *tktMeshModel, const QString &meshUUID, AbstractCallbackAlerter *alerter, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MeshDeviceListFrame)
{
    ui->setupUi(this);
    this->m_tktMeshModel=tktMeshModel;
    this->m_meshUUID=meshUUID;
    this->m_alerter=alerter;

    m_meshNodesModel=new MeshNodesModel(this);
    MeshNodeTableViewModel *m_meshNodeTableViewModel=new MeshNodeTableViewModel(m_meshNodesModel);
    ui->tableMeshNode->setModel(m_meshNodeTableViewModel);
    ui->tableMeshNode->setEditTriggers(QTableView::NoEditTriggers);
    ui->tableMeshNode->verticalHeader()->setVisible( true );
    ui->tableMeshNode->horizontalHeader()->setStretchLastSection( true );
    ui->tableMeshNode->setSelectionMode(QTableView::ExtendedSelection);
    ui->tableMeshNode->setSelectionBehavior(QTableView::SelectItems);

    ui->buttonMeshClose->setCursor(Qt::PointingHandCursor);

    ui->tableMeshNode->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableMeshNode->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->tableMeshNode->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableMeshNode->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    ui->tableMeshNode->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Interactive);
    ui->tableMeshNode->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->tableMeshNode->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);

    initMenuAndAction();
}

MeshDeviceListFrame::~MeshDeviceListFrame()
{
    delete ui;
}

void MeshDeviceListFrame::updateMeshNodes(QList<MeshDeviceNode *> *meshNodeList)
{
    if(m_meshNodesModel!=NULL)
    {
        m_meshNodesModel->updateNodes(*meshNodeList);
        ui->labelMeshNodeNumber->setText(QString::number(meshNodeList->size()));
    }
}

void MeshDeviceListFrame::on_buttonRefresh_clicked()
{
    if(m_tktMeshModel==NULL)
    {
        return;
    }
    RefreshMeshNodesCallback *callback=new RefreshMeshNodesCallback(this, this->m_alerter, m_tktMeshModel);
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "refresh_mesh_nodes");
    asyncTask->requestJson()->insert("data", this->m_meshUUID);
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void MeshDeviceListFrame::on_buttonGetAllNode_clicked()
{
    if(m_tktMeshModel==NULL)
    {
        return;
    }
    GetMeshAllNodesCallback *callback=new GetMeshAllNodesCallback(this, this->m_alerter, m_tktMeshModel);
    AsyncApiRequestTask *asyncTask=new AsyncApiRequestTask(callback);
    asyncTask->requestJson()->insert("topic", "request");
    asyncTask->requestJson()->insert("command", "get_mesh_all_nodes");
    asyncTask->requestJson()->insert("data", this->m_meshUUID);
    m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->runAsyncTask(asyncTask);
}

void MeshDeviceListFrame::on_tableMeshNode_customContextMenuRequested(const QPoint &pos)
{
    m_menu->exec(ui->tableMeshNode->mapToGlobal(pos));
}

void MeshDeviceListFrame::on_buttonMeshClose_clicked()
{
    emit backToTKTMeshPage();
}

void MeshDeviceListFrame::initMenuAndAction()
{
    m_menu = new QMenu(this);

    m_actionCopy = new QAction(tr("Copy"));
    connect(m_actionCopy, SIGNAL(triggered(bool)), this, SLOT(doActionCopy()));
    m_menu->addAction(m_actionCopy);
//    ui->tableMeshNode->setStyle(QStyleFactory::create("Fusion"));
    m_menu->setStyle(QStyleFactory::create("Fusion"));
}

void MeshDeviceListFrame::doActionCopy()
{
    QAbstractItemModel * model = ui->tableMeshNode->model();
    QItemSelectionModel * selection = ui->tableMeshNode->selectionModel();
    QModelIndexList indexes = selection->selectedIndexes();

    QString selected_text;
    bool bIsFirstItem = true;
    QModelIndex firstItemIndex = indexes.first();
    int preRow = firstItemIndex.row();

    foreach(const QModelIndex &current, indexes)
    {
        if (current.row() != preRow)
        {
            selected_text.append('\n');
        }
        else
        {
            if (bIsFirstItem)
            {
                bIsFirstItem = false;
            }
            else
            {
                selected_text.append('\t');
            }
        }

        QVariant data = model->data(current);
        QString text = data.toString();
        selected_text.append(text);
        preRow = current.row();
    }

    qApp->clipboard()->setText(selected_text);
}
