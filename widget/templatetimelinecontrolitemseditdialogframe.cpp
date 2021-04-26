#include "templatetimelinecontrolitemseditdialogframe.h"
#include "ui_templatetimelinecontrolitemseditdialogframe.h"
#include "dialogbuilder.h"
#include "model/meshesmodel.h"
#include "objectfactory.h"
#include <QDebug>

TemplateTimeLineControlItemsEditDialogFrame::TemplateTimeLineControlItemsEditDialogFrame(MeshModel *meshModel, NodeGroup *group,
                                                                                         const QString &policyId, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TemplateTimeLineControlItemsEditDialogFrame)
{
    ui->setupUi(this);
    m_policyId=policyId;
    m_group = group;
    qDebug()<<"TemplateTimeLineControlItemsEditDialogFrame::TemplateTimeLineControlItemsEditDialogFrame, "<<QString("policyId=%1").arg(policyId);

    m_meshModel = meshModel;
    m_templateTimeLineControlItemListEditModel=new TemplateTimeLineControlItemListEditModel(m_meshModel, m_group, policyId, this);
    ui->tableView->setModel(m_templateTimeLineControlItemListEditModel->templateTimeLineControlItemTableViewModel());

    ui->tableView->setEditTriggers(QTableView::DoubleClicked);
    ui->tableView->verticalHeader()->setVisible( false );
    ui->tableView->horizontalHeader()->setStretchLastSection( true );
    ui->tableView->setSelectionMode(QTableView::SingleSelection);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);

    qDebug()<<"TemplateTimeLineControlItemsEditDialogFrame::TemplateTimeLineControlItemsEditDialogFrame !!!";

    updateGraph();

    connect(m_templateTimeLineControlItemListEditModel->templateTimeLineControlItemTableViewModel()->templateTimeLineControlItemListModel(), SIGNAL(dataChanged()),
            this, SLOT(updateGraph()));
    connect(m_templateTimeLineControlItemListEditModel->templateTimeLineControlItemTableViewModel()->templateTimeLineControlItemListModel(), SIGNAL(modelChanged()),
            this, SLOT(updateGraph()));

    m_templateTimeLineControlItemListEditModel->templateTimeLineControlItemTableViewModel()->templateTimeLineControlItemListModel()->init();

    ui->buttonAdd->setToolTip(tr("Add"));
    ui->buttonRemove->setToolTip(tr("Remove"));
    ui->buttonAdd->setCursor(Qt::PointingHandCursor);
    ui->buttonRemove->setCursor(Qt::PointingHandCursor);
    ui->buttonDialogBottomClose->setCursor(Qt::PointingHandCursor);

    initMenuAndAction();
}

TemplateTimeLineControlItemsEditDialogFrame::~TemplateTimeLineControlItemsEditDialogFrame()
{
    delete ui;
}

void TemplateTimeLineControlItemsEditDialogFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void TemplateTimeLineControlItemsEditDialogFrame::on_buttonAdd_clicked()
{
    int minutes=0;
    QDialog *dialog=DialogBuilder::getInstance()->buildAddTimePointDialog(tr("Add Time Point"), &minutes);
    if(dialog->exec()==QDialog::Rejected)
    {
        return;
    }
    m_templateTimeLineControlItemListEditModel->addTemplateControlItem(m_policyId, minutes);
}

void TemplateTimeLineControlItemsEditDialogFrame::on_buttonRemove_clicked()
{
    QModelIndex index=ui->tableView->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    TimeLineControlItem *item=m_templateTimeLineControlItemListEditModel->templateTimeLineControlItemTableViewModel()->templateTimeLineControlItemListModel()->getById(index.data(Qt::UserRole).toString());
    if(item==NULL)
    {
        return;
    }

    QDialog *dialog=DialogBuilder::getInstance()->buildQuestionDialog(tr("Remove Time Point"),
                                                                      tr("Are you sure remove time point '%1:%2'?").arg(item->startMinute/60).arg(item->startMinute%60));
    if(dialog->exec()==QDialog::Accepted)
    {
        m_templateTimeLineControlItemListEditModel->removeTemplateControlItem(item->templatePolicyId, item->templateItemId);
    }
}

void TemplateTimeLineControlItemsEditDialogFrame::on_tableView_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
    {
        return;
    }
    TimeLineControlItem *origItem=m_templateTimeLineControlItemListEditModel->templateTimeLineControlItemTableViewModel()->templateTimeLineControlItemListModel()->getById(index.data(Qt::UserRole).toString());
    if(origItem == NULL)
    {
        return;
    }

    TimeLineControlItem *newItem = origItem->copyAll();
    QDialog *dialog = DialogBuilder::getInstance()->buildTemplateControlItemEditDialog(m_meshModel, m_group, newItem,
                                                                                     tr("Setup Template Control Item '%1:%2'").arg(newItem->startMinute/60).arg(newItem->startMinute%60));
    if(dialog->exec() == QDialog::Accepted)
    {
        m_templateTimeLineControlItemListEditModel->updateTemplateControlItem(newItem);
    }
}

void TemplateTimeLineControlItemsEditDialogFrame::updateGraph()
{
    QList<TimeLineControlItem*> itemList = m_templateTimeLineControlItemListEditModel->templateTimeLineControlItemTableViewModel()->templateTimeLineControlItemListModel()->getAll();
    ui->timeLineGraphFrame->updateGraph(itemList);
}

void TemplateTimeLineControlItemsEditDialogFrame::on_buttonDialogBottomClose_clicked()
{
    emit accept();
}

void TemplateTimeLineControlItemsEditDialogFrame::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    m_menuMain->exec(ui->tableView->mapToGlobal(pos));
}

void TemplateTimeLineControlItemsEditDialogFrame::initMenuAndAction()
{
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    m_menuMain=new QMenu(this);
    m_actionAdd=new QAction(tr("Add"), this);
    connect(m_actionAdd, SIGNAL(triggered(bool)), ui->buttonAdd, SLOT(click()));
    m_menuMain->addAction(m_actionAdd);
    m_actionRemove=new QAction(tr("Remove"), this);
    connect(m_actionRemove, SIGNAL(triggered(bool)), ui->buttonRemove, SLOT(click()));
    m_menuMain->addAction(m_actionRemove);
}
