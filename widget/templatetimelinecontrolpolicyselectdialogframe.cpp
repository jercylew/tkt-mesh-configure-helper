#include "templatetimelinecontrolpolicyselectdialogframe.h"
#include "ui_templatetimelinecontrolpolicyselectdialogframe.h"

#include "model/meshmodel.h"
#include "db/meshdbcaller.h"
#include "db/meshdbmanager.h"
#include "dialogbuilder.h"
#include <QDebug>

TemplateTimeLineControlPolicySelectDialogFrame::TemplateTimeLineControlPolicySelectDialogFrame(MeshModel *meshModel,
                                                  NodeGroup *group, int subId, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TemplateTimeLineControlPolicySelectDialogFrame)
{
    ui->setupUi(this);
    m_meshModel = meshModel;
    m_group = group;
    m_subId = subId;
    m_selectedTemplateTimeLineControlItemListModel=NULL;
    m_applyMovie=NULL;

    m_templateTimeLineControlPolicyListEditModel=new TemplateTimeLineControlPolicyListEditModel(m_meshModel, m_group, this);
    ui->tableItemCollection->setModel(m_templateTimeLineControlPolicyListEditModel->templateTimeLineControlPolicyTableViewModel());

    ui->tableItemCollection->setEditTriggers(QTableView::DoubleClicked);
    ui->tableItemCollection->verticalHeader()->setVisible( false );
    ui->tableItemCollection->horizontalHeader()->setStretchLastSection( true );
    ui->tableItemCollection->setSelectionMode(QTableView::SingleSelection);
    ui->tableItemCollection->setSelectionBehavior(QTableView::SelectRows);
    ui->tableItemCollection->setColumnWidth(1, 150);

    m_templateTimeLineControlPolicyListEditModel->templateTimeLineControlPolicyTableViewModel()->templateTimeLineControlPolicyListModel()->init();
    ui->stackedWidget->setCurrentWidget(ui->page1);

    ui->buttonAdd->setToolTip(tr("Add"));
    ui->buttonEdit->setToolTip(tr("Edit"));
    ui->buttonRemove->setToolTip(tr("Remove"));
    ui->buttonAdd->setCursor(Qt::PointingHandCursor);
    ui->buttonEdit->setCursor(Qt::PointingHandCursor);
    ui->buttonRemove->setCursor(Qt::PointingHandCursor);
    ui->buttonOk->setCursor(Qt::PointingHandCursor);
    ui->buttonCancel->setCursor(Qt::PointingHandCursor);
}

TemplateTimeLineControlPolicySelectDialogFrame::~TemplateTimeLineControlPolicySelectDialogFrame()
{
    if(m_selectedTemplateTimeLineControlItemListModel!=NULL)
    {
        m_selectedTemplateTimeLineControlItemListModel->deleteLater();
        m_selectedTemplateTimeLineControlItemListModel=NULL;
    }
    if(m_applyMovie!=NULL)
    {
        m_applyMovie->deleteLater();
        m_applyMovie=NULL;
    }
    delete ui;
}

void TemplateTimeLineControlPolicySelectDialogFrame::on_buttonDialogBottomClose_clicked()
{
    emit reject();
}

void TemplateTimeLineControlPolicySelectDialogFrame::on_buttonAdd_clicked()
{
    QString name;
    QString description;
    QDialog *dialog=DialogBuilder::getInstance()->buildAddTemplateTimeLineControlPolicyDialog(tr("TemplateTimeLineControlPolicyListDialogFrame.AddTemplate"), &name, &description);
    if(dialog->exec()==QDialog::Rejected)
    {
        return;
    }
    m_templateTimeLineControlPolicyListEditModel->addTemplateTimeLineControlPolicy(name, description);
}

void TemplateTimeLineControlPolicySelectDialogFrame::on_buttonRemove_clicked()
{
    QModelIndex index=ui->tableItemCollection->selectionModel()->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    QDialog *dialog=DialogBuilder::getInstance()->buildQuestionDialog(tr("TemplateTimeLineControlPolicyListDialogFrame.RemoveTemplate"),
                                                                      tr("TemplateTimeLineControlPolicyListDialogFrame.RemoveTemplateQuestion").arg(index.data(Qt::DisplayRole).toString()));
    if(dialog->exec()==QDialog::Rejected)
    {
        return;
    }
    QString collectionId=index.data(Qt::UserRole).toString();
    qDebug()<<"AAAAAAAAAAAAAAAAAA id="<<collectionId;
    m_templateTimeLineControlPolicyListEditModel->removeTemplateTimeLineControlPolicy(index.data(Qt::UserRole).toString());
}

