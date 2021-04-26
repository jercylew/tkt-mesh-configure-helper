#include "timelinecontrolitemseditdialogframe.h"
#include "ui_timelinecontrolitemseditdialogframe.h"
#include "utils/timelinecontrolmanager.h"
#include "delegate/controlitemtabledelegate.h"
#include <QInputDialog>
#include "widget/addtimepointdialogframe.h"
#include "db/meshdbmanager.h"
#include <QMessageBox>
#include "utils/timelinecontrolmanager.h"
#include <QDebug>
#include "timelinegraphframe.h"
#include "dialogbuilder.h"
#include "model/meshmodel.h"
#include "db/meshdbcaller.h"
#include "languagemanager.h"
#include <QColorDialog>
#include "dialogbuilder.h"

TimeLineControlItemsEditDialogFrame::TimeLineControlItemsEditDialogFrame(MeshModel *meshModel, int controlPolicyId, int policySubId, NodeGroup *group,  QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TimeLineControlItemsEditDialogFrame),
    m_meshModel(meshModel)
{
    ui->setupUi(this);
    qDebug()<<"TimeLineControlItemsEditDialog::TimeLineControlItemsEditDialog, "<<QString("groupId=%1").arg(group->getId());
    this->m_controlPolicyId=controlPolicyId;
    m_group = group;
    this->m_policySubId=policySubId;
    m_timeLineControlItemTableViewModel=new TimeLineControlItemTableViewModel(meshModel, m_group->getId(), m_policySubId);
    ui->tableView->setModel(m_timeLineControlItemTableViewModel);

    ui->tableView->setEditTriggers(QTableView::DoubleClicked);
    ui->tableView->verticalHeader()->setVisible( false );
    ui->tableView->horizontalHeader()->setStretchLastSection( true );
    ui->tableView->setSelectionMode(QTableView::SingleSelection);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);

    qDebug()<<"TimeLineControlItemsEditDialog::TimeLineControlItemsEditDialog !!!";

    updateGraph();

    connect(m_timeLineControlItemTableViewModel->timeLineControlPolicyModel(), SIGNAL(dataChanged()),
            this, SLOT(updateGraph()));
    connect(m_timeLineControlItemTableViewModel->timeLineControlPolicyModel(), SIGNAL(modelChanged()),
            this, SLOT(updateGraph()));

    m_timeLineControlItemTableViewModel->init();

    ui->labelControlPolicySubId->setText(QString::number(controlPolicyId));

    TimeLineControlPolicy *controlPolicy=m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->getById(controlPolicyId);
    if(controlPolicy!=NULL)
    {
        ui->lineControlPolicyName->setText(controlPolicy->name);
        ui->lineControlPolicyDescription->setText(controlPolicy->description);
        m_color=controlPolicy->color;
        ui->buttonColor->setStyleSheet(QString("border:1px solid grey; background-color:%1;").arg(m_color));
        if(controlPolicy->subId==0)
        {
            ui->lineControlPolicyName->setEnabled(false);
            ui->lineControlPolicyDescription->setEnabled(false);
            ui->buttonColor->setEnabled(false);
        }
        else
        {
            ui->lineControlPolicyName->setEnabled(true);
            ui->lineControlPolicyDescription->setEnabled(true);
            ui->buttonColor->setEnabled(true);
        }
    }

    ui->buttonAdd->setToolTip(tr("Global.Add"));
    ui->buttonRemove->setToolTip(tr("Global.Remove"));
    ui->buttonAdd->setCursor(Qt::PointingHandCursor);
    ui->buttonRemove->setCursor(Qt::PointingHandCursor);
    ui->buttonDialogBottomClose->setCursor(Qt::PointingHandCursor);

    initMenuAndAction();
}

TimeLineControlItemsEditDialogFrame::~TimeLineControlItemsEditDialogFrame()
{
    delete ui;
}

void TimeLineControlItemsEditDialogFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void TimeLineControlItemsEditDialogFrame::on_buttonAdd_clicked()
{
    int minutes=0;
    QDialog *dialog=DialogBuilder::getInstance()->buildAddTimePointDialog(tr("TimeLineControlItemsEditDialogFrame.AddTimePoint"), &minutes);
    if(dialog->exec()!=QDialog::Accepted)
    {
        return;
    }
    m_meshModel->timeLineControlManager()->getTimeLineControlItemsModel()->addControlItem(m_group->getId(), m_policySubId, minutes);
}

void TimeLineControlItemsEditDialogFrame::on_buttonRemove_clicked()
{
    QModelIndex index=ui->tableView->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    TimeLineControlItem *item=m_meshModel->timeLineControlManager()->getTimeLineControlItemsModel()->getById(index.data(Qt::UserRole).toInt());
    if(item==NULL)
    {
        return;
    }
    QDialog *dialog=DialogBuilder::getInstance()->buildQuestionDialog(tr("TimeLineControlItemsEditDialogFrame.RemoveTimePoint"),
                                                                      tr("TimeLineControlItemsEditDialogFrame.RemoveTimePointQuestion").arg(item->startMinute/60).arg(item->startMinute%60));
    if(dialog->exec()==QDialog::Accepted)
    {
        m_meshModel->dbManager()->dbCaller()->removeControlItem(item->id);
    }
}

