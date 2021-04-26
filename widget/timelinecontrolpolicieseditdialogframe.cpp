#include "timelinecontrolpolicieseditdialogframe.h"
#include "ui_timelinecontrolpolicieseditdialogframe.h"
#include "model/timelinecontrolpoliciesmodel.h"
#include "utils/timelinecontrolmanager.h"
#include "languagemanager.h"
#include "dialogbuilder.h"
#include "customoneyearcalendarframe.h"
#include "model/controldatesmodel.h"
#include "delegate/timelinecontrolpolicytableitemdelegate.h"
#include "model/nodegroupsmodel.h"
#include "db/meshdbmanager.h"
#include "db/meshdbcaller.h"

TimeLineControlPoliciesEditDialogFrame::TimeLineControlPoliciesEditDialogFrame(MeshModel *meshModel, NodeGroup *group, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TimeLineControlPoliciesEditDialogFrame)
{
    ui->setupUi(this);
    m_meshModel=meshModel;
    m_group=group;
    m_timeLineControlPolicyTableViewModel=new TimeLineControlPolicyTableViewModel(meshModel, m_group->getId());
    ui->tableControlPolicy->setModel(m_timeLineControlPolicyTableViewModel);
    ui->tableControlPolicy->setDragEnabled(true);
    ui->tableControlPolicy->setDragDropMode(QTableView::DragOnly);
    ui->tableControlPolicy->setDragDropOverwriteMode(true);

    ui->tableControlPolicy->setEditTriggers(QTableView::DoubleClicked);
    ui->tableControlPolicy->verticalHeader()->setVisible( false );
    ui->tableControlPolicy->horizontalHeader()->setStretchLastSection( true );
    ui->tableControlPolicy->setSelectionMode(QTableView::SingleSelection);
    ui->tableControlPolicy->setSelectionBehavior(QTableView::SelectRows);
    ui->tableControlPolicy->setItemDelegateForColumn(2, new TimeLineControlPolicyTableItemDelegate(m_timeLineControlPolicyTableViewModel->timeLineControlPolicyListModel(), ui->tableControlPolicy));

    m_timeLineControlPolicyTableViewModel->timeLineControlPolicyListModel()->init();

    ui->buttonDialogBottomClose->setCursor(Qt::PointingHandCursor);

    ui->groupOneYearCalendarContainer->setLayout(new QVBoxLayout);
    m_customOneYearCalendarFrame=new CustomOneYearCalendarFrame(m_meshModel, m_group->getId(), ui->groupOneYearCalendarContainer);
    connect(m_meshModel->controlDatesModel(), SIGNAL(controlDateModelChanged()), m_customOneYearCalendarFrame, SLOT(doControlDateModelChanged()));
    ui->groupOneYearCalendarContainer->layout()->addWidget(m_customOneYearCalendarFrame);

    m_buttonOnDuty=new CalendarDropableButton("on_duty", "");
    QHBoxLayout *layoutOnDuty=new QHBoxLayout;
    layoutOnDuty->setContentsMargins(5, 5, 5, 5);
    layoutOnDuty->addWidget(m_buttonOnDuty);
    ui->groupScheduleByOnDutyConfiguration->setLayout(layoutOnDuty);

    m_buttonOffDuty=new CalendarDropableButton("off_duty", "");
    QHBoxLayout *layoutOffDuty=new QHBoxLayout;
    layoutOffDuty->setContentsMargins(5, 5, 5, 5);
    layoutOffDuty->addWidget(m_buttonOffDuty);
    ui->groupScheduleByOffDutyConfiguration->setLayout(layoutOffDuty);

    connect(m_buttonOnDuty, SIGNAL(controlPolicyDroped(int)), this, SLOT(doDutyButtonPolicyDrop(int)));
    connect(m_buttonOffDuty, SIGNAL(controlPolicyDroped(int)), this, SLOT(doDutyButtonPolicyDrop(int)));

    connect(m_meshModel->controlDatesModel(), SIGNAL(controlDateModelChanged()), this, SLOT(doControlDateModelChanged()));

    updateDutyButtons();
}

TimeLineControlPoliciesEditDialogFrame::~TimeLineControlPoliciesEditDialogFrame()
{
    delete ui;
}

void TimeLineControlPoliciesEditDialogFrame::on_buttonDialogBottomClose_clicked()
{
    emit accept();
}

