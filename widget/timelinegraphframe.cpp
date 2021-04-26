#include "timelinegraphframe.h"
#include "ui_timelinegraphframe.h"
#include <QPainter>
#include <QMouseEvent>

#define TIME_LINE_ITEM_WIDTH    10
#define TIME_LINE_PIXMAP_WIDTH  (24*60*TIME_LINE_ITEM_WIDTH)
#define TIME_LINE_PIXMAP_HEIGHT    200

#define POWER_COLOR_NAME    "#ff6d6d"
#define POWER_VALUE_COLOR_NAME  "#b70404"
#define LUX_SENSOR_COLOR_NAME   "#ffe800"
#define MOTION_SENSOR_COLOR_NAME    "#a399ff"
#define NO_OPERATION_COLOR_NAME "#ffffff"
#define SELECTED_COLOR_NAME "#ff0000"

#define HOUR_PIXMAP_X   66
#define HOUR_PIXMAP_Y   300
#define HOUR_PIXMAP_WIDTH   600
#define HOUR_PIXMAP_HEIGHT  100


#define MINUTE_PIXMAP_X     66
#define MINUTE_PIXMAP_Y     50
#define MINUTE_PIXMAP_WIDTH     600
#define MINUTE_PIXMAP_HEIGHT    200

TimeLineGraphFrame::TimeLineGraphFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TimeLineGraphFrame)
{
    ui->setupUi(this);
    backgroundPixmap=NULL;
    timeLinePixmap=NULL;
    mousePressed=false;

    backgroundPixmap=new QPixmap(":/images/time_line_background.png");
    selectedStartMinute=0;
    selectedStartX=HOUR_PIXMAP_X;
}

TimeLineGraphFrame::~TimeLineGraphFrame()
{
    if(backgroundPixmap!=NULL)
    {
        delete backgroundPixmap;
        backgroundPixmap=NULL;
    }
    if(timeLinePixmap!=NULL)
    {
        delete timeLinePixmap;
        timeLinePixmap=NULL;
    }
    delete ui;
}

void TimeLineGraphFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void TimeLineGraphFrame::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    if(timeLineItemList.isEmpty())
    {
        return;
    }
    if(timeLinePixmap==NULL)
    {
        return;
    }
    if(timeLinePixmap==NULL)
    {
        return;
    }
    painter.drawPixmap(0, 0, backgroundPixmap->width(), backgroundPixmap->height(), *backgroundPixmap);
    painter.drawPixmap(HOUR_PIXMAP_X, HOUR_PIXMAP_Y, HOUR_PIXMAP_WIDTH, HOUR_PIXMAP_HEIGHT, *timeLinePixmap);
    QColor color=QColor(SELECTED_COLOR_NAME);
    painter.setPen(QPen(color));
    painter.drawRect(selectedStartX, HOUR_PIXMAP_Y, 24, 100);

    painter.drawPixmap(MINUTE_PIXMAP_X, MINUTE_PIXMAP_Y, *timeLinePixmap,
                       selectedStartMinute*10, 0, MINUTE_PIXMAP_WIDTH, MINUTE_PIXMAP_HEIGHT);

    for(int i=0; i<13; i++)
    {
        QString text=QString("%1:%2").arg((selectedStartMinute+i*5)/60, 2, 10, QLatin1Char('0')).arg((selectedStartMinute+i*5)%60, 2, 10, QLatin1Char('0'));
        painter.setPen(QPen(QColor("#363333")));
        painter.drawText(MINUTE_PIXMAP_X+i*50-40, MINUTE_PIXMAP_Y+MINUTE_PIXMAP_HEIGHT+5, 80, 40, Qt::AlignHCenter|Qt::AlignTop, text);
    }
}

void TimeLineGraphFrame::mousePressEvent(QMouseEvent *event)
{
    if(event->x()>HOUR_PIXMAP_X && event->x()<(event->x()+HOUR_PIXMAP_WIDTH) && event->y()>HOUR_PIXMAP_Y && event->y()<(HOUR_PIXMAP_Y+HOUR_PIXMAP_HEIGHT))
    {
        mousePressed=true;
        onMouseClickHourPixmap(event->x(), event->y());
    }
}

void TimeLineGraphFrame::mouseReleaseEvent(QMouseEvent *event)
{
    mousePressed=false;
}

void TimeLineGraphFrame::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePressed)
    {
        onMouseClickHourPixmap(event->x(), event->y());
    }
}

