#include "customoneyearcalendarframe.h"
#include "ui_customoneyearcalendarframe.h"

#include <QDate>
#include <QDebug>
#include <QMouseEvent>
#include "calendardropablebutton.h"
#include "model/meshmodel.h"
#include "model/controldatesmodel.h"
#include "utils/timelinecontrolmanager.h"

CustomOneYearCalendarFrame::CustomOneYearCalendarFrame(MeshModel *meshModel, quint8 groupId, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CustomOneYearCalendarFrame),
    m_meshModel(meshModel)
{
    ui->setupUi(this);
    m_groupId=groupId;

    m_frameMonthMap.insert(1, ui->frameMonthButtonContainer_1);
    m_frameMonthMap.insert(2, ui->frameMonthButtonContainer_2);
    m_frameMonthMap.insert(3, ui->frameMonthButtonContainer_3);
    m_frameMonthMap.insert(4, ui->frameMonthButtonContainer_4);
    m_frameMonthMap.insert(5, ui->frameMonthButtonContainer_5);
    m_frameMonthMap.insert(6, ui->frameMonthButtonContainer_6);
    m_frameMonthMap.insert(7, ui->frameMonthButtonContainer_7);
    m_frameMonthMap.insert(8, ui->frameMonthButtonContainer_8);
    m_frameMonthMap.insert(9, ui->frameMonthButtonContainer_9);
    m_frameMonthMap.insert(10, ui->frameMonthButtonContainer_10);
    m_frameMonthMap.insert(11, ui->frameMonthButtonContainer_11);
    m_frameMonthMap.insert(12, ui->frameMonthButtonContainer_12);

    QList<QLabel*> monthTitleList;
    monthTitleList.push_back(ui->labelMonthTitle_1);
    monthTitleList.push_back(ui->labelMonthTitle_2);
    monthTitleList.push_back(ui->labelMonthTitle_3);
    monthTitleList.push_back(ui->labelMonthTitle_4);
    monthTitleList.push_back(ui->labelMonthTitle_5);
    monthTitleList.push_back(ui->labelMonthTitle_6);
    monthTitleList.push_back(ui->labelMonthTitle_7);
    monthTitleList.push_back(ui->labelMonthTitle_8);
    monthTitleList.push_back(ui->labelMonthTitle_9);
    monthTitleList.push_back(ui->labelMonthTitle_10);
    monthTitleList.push_back(ui->labelMonthTitle_11);
    monthTitleList.push_back(ui->labelMonthTitle_12);
    for(int i=0; i<monthTitleList.size(); i++)
    {
        if((i+1)>=QDateTime::currentDateTime().date().month())
        {
            monthTitleList.at(i)->setToolTip(QString::number(QDateTime::currentDateTime().date().year()));
            monthTitleList.at(i)->setStyleSheet("font-weight:bold;");
        }
        else
        {
            monthTitleList.at(i)->setToolTip(QString::number(QDateTime::currentDateTime().date().year()+1));
            monthTitleList.at(i)->setStyleSheet("font-weight:normal;");
        }
    }


    for(int month=1; month<=12; month++)
    {
        QDate monthDate;
        if(month>=QDateTime::currentDateTime().date().month())
        {
            monthDate=QDate(QDate::currentDate().year(), month, 1);  //大于等于现在月份的按照今年处理
        }
        else
        {
            monthDate=QDate(QDate::currentDate().year()+1, month, 1);  //小于现在月份的按照明年处理
        }
        if(monthDate.dayOfWeek()==1)
        {
            monthDate=monthDate.addDays(-7);
        }
        else
        {
            monthDate=monthDate.addDays(-1*monthDate.dayOfWeek()+1);
        }

        for(int weekLine=1; weekLine<=6; weekLine++)
        {
            for(int week=1; week<=7; week++)
            {
                CalendarDropableButton *button=new CalendarDropableButton(monthDate);
                button->setMonthInDetailsMode(false);
                button->setMaximumSize(20, 20);
                button->setMinimumSize(20, 20);
                button->setParent(m_frameMonthMap.value(month));
                button->move(20*(week-1), 20*(weekLine-1));
                connect(button, SIGNAL(clicked()), this, SLOT(clicked()));
                connect(button, SIGNAL(pressed()), this, SLOT(pressed()));
                if(monthDate.month()==month)
                {
                    button->setInMonth(true);
                    m_dateButtonMap.insert(monthDate, button);
                    m_buttonDateMap.insert(button, monthDate);
                    m_inMothButtonList.push_back(button);
                    connect(button, SIGNAL(controlPolicyDroped(int)), this, SLOT(doPolicyDroped(int)));
                }
                else
                {
                    button->setInMonth(false);
                    button->resetStyle();
                }
                monthDate=monthDate.addDays(1);
            }
        }
    }

    QGridLayout *gridLayout=new QGridLayout(ui->frameMonthDetailButtonContainer);
    gridLayout->setVerticalSpacing(0);
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    ui->frameMonthDetailButtonContainer->setLayout(gridLayout);

    for(int weekLine=1; weekLine<=6; weekLine++)
    {
        for(int week=1; week<=7; week++)
        {
            CalendarDropableButton *button=new CalendarDropableButton(ui->frameMonthDetailButtonContainer);
            button->setMonthInDetailsMode(true);
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            gridLayout->addWidget(button, weekLine-1, week-1, 1, 1);
            m_monthDetailsButtonList.push_back(button);
            connect(button, SIGNAL(clicked()), this, SLOT(clicked()));
            connect(button, SIGNAL(pressed()), this, SLOT(pressed()));
            connect(button, SIGNAL(controlPolicyDroped(int)), this, SLOT(doPolicyDroped(int)));
        }
    }

    m_mousePressed=false;

    ui->frameMonthButtonContainer_1->setProperty("type", "calendar.month.container");
    ui->frameMonthButtonContainer_2->setProperty("type", "calendar.month.container");
    ui->frameMonthButtonContainer_3->setProperty("type", "calendar.month.container");
    ui->frameMonthButtonContainer_4->setProperty("type", "calendar.month.container");
    ui->frameMonthButtonContainer_5->setProperty("type", "calendar.month.container");
    ui->frameMonthButtonContainer_6->setProperty("type", "calendar.month.container");
    ui->frameMonthButtonContainer_7->setProperty("type", "calendar.month.container");
    ui->frameMonthButtonContainer_8->setProperty("type", "calendar.month.container");
    ui->frameMonthButtonContainer_9->setProperty("type", "calendar.month.container");
    ui->frameMonthButtonContainer_10->setProperty("type", "calendar.month.container");
    ui->frameMonthButtonContainer_11->setProperty("type", "calendar.month.container");
    ui->frameMonthButtonContainer_12->setProperty("type", "calendar.month.container");

    ui->frameMonthTitle_1->setProperty("type", "calendar.month.title");
    ui->frameMonthTitle_2->setProperty("type", "calendar.month.title");
    ui->frameMonthTitle_3->setProperty("type", "calendar.month.title");
    ui->frameMonthTitle_4->setProperty("type", "calendar.month.title");
    ui->frameMonthTitle_5->setProperty("type", "calendar.month.title");
    ui->frameMonthTitle_6->setProperty("type", "calendar.month.title");
    ui->frameMonthTitle_7->setProperty("type", "calendar.month.title");
    ui->frameMonthTitle_8->setProperty("type", "calendar.month.title");
    ui->frameMonthTitle_9->setProperty("type", "calendar.month.title");
    ui->frameMonthTitle_10->setProperty("type", "calendar.month.title");
    ui->frameMonthTitle_11->setProperty("type", "calendar.month.title");
    ui->frameMonthTitle_12->setProperty("type", "calendar.month.title");

    ui->buttonMonthDetailsView_1->setProperty("type", "calendar.month.details");
    ui->buttonMonthDetailsView_2->setProperty("type", "calendar.month.details");
    ui->buttonMonthDetailsView_3->setProperty("type", "calendar.month.details");
    ui->buttonMonthDetailsView_4->setProperty("type", "calendar.month.details");
    ui->buttonMonthDetailsView_5->setProperty("type", "calendar.month.details");
    ui->buttonMonthDetailsView_6->setProperty("type", "calendar.month.details");
    ui->buttonMonthDetailsView_7->setProperty("type", "calendar.month.details");
    ui->buttonMonthDetailsView_8->setProperty("type", "calendar.month.details");
    ui->buttonMonthDetailsView_9->setProperty("type", "calendar.month.details");
    ui->buttonMonthDetailsView_10->setProperty("type", "calendar.month.details");
    ui->buttonMonthDetailsView_11->setProperty("type", "calendar.month.details");
    ui->buttonMonthDetailsView_12->setProperty("type", "calendar.month.details");

    ui->buttonMonthDetailsView_1->setToolTip(tr("Global.ShowDaysOfThisMonth"));
    ui->buttonMonthDetailsView_2->setToolTip(tr("Global.ShowDaysOfThisMonth"));
    ui->buttonMonthDetailsView_3->setToolTip(tr("Global.ShowDaysOfThisMonth"));
    ui->buttonMonthDetailsView_4->setToolTip(tr("Global.ShowDaysOfThisMonth"));
    ui->buttonMonthDetailsView_5->setToolTip(tr("Global.ShowDaysOfThisMonth"));
    ui->buttonMonthDetailsView_6->setToolTip(tr("Global.ShowDaysOfThisMonth"));
    ui->buttonMonthDetailsView_7->setToolTip(tr("Global.ShowDaysOfThisMonth"));
    ui->buttonMonthDetailsView_8->setToolTip(tr("Global.ShowDaysOfThisMonth"));
    ui->buttonMonthDetailsView_9->setToolTip(tr("Global.ShowDaysOfThisMonth"));
    ui->buttonMonthDetailsView_10->setToolTip(tr("Global.ShowDaysOfThisMonth"));
    ui->buttonMonthDetailsView_11->setToolTip(tr("Global.ShowDaysOfThisMonth"));
    ui->buttonMonthDetailsView_12->setToolTip(tr("Global.ShowDaysOfThisMonth"));

    ui->buttonMonthListView->setToolTip(tr("Global.ShowDaysOfThisYear"));

    ui->buttonMonthListView->setProperty("type", "calendar.month.list");
    ui->frameCommonMonthTitle->setProperty("type", "calendar.monthdetails.title");
    ui->labelMonthTitle_Common->setProperty("type", "calendar.monthdetails.title");
    ui->frameMonthCommonContainer->setProperty("type", "calendar.monthdetails.container");

    ui->buttonMonthDetailsView_1->setProperty("month", 1);
    ui->buttonMonthDetailsView_2->setProperty("month", 2);
    ui->buttonMonthDetailsView_3->setProperty("month", 3);
    ui->buttonMonthDetailsView_4->setProperty("month", 4);
    ui->buttonMonthDetailsView_5->setProperty("month", 5);
    ui->buttonMonthDetailsView_6->setProperty("month", 6);
    ui->buttonMonthDetailsView_7->setProperty("month", 7);
    ui->buttonMonthDetailsView_8->setProperty("month", 8);
    ui->buttonMonthDetailsView_9->setProperty("month", 9);
    ui->buttonMonthDetailsView_10->setProperty("month", 10);
    ui->buttonMonthDetailsView_11->setProperty("month", 11);
    ui->buttonMonthDetailsView_12->setProperty("month", 12);
    connect(ui->buttonMonthDetailsView_1, SIGNAL(clicked(bool)), this, SLOT(doMonthDetailsViewButtonClicked()));
    connect(ui->buttonMonthDetailsView_2, SIGNAL(clicked(bool)), this, SLOT(doMonthDetailsViewButtonClicked()));
    connect(ui->buttonMonthDetailsView_3, SIGNAL(clicked(bool)), this, SLOT(doMonthDetailsViewButtonClicked()));
    connect(ui->buttonMonthDetailsView_4, SIGNAL(clicked(bool)), this, SLOT(doMonthDetailsViewButtonClicked()));
    connect(ui->buttonMonthDetailsView_5, SIGNAL(clicked(bool)), this, SLOT(doMonthDetailsViewButtonClicked()));
    connect(ui->buttonMonthDetailsView_6, SIGNAL(clicked(bool)), this, SLOT(doMonthDetailsViewButtonClicked()));
    connect(ui->buttonMonthDetailsView_7, SIGNAL(clicked(bool)), this, SLOT(doMonthDetailsViewButtonClicked()));
    connect(ui->buttonMonthDetailsView_8, SIGNAL(clicked(bool)), this, SLOT(doMonthDetailsViewButtonClicked()));
    connect(ui->buttonMonthDetailsView_9, SIGNAL(clicked(bool)), this, SLOT(doMonthDetailsViewButtonClicked()));
    connect(ui->buttonMonthDetailsView_10, SIGNAL(clicked(bool)), this, SLOT(doMonthDetailsViewButtonClicked()));
    connect(ui->buttonMonthDetailsView_11, SIGNAL(clicked(bool)), this, SLOT(doMonthDetailsViewButtonClicked()));
    connect(ui->buttonMonthDetailsView_12, SIGNAL(clicked(bool)), this, SLOT(doMonthDetailsViewButtonClicked()));

    connect(ui->buttonMonthListView, SIGNAL(clicked(bool)), this, SLOT(doMonthListViewButtonClicked()));

    ui->buttonMonthDetailsView_1->setCursor(Qt::PointingHandCursor);
    ui->buttonMonthDetailsView_2->setCursor(Qt::PointingHandCursor);
    ui->buttonMonthDetailsView_3->setCursor(Qt::PointingHandCursor);
    ui->buttonMonthDetailsView_4->setCursor(Qt::PointingHandCursor);
    ui->buttonMonthDetailsView_5->setCursor(Qt::PointingHandCursor);
    ui->buttonMonthDetailsView_6->setCursor(Qt::PointingHandCursor);
    ui->buttonMonthDetailsView_7->setCursor(Qt::PointingHandCursor);
    ui->buttonMonthDetailsView_8->setCursor(Qt::PointingHandCursor);
    ui->buttonMonthDetailsView_9->setCursor(Qt::PointingHandCursor);
    ui->buttonMonthDetailsView_10->setCursor(Qt::PointingHandCursor);
    ui->buttonMonthDetailsView_11->setCursor(Qt::PointingHandCursor);
    ui->buttonMonthDetailsView_12->setCursor(Qt::PointingHandCursor);

    ui->buttonMonthListView->setCursor(Qt::PointingHandCursor);

    ui->checkMonday->setProperty("week", 1);
    ui->checkTuesday->setProperty("week", 2);
    ui->checkWednesday->setProperty("week", 3);
    ui->checkThursday->setProperty("week", 4);
    ui->checkFriday->setProperty("week", 5);
    ui->checkSaturday->setProperty("week", 6);
    ui->checkSunday->setProperty("week", 7);
    connect(ui->checkMonday, SIGNAL(clicked(bool)), this, SLOT(doWeekCheckboxClicked(bool)));
    connect(ui->checkTuesday, SIGNAL(clicked(bool)), this, SLOT(doWeekCheckboxClicked(bool)));
    connect(ui->checkWednesday, SIGNAL(clicked(bool)), this, SLOT(doWeekCheckboxClicked(bool)));
    connect(ui->checkThursday, SIGNAL(clicked(bool)), this, SLOT(doWeekCheckboxClicked(bool)));
    connect(ui->checkFriday, SIGNAL(clicked(bool)), this, SLOT(doWeekCheckboxClicked(bool)));
    connect(ui->checkSaturday, SIGNAL(clicked(bool)), this, SLOT(doWeekCheckboxClicked(bool)));
    connect(ui->checkSunday, SIGNAL(clicked(bool)), this, SLOT(doWeekCheckboxClicked(bool)));

    this->updateYearButtons();
    showMonthDetails(QDateTime::currentDateTime().date().year(), QDate::currentDate().month());
}

