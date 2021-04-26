#include "templatetimelinecontrolpolicylistdialogframe.h"
#include "ui_templatetimelinecontrolpolicylistdialogframe.h"
#include "dialogbuilder.h"
#include "model/meshmodel.h"
#include <QDebug>

TemplateTimeLineControlPolicyListDialogFrame::TemplateTimeLineControlPolicyListDialogFrame(MeshModel *meshModel, NodeGroup *group, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TemplateTimeLineControlPolicyListDialogFrame)
{
    ui->setupUi(this);
    m_meshModel = meshModel;
    m_templateTimeLineControlPolicyListEditModel=new TemplateTimeLineControlPolicyListEditModel(meshModel, group, this);
    ui->tableItemCollection->setModel(this->m_templateTimeLineControlPolicyListEditModel->templateTimeLineControlPolicyTableViewModel());

    ui->tableItemCollection->setEditTriggers(QTableView::DoubleClicked);
    ui->tableItemCollection->verticalHeader()->setVisible( false );
    ui->tableItemCollection->horizontalHeader()->setStretchLastSection( true );
    ui->tableItemCollection->setSelectionMode(QTableView::SingleSelection);
    ui->tableItemCollection->setSelectionBehavior(QTableView::SelectRows);
    ui->tableItemCollection->setColumnWidth(1, 200);

    this->m_templateTimeLineControlPolicyListEditModel->templateTimeLineControlPolicyTableViewModel()->templateTimeLineControlPolicyListModel()->init();

    ui->buttonAdd->setToolTip(tr("Add"));
    ui->buttonEdit->setToolTip(tr("Edit"));
    ui->buttonRemove->setToolTip(tr("Remove"));
    ui->buttonAdd->setCursor(Qt::PointingHandCursor);
    ui->buttonEdit->setCursor(Qt::PointingHandCursor);
    ui->buttonRemove->setCursor(Qt::PointingHandCursor);
    ui->buttonDialogBottomClose->setCursor(Qt::PointingHandCursor);

    initMenuAndAction();
}

TemplateTimeLineControlPolicyListDialogFrame::~TemplateTimeLineControlPolicyListDialogFrame()
{
    delete ui;
}

void TemplateTimeLineControlPolicyListDialogFrame::on_buttonDialogBottomClose_clicked()
{
    emit reject();
}

void TemplateTimeLineControlPolicyListDialogFrame::on_buttonAdd_clicked()
{
    QString name;
    QString description;
    QDialog *dialog=DialogBuilder::getInstance()->buildAddTemplateTimeLineControlPolicyDialog(tr("Add Template"), &name, &description);
    if(dialog->exec()==QDialog::Rejected)
    {
        return;
    }
    this->m_templateTimeLineControlPolicyListEditModel->addTemplateTimeLineControlPolicy(name, description);
}

void TemplateTimeLineControlPolicyListDialogFrame::on_buttonRemove_clicked()
{
    QModelIndex index=ui->tableItemCollection->selectionModel()->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    QString id=index.data(Qt::UserRole).toString();
    TemplateTimeLineControlPolicy *itemCollection=this->m_templateTimeLineControlPolicyListEditModel->templateTimeLineControlPolicyTableViewModel()->templateTimeLineControlPolicyListModel()->getById(id);
    if(itemCollection==NULL)
    {
        return;
    }
    QDialog *dialog=DialogBuilder::getInstance()->buildQuestionDialog(tr("Remove Template"), tr("TemplateTimeLineControlPolicyListDialogFrame.RemoveTemplateQuestion").arg(itemCollection->name));
    if(dialog->exec()==QDialog::Rejected)
    {
        return;
    }
    QString collectionId=index.data(Qt::UserRole).toString();
    qDebug()<<"TemplateTimeLineControlPolicyListDialogFrame::on_buttonRemove_clicked id="<<collectionId;
    this->m_templateTimeLineControlPolicyListEditModel->removeTemplateTimeLineControlPolicy(index.data(Qt::UserRole).toString());
}

void TemplateTimeLineControlPolicyListDialogFrame::on_buttonEdit_clicked()
{
    QModelIndex index=ui->tableItemCollection->selectionModel()->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    QString id=index.data(Qt::UserRole).toString();
    TemplateTimeLineControlPolicy *itemCollection=this->m_templateTimeLineControlPolicyListEditModel->templateTimeLineControlPolicyTableViewModel()->templateTimeLineControlPolicyListModel()->getById(id);
    if(itemCollection==NULL)
    {
        return;
    }
    QString name=itemCollection->name;
    QString description=itemCollection->description;
    QDialog *dialog=DialogBuilder::getInstance()->buildAddTemplateTimeLineControlPolicyDialog(tr("Edit Template"), &name, &description);
    if(dialog->exec()==QDialog::Rejected)
    {
        return;
    }
    TemplateTimeLineControlPolicy *collection=new TemplateTimeLineControlPolicy;
    collection->id=id;
    collection->name=name;
    collection->description=description;
    this->m_templateTimeLineControlPolicyListEditModel->updateTemplateTimeLineControlPolicy(collection);
}

void TemplateTimeLineControlPolicyListDialogFrame::on_tableItemCollection_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
    {
        return;
    }
    QString id=index.data(Qt::UserRole).toString();
    TemplateTimeLineControlPolicy *itemCollection=this->m_templateTimeLineControlPolicyListEditModel->templateTimeLineControlPolicyTableViewModel()->templateTimeLineControlPolicyListModel()->getById(id);
    if(itemCollection==NULL)
    {
        return;
    }
    QDialog *dialog=DialogBuilder::getInstance()->buildTemplateControlItemsEditDialog(m_meshModel, m_group, itemCollection->id, tr("Setup Template '%1'").arg(itemCollection->name));
    dialog->exec();
}

void TemplateTimeLineControlPolicyListDialogFrame::on_tableItemCollection_customContextMenuRequested(const QPoint &pos)
{
    m_menuMain->exec(ui->tableItemCollection->mapToGlobal(pos));
}

void TemplateTimeLineControlPolicyListDialogFrame::initMenuAndAction()
{
    ui->tableItemCollection->setContextMenuPolicy(Qt::CustomContextMenu);

    m_menuMain=new QMenu(this);
    m_actionAdd=new QAction(tr("Add"), this);
    connect(m_actionAdd, SIGNAL(triggered(bool)), ui->buttonAdd, SLOT(click()));
    m_menuMain->addAction(m_actionAdd);
    m_actionEdit=new QAction(tr("Edit"), this);
    connect(m_actionEdit, SIGNAL(triggered(bool)), ui->buttonEdit, SLOT(click()));
    m_menuMain->addAction(m_actionEdit);
    m_actionRemove=new QAction(tr("Remove"), this);
    connect(m_actionRemove, SIGNAL(triggered(bool)), ui->buttonRemove, SLOT(click()));
    m_menuMain->addAction(m_actionRemove);
}
