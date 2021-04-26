#ifndef CALENDARDROPABLEBUTTON_H
#define CALENDARDROPABLEBUTTON_H

#include <QPushButton>
#include <QFrame>
#include <QDate>

class CalendarDropableButton : public QFrame
{
    Q_OBJECT
public:
    explicit CalendarDropableButton(QWidget *parent = 0);
    CalendarDropableButton(const QDate &date, QWidget *parent = 0);
    CalendarDropableButton(const QString &dutyTypeText, const QString &dutyDisplayText, QWidget *parent = 0);

    bool selected();
    void setSelected(bool selected);
    QString color();
    void setColor(const QString &color);
    bool inMonth();
    void setInMonth(bool inMonth);
    void setDate(const QDate &date);
    QDate date();
    void setMonthInDetailsMode(bool monthInDetailsMode);
    bool monthInDetailsMode();
    bool isDutyMode();
    QString dutyTypeText();
    void setText(const QString &text);

    void resetStyle();

protected:
    void dragEnterEvent(QDragEnterEvent *ev);
    void dropEvent(QDropEvent *ev);

private:
    QPushButton *m_button;
    bool m_selected;
    bool m_inMonth;
    QString m_color;
    QDate m_date;
    bool m_monthInDetailsMode;
    QString m_dutyTypeText;
    bool m_isDutyMode;

signals:
    void clicked();
    void pressed();
    void controlPolicyDroped(int controlPolicyId);

public slots:

};

#endif // CALENDARDROPABLEBUTTON_H