CustomOneYearCalendarFrame::~CustomOneYearCalendarFrame()
{
    delete ui;
}

void CustomOneYearCalendarFrame::mousePressEvent(QMouseEvent *ev)
{
    m_mousePressed=true;
    m_pressPoint=ev->pos();
    m_currentPoint=ev->pos();
}

void CustomOneYearCalendarFrame::mouseReleaseEvent(QMouseEvent *ev)
{
    m_mousePressed=false;
}

void CustomOneYearCalendarFrame::mouseMoveEvent(QMouseEvent *ev)
{
    m_currentPoint=ev->pos();
    updateSelection();
}

bool CustomOneYearCalendarFrame::ctrlPressed()
{
    if(QApplication::queryKeyboardModifiers() & Qt::ControlModifier)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CustomOneYearCalendarFrame::updateSelection()
{
    QRect rect;
    if(m_pressPoint.x()<m_currentPoint.x())
    {
        if(m_pressPoint.y()<m_currentPoint.y())
        {
            rect.setX(m_pressPoint.x());
            rect.setY(m_pressPoint.y());
            rect.setWidth(m_currentPoint.x()-m_pressPoint.x());
            rect.setHeight(m_currentPoint.y()-m_pressPoint.y());
        }
        else
        {
            rect.setX(m_pressPoint.x());
            rect.setY(m_currentPoint.y());
            rect.setWidth(m_currentPoint.x()-m_pressPoint.x());
            rect.setHeight(m_pressPoint.y()-m_currentPoint.y());
        }
    }
    else
    {
        if(m_pressPoint.y()<m_currentPoint.y())
        {
            rect.setX(m_currentPoint.x());
            rect.setY(m_pressPoint.y());
            rect.setWidth(m_pressPoint.x()-m_currentPoint.x());
            rect.setHeight(m_currentPoint.y()-m_pressPoint.y());
        }
        else
        {
            rect.setX(m_currentPoint.x());
            rect.setY(m_currentPoint.y());
            rect.setWidth(m_pressPoint.x()-m_currentPoint.x());
            rect.setHeight(m_pressPoint.y()-m_currentPoint.y());
        }
    }
    if(ui->stackEditInYearAndMonth->currentWidget()==ui->pageEditInYear)
    {
        int len=m_inMothButtonList.size();
        for(int i=0; i<len; i++)
        {
            CalendarDropableButton *button=m_inMothButtonList.at(i);
            QPoint pos=button->mapTo(this, QPoint(button->width()/2, button->height()/2));
            if(pos.x()>=rect.x() && pos.x()<=(rect.x()+rect.width()) && pos.y()>=rect.y() && (pos.y()<=(rect.y()+rect.height())))
            {
                button->setSelected(true);
            }
            else
            {
                button->setSelected(false);
            }
            button->resetStyle();
        }
    }
    else
    {
        int len2=m_monthDetailsInMonthButtonList.size();
        for(int i=0; i<len2; i++)
        {
            CalendarDropableButton *button=m_monthDetailsInMonthButtonList.at(i);
            QPoint pos=button->mapTo(this, QPoint(button->width()/2, button->height()/2));
            if(pos.x()>=rect.x() && pos.x()<=(rect.x()+rect.width()) && pos.y()>=rect.y() && (pos.y()<=(rect.y()+rect.height())))
            {
                button->setSelected(true);
            }
            else
            {
                button->setSelected(false);
            }
            button->resetStyle();
        }
    }
}

void CustomOneYearCalendarFrame::updateYearButtons()
{
    int len=m_inMothButtonList.size();
    for(int i=0; i<len; i++)
    {
        CalendarDropableButton *button=m_inMothButtonList.at(i);
        QDate date=button->date();
        QString color="#888888";
        ControlDate *controlDate=m_meshModel->controlDatesModel()->getById(QString("%1_%2_%3").arg(m_groupId).arg(date.month()).arg(date.day()));
        if(controlDate!=NULL)
        {
            TimeLineControlPolicy *controlPolicy=m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->getById(controlDate->controlPolicyId);
            if(controlPolicy!=NULL)
            {
                color=controlPolicy->color;
            }
        }
        button->setColor(color);
        button->resetStyle();
    }
}

void CustomOneYearCalendarFrame::updateMonthDetailsButtons()
{
    int len2=m_monthDetailsInMonthButtonList.size();
    for(int i=0; i<len2; i++)
    {
        CalendarDropableButton *button=m_monthDetailsInMonthButtonList.at(i);
        QDate date=button->date();
        QString color="#888888";
        ControlDate *controlDate=m_meshModel->controlDatesModel()->getById(QString("%1_%2_%3").arg(m_groupId).arg(date.month()).arg(date.day()));
        if(controlDate!=NULL)
        {
            TimeLineControlPolicy *controlPolicy=m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->getById(controlDate->controlPolicyId);
            if(controlPolicy!=NULL)
            {
                color=controlPolicy->color;
            }
        }
        button->setColor(color);
    }
    int len3=m_monthDetailsButtonList.size();
    for(int i=0; i<len3; i++){
        m_monthDetailsButtonList.at(i)->resetStyle();
    }
}

void CustomOneYearCalendarFrame::clicked()
{
    CalendarDropableButton *button=static_cast<CalendarDropableButton*>(sender());
    if(button==NULL)
    {
        return;
    }
    if(!ctrlPressed())
    {
        if(ui->stackEditInYearAndMonth->currentWidget()==ui->pageEditInYear)
        {
            int len=m_inMothButtonList.size();
            for(int i=0; i<len; i++)
            {
                m_inMothButtonList.at(i)->setSelected(false);
                m_inMothButtonList.at(i)->resetStyle();
            }
        }
        else
        {
            int len2=m_monthDetailsInMonthButtonList.size();
            for(int i=0; i<len2; i++)
            {
                m_monthDetailsInMonthButtonList.at(i)->setSelected(false);
                m_monthDetailsInMonthButtonList.at(i)->resetStyle();
            }
        }
    }
    if(!button->inMonth())
    {
        return;
    }
    button->setSelected(true);
    button->resetStyle();

    QDate date=button->date();
    int policyId=0;
    ControlDate *controlDate=m_meshModel->controlDatesModel()->getById(QString("%1_%2_%3").arg(m_groupId).arg(date.month()).arg(date.day()));
    if(controlDate!=NULL)
    {
        policyId=controlDate->controlPolicyId;
    }
    TimeLineControlPolicy *controlPolicy=m_meshModel->timeLineControlManager()->timeLineControlPoliciesModel()->getById(policyId);
    if(controlPolicy==NULL)
    {
        ui->labelDateDetails->setText(QString("%1 [%2]").arg(date.toString("yyyy-MM-dd dddd")).arg(tr("Default Mesh")));
    }
    else
    {
        ui->labelDateDetails->setText(QString("%1 [%2]").arg(date.toString("yyyy-MM-dd dddd")).arg(controlPolicy->name));
    }

    ui->checkMonday->setChecked(false);
    ui->checkTuesday->setChecked(false);
    ui->checkWednesday->setChecked(false);
    ui->checkThursday->setChecked(false);
    ui->checkFriday->setChecked(false);
    ui->checkSaturday->setChecked(false);
    ui->checkSunday->setChecked(false);

    qDebug()<<"clicked";
}

void CustomOneYearCalendarFrame::pressed()
{
    CalendarDropableButton *button=static_cast<CalendarDropableButton*>(sender());
    if(button==NULL)
    {
        return;
    }
    m_pressPoint=button->mapTo(this, QPoint(button->width()/2, button->height()/2));
}

void CustomOneYearCalendarFrame::doPolicyDroped(int controlPolicyId)
{
    qDebug()<<" CustomOneYearCalendarFrame::doPolicyDroped controlPolicyId="<<controlPolicyId;
    QList<ControlDate*> *controlDateList=new QList<ControlDate*>;
    if(ui->stackEditInYearAndMonth->currentWidget()==ui->pageEditInYear)
    {
        int len=m_inMothButtonList.size();
        for(int i=0; i<len; i++)
        {
            CalendarDropableButton *button=m_inMothButtonList.at(i);
            if(button->selected())
            {
                ControlDate *controlDate=new ControlDate;
                QDate date=button->date();
                controlDate->id=QString("%1_%2_%3").arg(m_groupId).arg(date.month()).arg(date.day());
                controlDate->month=date.month();
                controlDate->day=date.day();
                controlDate->groupId=m_groupId;
                controlDate->controlPolicyId=controlPolicyId;
                controlDateList->push_back(controlDate);
            }
        }
    }
    else
    {
        int len=m_monthDetailsInMonthButtonList.size();
        for(int i=0; i<len; i++)
        {
            CalendarDropableButton *button=m_monthDetailsInMonthButtonList.at(i);
            if(button->selected())
            {
                ControlDate *controlDate=new ControlDate;
                QDate date=button->date();
                controlDate->id=QString("%1_%2_%3").arg(m_groupId).arg(date.month()).arg(date.day());
                controlDate->month=date.month();
                controlDate->day=date.day();
                controlDate->groupId=m_groupId;
                controlDate->controlPolicyId=controlPolicyId;
                controlDateList->push_back(controlDate);
            }
        }
    }
    m_meshModel->controlDatesModel()->setControlDates(m_groupId, controlDateList);
}

void CustomOneYearCalendarFrame::doWeekCheckboxClicked(bool checked)
{
    int week=sender()->property("week").toInt();
    if(ui->stackEditInYearAndMonth->currentWidget()==ui->pageEditInYear)
    {
        int len=m_inMothButtonList.size();
        for(int i=0; i<len; i++)
        {
            CalendarDropableButton *button=m_inMothButtonList.at(i);
            if(button->date().dayOfWeek()==week)
            {
                button->setSelected(checked);
                button->resetStyle();
            }
        }
    }
    else
    {
        int len2=m_monthDetailsInMonthButtonList.size();
        for(int i=0; i<len2; i++)
        {
            CalendarDropableButton *button=m_monthDetailsInMonthButtonList.at(i);
            if(button->date().dayOfWeek()==week)
            {
                button->setSelected(checked);
            }
        }
    }
}

void CustomOneYearCalendarFrame::doMonthDetailsViewButtonClicked()
{
    int month=sender()->property("month").toInt();
    if(month>=QDateTime::currentDateTime().date().month())
    {
        showMonthDetails(QDateTime::currentDateTime().date().year(), month);
    }
    else
    {
        showMonthDetails(QDateTime::currentDateTime().date().year()+1, month);
    }
}

void CustomOneYearCalendarFrame::doMonthListViewButtonClicked()
{
    showMonthBriefList();
}

void CustomOneYearCalendarFrame::showMonthDetails(int year, int month)
{
    ui->stackEditInYearAndMonth->setCurrentIndex(ui->stackEditInYearAndMonth->indexOf(ui->pageEditInMonth));

    ui->labelMonthTitle_Common->setText(QString("%1 (%2)").arg(tr(QString("Month.%1").arg(month).toLatin1().data())).arg(year));
    ui->labelMonthTitle_Common->setToolTip(QString::number(year));

    m_monthDetailsInMonthButtonList.clear();

    QDate monthDate=QDate(year, month, 1);
    if(monthDate.dayOfWeek()==1)
    {
        monthDate=monthDate.addDays(-7);
    }
    else
    {
        monthDate=monthDate.addDays(-1*monthDate.dayOfWeek()+1);
    }
    int len=m_monthDetailsButtonList.size();
    for(int i=0; i<len; i++)
    {
        CalendarDropableButton *button=m_monthDetailsButtonList.at(i);
        button->setDate(monthDate);
        monthDate=monthDate.addDays(1);
        if(button->date().month()==month)
        {
            button->setInMonth(true);
            m_monthDetailsInMonthButtonList.push_back(button);
        }
        else
        {
            button->setInMonth(false);
        }
    }
    updateMonthDetailsButtons();
}

void CustomOneYearCalendarFrame::showMonthBriefList()
{
    ui->stackEditInYearAndMonth->setCurrentIndex(ui->stackEditInYearAndMonth->indexOf(ui->pageEditInYear));
}

void CustomOneYearCalendarFrame::doControlDateModelChanged()
{
    updateYearButtons();
    updateMonthDetailsButtons();
}
