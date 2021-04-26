#include "exporttimelinecontrolitemtemplatedialogframe.h"
#include "ui_exporttimelinecontrolitemtemplatedialogframe.h"
#include "domain/timelinecontrolitem.h"
#include "domain/timelinecontrolitem.h"
#include "domain/templatetimelinecontrolpolicy.h"
#include "model/meshmodel.h"
#include "db/meshdbmanager.h"
#include "db/meshdbcaller.h"
#include "objectfactory.h"
#include "utils/tools.h"
#include "utils/timelinecontrolmanager.h"
#include <QUuid>


ExportTimeLineControlItemTemplateDialogFrame::ExportTimeLineControlItemTemplateDialogFrame(MeshModel *meshModel, NodeGroup *group, int policySubId, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ExportTimeLineControlItemTemplateDialogFrame)
{
    ui->setupUi(this);
    m_applyMovie=NULL;
    m_meshModel=meshModel;
    m_group=group;
    m_policySubId=policySubId;

    ui->stackedWidget->setCurrentWidget(ui->page1);

    ui->buttonOk->setCursor(Qt::PointingHandCursor);
    ui->buttonCancel->setCursor(Qt::PointingHandCursor);
}

ExportTimeLineControlItemTemplateDialogFrame::~ExportTimeLineControlItemTemplateDialogFrame()
{
    if(m_applyMovie!=NULL)
    {
        m_applyMovie->deleteLater();
        m_applyMovie=NULL;
    }
    delete ui;
}

void ExportTimeLineControlItemTemplateDialogFrame::on_buttonOk_clicked()
{
    if(m_applyMovie!=NULL)
    {
        return;
    }
    m_applyMovie=new QMovie(":/images/loading.gif");
    ui->labelLoading->setMovie(m_applyMovie);
    m_applyMovie->start();
    ui->stackedWidget->setCurrentWidget(ui->page2);

    connect(m_meshModel->dbManager()->dbCaller(), SIGNAL(saveTemplateItemListAsNewCollectionFinished()),
            this, SLOT(doSaveTemplateItemListAsNewCollectionFinished()));

    QList<TimeLineControlItem*> itemList = m_meshModel->timeLineControlManager()->getTimeLineControlItemsModel()->getTimeLineControlItemList(m_group->getId(), m_policySubId);
    QList<TimeLineControlItem*> *templateItemList = new QList<TimeLineControlItem*>;
    TemplateTimeLineControlPolicy *policy = new TemplateTimeLineControlPolicy;

    policy->id = Tools::getRandomUUID();
    policy->name = ui->lineName->text();
    policy->description = ui->lineDescription->text();
    policy->groupType = NodeGroup::getTextByGroupType(m_group->getGroupType());

    for(int i = 0; i < itemList.size(); i++)
    {
        TimeLineControlItem *item = itemList.at(i);
        TimeLineControlItem *templateItem = ObjectFactory::instance()->createTimeLineControlItemByGroupType(m_group->getGroupType());

        templateItem->loadAll(item);
        templateItem->templateItemId = Tools::getRandomUUID();
        templateItem->templatePolicyId = policy->id;

        templateItemList->push_back(templateItem);
    }

    m_meshModel->dbManager()->dbCaller()->saveTemplateItemListAsNewCollection(policy, templateItemList);
}

void ExportTimeLineControlItemTemplateDialogFrame::on_buttonCancel_clicked()
{
    emit reject();
}

void ExportTimeLineControlItemTemplateDialogFrame::doSaveTemplateItemListAsNewCollectionFinished()
{
    emit accept();
}