void TimeLineControlItemsEditDialogFrame::on_tableView_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
    {
        return;
    }
    TimeLineControlItem *item=m_meshModel->timeLineControlManager()->getTimeLineControlItemsModel()->getById(index.data(Qt::UserRole).toInt());
    if(item==NULL)
    {
        return;
    }

    TimeLineControlItem *newItem=item->copyAll();
    QDialog *dialog=DialogBuilder::getInstance()->buildControlItemEditDialog(m_meshModel, tr("Edit Control Item %1:%2").arg(item->startMinute/60, 2, 10, QLatin1Char('0')).arg(item->startMinute%60, 2, 10, QLatin1Char('0')), newItem);
    if(dialog==NULL)
    {
        return;
    }
    if(dialog->exec()==QDialog::Accepted)
    {
        m_meshModel->dbManager()->dbCaller()->updateControlItem(newItem);
    }
    else
    {
        delete newItem;
    }
}

void TimeLineControlItemsEditDialogFrame::updateGraph()
{
    QList<TimeLineControlItem*> itemList=m_meshModel->timeLineControlManager()->getTimeLineControlItemsModel()->getTimeLineControlItemList(m_group->getId(), m_policySubId);
    ui->timeLineGraphFrame->updateGraph(itemList);
}

void TimeLineControlItemsEditDialogFrame::on_buttonDialogBottomClose_clicked()
{
    emit accept();
}

void TimeLineControlItemsEditDialogFrame::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    m_menuMain->exec(ui->tableView->mapToGlobal(pos));
}

void TimeLineControlItemsEditDialogFrame::initMenuAndAction()
{
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    m_menuMain=new QMenu(this);
    m_actionAdd=new QAction(tr("Global.Add"), this);
    connect(m_actionAdd, SIGNAL(triggered(bool)), ui->buttonAdd, SLOT(click()));
    m_menuMain->addAction(m_actionAdd);
    m_actionDelete=new QAction(tr("Global.Remove"), this);
    connect(m_actionDelete, SIGNAL(triggered(bool)), ui->buttonRemove, SLOT(click()));
    m_menuMain->addAction(m_actionDelete);
}

void TimeLineControlItemsEditDialogFrame::on_lineControlPolicyName_editingFinished()
{
    TimeLineControlPolicy *controlPolicy=m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->getById(this->m_controlPolicyId);
    if(controlPolicy==NULL)
    {
        return;
    }
    m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->updatePolicy(controlPolicy->id, ui->lineControlPolicyName->text(), controlPolicy->color, controlPolicy->description, controlPolicy->subId);
}

void TimeLineControlItemsEditDialogFrame::on_lineControlPolicyDescription_editingFinished()
{
    TimeLineControlPolicy *controlPolicy=m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->getById(this->m_controlPolicyId);
    if(controlPolicy==NULL)
    {
        return;
    }
    m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->updatePolicy(controlPolicy->id, controlPolicy->name, controlPolicy->color, ui->lineControlPolicyDescription->text(), controlPolicy->subId);
}

void TimeLineControlItemsEditDialogFrame::on_buttonColor_clicked()
{
    bool ok=false;
    QColor color=DialogBuilder::getInstance()->runGetColorDialog(tr("Select Color"), QColor(m_color), &ok);
    if(!ok)
    {
        return;
    }
    m_color=color.name();
    ui->buttonColor->setStyleSheet(QString("border:none; background-color:%1;").arg(m_color));

    TimeLineControlPolicy *controlPolicy=m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->getById(this->m_controlPolicyId);
    if(controlPolicy==NULL)
    {
        return;
    }
    m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->updatePolicy(controlPolicy->id, controlPolicy->name, this->m_color, controlPolicy->description, controlPolicy->subId);
}

void TimeLineControlItemsEditDialogFrame::on_buttonLoadFrameTemplate_clicked()
{
    QDialog *dialog = nullptr;
    if (m_timeLineControlItemTableViewModel->timeLineControlPolicyModel()->count() > 0) //There are some existing policy items
    {
        dialog = DialogBuilder::getInstance()->buildQuestionDialog(tr("Import From Template"),
                                                                          tr("Original configurations will be cleared, are you sure to import?"));
        if(dialog->exec()==QDialog::Rejected)
        {
            return;
        }
    }

    dialog = DialogBuilder::getInstance()->buildTemplateTimeLineControlPolicySelectDialog(m_meshModel,
                                              m_group, m_policySubId, tr("Import From Template"));
    dialog->exec();
}

void TimeLineControlItemsEditDialogFrame::on_buttonExportTemplate_clicked()
{
    if (m_timeLineControlItemTableViewModel->timeLineControlPolicyModel()->count() == 0) //There are some existing policy items
    {
        QMessageBox::information(this, tr("TimeLineControlItemsEditDialogFrame.ExportTemplate"),
                                 tr("TimeLineControlItemsEditDialogFrame.NoTemplateToExport"));
        return;
    }

    QDialog *dialog=DialogBuilder::getInstance()->buildExportTimeLineControlItemTemplateDialog(m_meshModel, m_group, m_policySubId, tr("Export To Template"));
    dialog->exec();
}
