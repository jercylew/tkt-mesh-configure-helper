#ifndef CUSTOMONEYEARCALENDARFRAME_H
#define CUSTOMONEYEARCALENDARFRAME_H

#include <QFrame>
#include <QPushButton>
#include <QMap>
#include <QList>
#include "calendardropablebutton.h"

namespace Ui {
class CustomOneYearCalendarFrame;
}

class MeshModel;

class CustomOneYearCalendarFrame : public QFrame
{
    Q_OBJECT

public:
    typedef enum{
        DefaultSchedule,
        MondaySchedule,
        TuesdaySchedule,
        WednesdaySchedule,
        ThursdaySchedule,
        FridaySchedule,
        SaturdaySchedule,
        SundaySchedule,
        CustomSchedule
    }ScheduleType;

    explicit CustomOneYearCalendarFrame(MeshModel *meshModel, quint8 groupId, QWidget *parent = 0);
    ~CustomOneYearCalendarFrame();

public slots:
    void doControlDateModelChanged();

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

private:
    Ui::CustomOneYearCalendarFrame *ui;
    QMap<QDate, CalendarDropableButton*> m_dateButtonMap;
    QMap<CalendarDropableButton*, QDate> m_buttonDateMap;
    QList<CalendarDropableButton*> m_inMothButtonList;
    QList<CalendarDropableButton*> m_monthDetailsButtonList;
    QList<CalendarDropableButton*> m_monthDetailsInMonthButtonList;
    QMap<int, QFrame*> m_frameMonthMap;

    bool m_mousePressed;
    QPoint m_pressPoint;
    QPoint m_currentPoint;
    bool ctrlPressed();

    void updateSelection();
    MeshModel *m_meshModel;
    quint8 m_groupId;

    void updateYearButtons();
    void updateMonthDetailsButtons();

private slots:
    void clicked();
    void pressed();
    void doPolicyDroped(int controlPolicyId);
    void doWeekCheckboxClicked(bool checked);
    void doMonthDetailsViewButtonClicked();
    void doMonthListViewButtonClicked();

    void showMonthDetails(int year, int month);
    void showMonthBriefList();
};

#endif // CUSTOMONEYEARCALENDARFRAME_H