void TemplateTimeLineControlPolicySelectDialogFrame::on_buttonEdit_clicked()
{
    QModelIndex index=ui->tableItemCollection->selectionModel()->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    QString id=index.data(Qt::UserRole).toString();
    TemplateTimeLineControlPolicy *itemCollection=m_templateTimeLineControlPolicyListEditModel->templateTimeLineControlPolicyTableViewModel()->templateTimeLineControlPolicyListModel()->getById(id);
    if(itemCollection==NULL)
    {
        return;
    }
    QString name=itemCollection->name;
    QString description=itemCollection->description;
    QDialog *dialog=DialogBuilder::getInstance()->buildAddTemplateTimeLineControlPolicyDialog(tr("TemplateTimeLineControlPolicyListDialogFrame.EditTemplate"), &name, &description);
    if(dialog->exec()==QDialog::Rejected)
    {
        return;
    }
    TemplateTimeLineControlPolicy *collection=new TemplateTimeLineControlPolicy;
    collection->id=id;
    collection->name=name;
    collection->description=description;
    collection->groupType = NodeGroup::getTextByGroupType(m_group->getGroupType());
    m_templateTimeLineControlPolicyListEditModel->updateTemplateTimeLineControlPolicy(collection);
}

void TemplateTimeLineControlPolicySelectDialogFrame::on_tableItemCollection_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
    {
        return;
    }
    QString id=index.data(Qt::UserRole).toString();
    TemplateTimeLineControlPolicy *itemCollection=m_templateTimeLineControlPolicyListEditModel->templateTimeLineControlPolicyTableViewModel()->templateTimeLineControlPolicyListModel()->getById(id);
    if(itemCollection==NULL)
    {
        return;
    }
    QDialog *dialog=DialogBuilder::getInstance()->buildTemplateControlItemsEditDialog(m_meshModel, m_group, itemCollection->id, tr("Setup Template '%1'").arg(itemCollection->name));
    dialog->exec();
}

void TemplateTimeLineControlPolicySelectDialogFrame::on_buttonOk_clicked()
{
    QModelIndex index=ui->tableItemCollection->selectionModel()->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    QString id=index.data(Qt::UserRole).toString();
    TemplateTimeLineControlPolicy *itemCollection=m_templateTimeLineControlPolicyListEditModel->templateTimeLineControlPolicyTableViewModel()->templateTimeLineControlPolicyListModel()->getById(id);
    if(itemCollection==NULL)
    {
        return;
    }
    if(m_selectedTemplateTimeLineControlItemListModel!=NULL)
    {
        return;
    }
    m_selectedTemplateTimeLineControlItemListModel=new TemplateTimeLineControlItemListModel(m_meshModel, itemCollection->id);
    connect(m_selectedTemplateTimeLineControlItemListModel, SIGNAL(modelChanged()),
            this, SLOT(doTemplateTimeLineControlItemListModelModelChanged()));
    connect(m_meshModel->dbManager()->dbCaller(), SIGNAL(addControlItemsFromTemplateFinshed()),
            this, SLOT(doAddControlItemsFromTemplateFinished()));
    ui->buttonOk->setEnabled(false);
    ui->buttonCancel->setEnabled(false);
    if(m_applyMovie==NULL)
    {
        m_applyMovie=new QMovie(":/images/loading.gif");
        ui->labelLoading->setMovie(m_applyMovie);
        m_applyMovie->start();
    }
    ui->stackedWidget->setCurrentWidget(ui->page2);
    m_selectedTemplateTimeLineControlItemListModel->init();
}

void TemplateTimeLineControlPolicySelectDialogFrame::on_buttonCancel_clicked()
{
    emit reject();
}

//Insert chosen template into current policy of this group
void TemplateTimeLineControlPolicySelectDialogFrame::doTemplateTimeLineControlItemListModelModelChanged()
{
    if(m_selectedTemplateTimeLineControlItemListModel==NULL)
    {
        return;
    }
    QList<TimeLineControlItem*> *itemList=new QList<TimeLineControlItem*>;
    int len=m_selectedTemplateTimeLineControlItemListModel->count();
    for(int i = 0; i < len; i++)
    {
        TimeLineControlItem *templateItem = m_selectedTemplateTimeLineControlItemListModel->at(i);
        TimeLineControlItem *newItem = templateItem->copyAll();
        itemList->push_back(newItem);
    }

    //groupId and subId needed
    m_meshModel->dbManager()->dbCaller()->addControlItemsFromTemplate(m_group, m_subId, itemList);
}

void TemplateTimeLineControlPolicySelectDialogFrame::doAddControlItemsFromTemplateFinished()
{
    emit accept();
}