void TimeLineGraphFrame::onMouseClickHourPixmap(int x, int y)
{
    if(x>HOUR_PIXMAP_X && x<(HOUR_PIXMAP_X+HOUR_PIXMAP_WIDTH) && y>HOUR_PIXMAP_Y && y<(HOUR_PIXMAP_Y+HOUR_PIXMAP_HEIGHT))
    {
        selectedStartX=x-12;
        if(selectedStartX<HOUR_PIXMAP_X)
        {
            selectedStartX=HOUR_PIXMAP_X;
        }
        if(selectedStartX>(HOUR_PIXMAP_X+HOUR_PIXMAP_WIDTH-24))
        {
            selectedStartX=(HOUR_PIXMAP_X+HOUR_PIXMAP_WIDTH-24);
        }
        selectedStartMinute=(int)(((selectedStartX-HOUR_PIXMAP_X)*1.0/HOUR_PIXMAP_WIDTH)*(24*60));
        selectedStartMinute=(selectedStartMinute/5)*5;
        if(selectedStartMinute>(23*60))
        {
            selectedStartMinute=(23*60);
        }
        repaint();
    }
}

void TimeLineGraphFrame::updateGraph()
{
    if(timeLinePixmap!=NULL)
    {
        delete timeLinePixmap;
        timeLinePixmap=NULL;
    }
    timeLinePixmap=new QPixmap(TIME_LINE_PIXMAP_WIDTH, TIME_LINE_PIXMAP_HEIGHT);
    timeLinePixmap->fill(QColor(255, 255, 255, 128));
    QPainter painter(timeLinePixmap);
    QColor color;
    int len=timeLineItemList.size();
    for(int i=0; i<len-1; i++)
    {
        TimeLineControlItem *item1=timeLineItemList.at(i);
        TimeLineControlItem *item2=timeLineItemList.at(i+1);
        if(item1->controlType=="power")
        {
            color=QColor(POWER_COLOR_NAME);
            color.setAlpha(128);
        }
        else if(item1->controlType=="lux_sensor")
        {
            color=QColor(LUX_SENSOR_COLOR_NAME);
            color.setAlpha(128);
        }
        else if(item1->controlType=="motion_sensor")
        {
            color=QColor(MOTION_SENSOR_COLOR_NAME);
            color.setAlpha(128);
        }
        else if(item1->controlType=="no_operation")
        {
            color=QColor(NO_OPERATION_COLOR_NAME);
            color.setAlpha(0);
        }
        int x=item1->startMinute*TIME_LINE_ITEM_WIDTH;
        int width=(item2->startMinute-item1->startMinute)*10;
        painter.fillRect(x, 0, width, 200, color);
        if(item1->controlType=="power")
        {
            //绘制Power线条
            int y=(200-item1->powerTypePower*2);
            if(y<0)
            {
                y=0;
            }
            color=QColor(POWER_VALUE_COLOR_NAME);
            QPen pen(color);
            pen.setWidth(3);
            painter.setPen(pen);
            painter.drawLine(x, y, x+width, y);
        }
    }
    if(len>=1)
    {
        TimeLineControlItem *item=timeLineItemList.at(len-1);
        if(item->controlType=="power")
        {
            color=QColor(POWER_COLOR_NAME);
            color.setAlpha(128);
        }
        else if(item->controlType=="lux_sensor")
        {
            color=QColor(LUX_SENSOR_COLOR_NAME);
            color.setAlpha(128);
        }
        else if(item->controlType=="motion_sensor")
        {
            color=QColor(MOTION_SENSOR_COLOR_NAME);
            color.setAlpha(128);
        }
        else if(item->controlType=="no_operation")
        {
            color=QColor(NO_OPERATION_COLOR_NAME);
            color.setAlpha(0);
        }
        int x=item->startMinute*TIME_LINE_ITEM_WIDTH;
        int width=((24*60)-item->startMinute)*10;
        painter.fillRect(x, 0, width, 200, color);
        if(item->controlType=="power")
        {
            //绘制Power线条
            int y=(200-item->powerTypePower*2);
            if(y<0)
            {
                y=0;
            }
            color=QColor(POWER_VALUE_COLOR_NAME);
            QPen pen(color);
            pen.setWidth(3);
            painter.setPen(pen);
            painter.drawLine(x, y, x+width, y);
        }
    }
    this->repaint();
}