void TimeLineControlPoliciesEditDialogFrame::on_tableControlPolicy_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
    {
        return;
    }
    int id=index.data(Qt::UserRole).toInt();
    TimeLineControlPolicy *controlPolicy=m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->getById(id);
    if(controlPolicy==NULL)
    {
        return;
    }

    QDialog *dialog=DialogBuilder::getInstance()->buildTimeLineControlItemsEditDialog(m_meshModel,
                                                                                      tr("TimeLineControlItemsEditDialog.DialogTitle").arg(controlPolicy->name),
                                                                                      controlPolicy->id,
                                                                                      controlPolicy->subId,
                                                                                      m_group);
    dialog->exec();
}

void TimeLineControlPoliciesEditDialogFrame::doDutyButtonPolicyDrop(int policyId)
{
    qDebug()<<" TimeLineControlPoliciesEditDialogFrame::doDutyButtonPolicyDrop controlPolicyId="<<policyId;
    QList<ControlDate*> *controlDateList=new QList<ControlDate*>;
    CalendarDropableButton *button=static_cast<CalendarDropableButton*>(sender());
    if(button==NULL)
    {
        return;
    }
    if(!button->isDutyMode())
    {
        return;
    }

    ControlDate *controlDate=new ControlDate;
    controlDate->id=QString("%1_%2").arg(this->m_group->getId()).arg(button->dutyTypeText());
    controlDate->month=1;
    controlDate->day=1;
    controlDate->groupId=m_group->getId();
    controlDate->controlPolicyId=policyId;
    controlDateList->push_back(controlDate);

    m_meshModel->controlDatesModel()->setControlDates(m_group->getId(), controlDateList);
}

void TimeLineControlPoliciesEditDialogFrame::doControlDateModelChanged()
{
    updateDutyButtons();
}

void TimeLineControlPoliciesEditDialogFrame::updateDutyButtons()
{
    QString color="#888888";
    ControlDate *controlDateOnDuty=m_meshModel->controlDatesModel()->getById(QString("%1_on_duty").arg(m_group->getId()));
    if(controlDateOnDuty!=NULL)
    {
        TimeLineControlPolicy *controlPolicy=m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->getById(controlDateOnDuty->controlPolicyId);
        if(controlPolicy!=NULL)
        {
            color=controlPolicy->color;
            m_buttonOnDuty->setText(QString("[%1] %2").arg(controlPolicy->subId).arg(controlPolicy->name));
        }
    }
    m_buttonOnDuty->setColor(color);
    m_buttonOnDuty->resetStyle();

    color="#888888";
    ControlDate *controlDateOffDuty=m_meshModel->controlDatesModel()->getById(QString("%1_off_duty").arg(m_group->getId()));
    if(controlDateOffDuty!=NULL)
    {
        TimeLineControlPolicy *controlPolicy=m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->getById(controlDateOffDuty->controlPolicyId);
        if(controlPolicy!=NULL)
        {
            color=controlPolicy->color;
            m_buttonOffDuty->setText(QString("[%1] %2").arg(controlPolicy->subId).arg(controlPolicy->name));
        }
    }
    m_buttonOffDuty->setColor(color);
    m_buttonOffDuty->resetStyle();
}


void TimeLineControlPoliciesEditDialogFrame::on_buttonAdd_clicked()
{
    TimeLineControlPolicy *controlPolicy=new TimeLineControlPolicy;
    controlPolicy->groupId=this->m_group->getId();
    QDialog *dialog=DialogBuilder::getInstance()->buildAddControlPolicyDialog(controlPolicy, tr("Add Control Policy"));
    if(dialog->exec()==QDialog::Accepted)
    {
        int subId=m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->getNextSubId(this->m_group->getId());
        controlPolicy->subId=subId;
        m_meshModel->dbManager()->dbCaller()->addControlPolicy(controlPolicy);
    }
    else
    {
        delete controlPolicy;
    }
}

void TimeLineControlPoliciesEditDialogFrame::on_buttonRemove_clicked()
{
    QModelIndex index=ui->tableControlPolicy->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    int id=index.data(Qt::UserRole).toInt();
    TimeLineControlPolicy *itemCollection=m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->getById(id);
    if(itemCollection==NULL)
    {
        return;
    }

    QDialog *dialog=DialogBuilder::getInstance()->buildQuestionDialog(tr("Remove Control Policy"), tr("Are you sure to remove the control policy?"));
    if(dialog->exec()==QDialog::Accepted)
    {
        if(itemCollection->isDefaultPolicy())
        {
            return;
        }
        m_meshModel->dbManager()->dbCaller()->removeControlPolicy(id);
    }
}
