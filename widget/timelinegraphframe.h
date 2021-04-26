#ifndef TIMELINEGRAPHFRAME_H
#define TIMELINEGRAPHFRAME_H

#include <QFrame>
#include "domain/timelinecontrolitem.h"


namespace Ui {
class TimeLineGraphFrame;
}

class TimeLineGraphFrame : public QFrame
{
    Q_OBJECT

public:
    explicit TimeLineGraphFrame(QWidget *parent = 0);
    ~TimeLineGraphFrame();

//    template<class BaseTimeLineControlItem>
    void updateGraph(QList<TimeLineControlItem*> timeLineItemList)
    {
        if(timeLineItemList.isEmpty())
        {
            return;
        }
        this->timeLineItemList.clear();
        int len=timeLineItemList.size();
        for(int i=0; i<len; i++)
        {
            this->timeLineItemList.push_back(timeLineItemList.at(i));
        }
        this->updateGraph();
    }

protected:
    void changeEvent(QEvent *ev);

protected:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    Ui::TimeLineGraphFrame *ui;

    QList<TimeLineControlItem*> timeLineItemList;

    QPixmap *backgroundPixmap;

    QPixmap *timeLinePixmap;

    bool mousePressed;

    int selectedStartMinute;

    int selectedStartX;

    void onMouseClickHourPixmap(int x, int y);

    void updateGraph();
};

#endif // TIMELINEGRAPHFRAME_H
