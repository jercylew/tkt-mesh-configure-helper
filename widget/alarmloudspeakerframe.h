#ifndef ALARMLOUDSPEAKERFRAME_H
#define ALARMLOUDSPEAKERFRAME_H

#include <QFrame>
#include <QLabel>

namespace Ui {
class AlarmLoudspeakerFrame;
}

class AlarmLoudspeakerFrame : public QFrame
{
    Q_OBJECT

public:
    explicit AlarmLoudspeakerFrame(QWidget *parent = 0);
    ~AlarmLoudspeakerFrame();

    QLabel *getIconLabel();
    void setName(const QString &name);
    void setGroupColors(const QList<QColor> &groupColorList);
    void resetStyle();

protected:
    void changeEvent(QEvent *ev);

private:
    Ui::AlarmLoudspeakerFrame *ui;
    QList<QLabel*> groupLabelList;
};

#endif // ALARMLOUDSPEAKERFRAME_H
